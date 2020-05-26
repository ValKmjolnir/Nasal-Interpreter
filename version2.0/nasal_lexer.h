#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#define IS_IDENTIFIER_HEAD(c) (c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')
#define IS_IDENTIFIER_BODY(c) (c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')||('0'<=c&&c<='9')
#define IS_NUMBER_HEAD(c)     ('0'<=c&&c<='9')
#define IS_NUMBER_BODY(c)     ('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c&&c<='F')||(c=='e'||c=='E'||c=='.'||c=='x'||c=='o')
#define IS_STRING_HEAD(c)     (c=='\''||c=='\"')
// single operators have only one character
#define IS_SINGLE_OPRATOR(c)  (c=='('||c==')'||c=='['||c==']'||c=='{'||c=='}'||c==','||c==';'||c=='|'||c==':'||\
							   c=='?'||c=='.'||c=='`'||c=='&'||c=='@'||c=='%'||c=='$'||c=='^'||c=='\\')
// calculation operators may have two chars, for example: += -= *= /= ~= != == >= <=
#define IS_CALC_OPERATOR(c)   (c=='='||c=='+'||c=='-'||c=='*'||c=='!'||c=='/'||c=='<'||c=='>'||c=='~')
#define IS_NOTE_HEAD(c)       (c=='#')

/* filenames of lib files */
#ifndef LIB_FILE_NUM
#define LIB_FILE_NUM 11
const std::string lib_filename[LIB_FILE_NUM]=
{
	"lib/base.nas",
	"lib/bits.nas",
	"lib/io.nas",
	"lib/math.nas",
	"lib/readline.nas",
	"lib/regex.nas",
	"lib/sqlite.nas",
	"lib/system.nas",
	"lib/thread.nas",
	"lib/unix.nas",
	"lib/utf8.nas"
};
#endif

/* reserve words */
#ifndef RESERVE_WORD_NUM
#define RESERVE_WORD_NUM 15
std::string reserve_word[RESERVE_WORD_NUM]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","and","or","nil"
};
/* check if an identifier is a reserve word */
int is_reserve_word(std::string str)
{
	for(int i=0;i<RESERVE_WORD_NUM;++i)
		if(reserve_word[i]==str)
			return __token_reserve_word;
	return __token_identifier;
}
#endif

class resource_file
{
private:
	std::vector<char> source_code;
public:
	/*
		delete_all_source: clear all the source codes in std::list<char> resource
		input_file       : input source codes by filenames
		load_lib_file    : input lib source codes
		get_source       : get the std::vector<char> source_code
		print_resource   : print source codes
	*/
	void delete_all_source();
	void input_file(std::string);
	void load_lib_file();
	std::vector<char>& get_source();
	void print_resource();
};

/* struct token: mainly used in nasal_lexer and nasal_parse*/
struct token
{
	int line;
	int type;
	std::string str;
	token& operator=(const token& tmp)
	{
		line=tmp.line;
		type=tmp.type;
		str =tmp.str;
		return *this;
	}
};

class nasal_lexer
{
private:
	std::list<token> token_list;
	std::list<token> detail_token_list;
	int error;
	std::string identifier_gen(std::vector<char>&,int&,int&);
	std::string number_gen    (std::vector<char>&,int&,int&);
	std::string string_gen    (std::vector<char>&,int&,int&);
public:
	/*
		identifier_gen       : scan the source codes and generate identifiers
		number_gen           : scan the source codes and generate numbers
		string_gen           : scan the source codes and generate strings
		print_token_list     : print generated token list
		scanner              : scan the source codes and generate tokens
		generate_detail_token: recognize and change token types to detailed types that can be processed by nasal_parse
		get_error            : get the number of errors that occurred when generating tokens
		get_detail_token     : output the detailed tokens,must be used after generate_detail_token()
	*/
	nasal_lexer();
	~nasal_lexer();
	void delete_all_tokens();
	void print_token_list();
	void scanner(std::vector<char>&);
	void generate_detail_token();
	int  get_error();
	std::list<token>& get_detail_token_list();
};


void resource_file::delete_all_source()
{
	std::vector<char> tmp;
	source_code.clear();
	source_code.swap(tmp);
	// use tmp's destructor to delete the memory space that source_code used before
	return;
}
void resource_file::input_file(std::string filename)
{
	char c=0;
	std::ifstream fin(filename,std::ios::binary);
	if(fin.fail())
	{
		std::cout<<">> [Resource] cannot open file \'"<<filename<<"\' ."<<std::endl;
		fin.close();
		return;
	}
	while(!fin.eof())
	{
		c=fin.get();
		if(fin.eof()) break;
		source_code.push_back(c);
	}
	fin.close();
	return;
}
void resource_file::load_lib_file()
{
	source_code.clear();
	for(int i=0;i<LIB_FILE_NUM;++i)
	{
		std::ifstream fin(lib_filename[i],std::ios::binary);
		if(fin.fail())
			std::cout<<">> [Resource] fatal error: lack \'"<<lib_filename[i]<<"\'"<<std::endl;
		else
		{
			char c=0;
			while(!fin.eof())
			{
				c=fin.get();
				if(fin.eof()) break;
				source_code.push_back(c);
			}
		}
		fin.close();
	}
	return;
}
std::vector<char>& resource_file::get_source()
{
	return source_code;
}
void resource_file::print_resource()
{
	int size=source_code.size();
	int line=1;
	std::cout<<line<<"\t";
	for(int i=0;i<size;++i)
	{
		if(32<=source_code[i]) std::cout<<source_code[i];
		else if(source_code[i]<0)
		{
			// print unicode
			std::string tmp="";
			for(;i<size;++i)
			{
				if(source_code[i]>=0) break;
				tmp.push_back(source_code[i]);
			}
			std::cout<<tmp;--i;
		}
		else std::cout<<" ";
		if(i<size && source_code[i]=='\n')
		{
			++line;
			std::cout<<std::endl<<line<<"\t";
		}
	}
	std::cout<<std::endl;
	return;
}

std::string nasal_lexer::identifier_gen(std::vector<char>& res,int& ptr,int& line)
{
	std::string token_str="";
	while(IS_IDENTIFIER_BODY(res[ptr]))
	{
		token_str+=res[ptr];
		++ptr;
		if(ptr>=res.size()) break;
	}
	// check dynamic identifier "..."
	if(ptr+2<res.size() && res[ptr]=='.' && res[ptr+1]=='.' && res[ptr+2]=='.')
	{
		token_str+="...";
		ptr+=3;
	}
	return token_str;
	// after running this process, ptr will point to the next token's beginning character
}

std::string nasal_lexer::number_gen(std::vector<char>& res,int& ptr,int& line)
{
	bool scientific_notation=false;// numbers like 1e8 are scientific_notation
	std::string token_str="";
	while(IS_NUMBER_BODY(res[ptr]))
	{
		token_str+=res[ptr];
		if(res[ptr]=='e' || res[ptr]=='E')
		{
			scientific_notation=true;
			++ptr;
			break;
		}
		++ptr;
		if(ptr>=res.size())
			break;
	}
	if(scientific_notation && ptr<res.size())
	{
		if(res[ptr]=='-')
		{
			token_str+='-';
			++ptr;
		}
		while(ptr<res.size() && '0'<=res[ptr] && res[ptr]<='9')
		{
			token_str+=res[ptr];
			++ptr;
		}
	}
	if(!check_numerable_string(token_str))
	{
		++error;
		std::cout<<">> [Lexer] line "<<line<<": "<<token_str<<" is not a numerable string."<<std::endl;
		token_str="0";
	}
	return token_str;
}
std::string nasal_lexer::string_gen(std::vector<char>& res,int& ptr,int& line)
{
	std::string token_str="";
	char str_begin=res[ptr];
	++ptr;
	if(ptr>=res.size()) return token_str;
	while(ptr<res.size() && res[ptr]!=str_begin)
	{
		token_str+=res[ptr];
		if(res[ptr]=='\n') ++line;
		if(res[ptr]=='\\' && ptr+1<res.size())
		{
			++ptr;
			switch(res[ptr])
			{
				case '\\':token_str.pop_back();token_str.push_back('\\');break;
				case 'r': token_str.pop_back();token_str.push_back('\r');break;
				case 't': token_str.pop_back();token_str.push_back('\t');break;
				case 'n': token_str.pop_back();token_str.push_back('\n');break;
				case '\'':token_str.pop_back();token_str.push_back('\'');break;
				case '\"':token_str.pop_back();token_str.push_back('\"');break;
				default:  token_str.push_back(res[ptr]);break;
			}
		}
		++ptr;
		if(ptr>=res.size()) break;
	}
	// check if this string ends with a " or '
	if(ptr>=res.size())
	{
		++error;
		std::cout<<">> [Lexer] line "<<line<<": this string must have a \' "<<str_begin<<" \' as its end."<<std::endl;
		--ptr;
	}
	++ptr;
	return token_str;
}
nasal_lexer::nasal_lexer()
{
	token_list.clear();
	detail_token_list.clear();
	error=0;
	return;
}
nasal_lexer::~nasal_lexer()
{
	token_list.clear();
	detail_token_list.clear();
	return;
}
void nasal_lexer::delete_all_tokens()
{
	token_list.clear();
	detail_token_list.clear();
	error=0;
	return;
}
void nasal_lexer::print_token_list()
{
	for(std::list<token>::iterator i=token_list.begin();i!=token_list.end();++i)
	{
		std::cout<<"line "<<i->line<<" ( ";
		print_lexer_token(i->type);
		std::cout<<" | "<<i->str<<" )"<<std::endl;
	}
	return;
}
void nasal_lexer::scanner(std::vector<char>& res)
{
	token_list.clear();
	detail_token_list.clear();
	error=0;
	
	int line=1;
	std::string token_str;
	int ptr=0;
	while(ptr<res.size())
	{
		while(ptr<res.size() && (res[ptr]==' ' || res[ptr]=='\n' || res[ptr]=='\t' || res[ptr]=='\r' || res[ptr]<0))
		{
			// these characters will be ignored, and '\n' will cause ++line
			if(res[ptr]=='\n') ++line;
			++ptr;
		}
		if(ptr>=res.size()) break;
		if(IS_IDENTIFIER_HEAD(res[ptr]))
		{
			token_str=identifier_gen(res,ptr,line);
			token new_token;
			new_token.line=line;
			new_token.type=is_reserve_word(token_str);
			new_token.str=token_str;
			token_list.push_back(new_token);
		}
		else if(IS_NUMBER_HEAD(res[ptr]))
		{
			token_str=number_gen(res,ptr,line);
			token new_token;
			new_token.line=line;
			new_token.type=__token_number;
			new_token.str=token_str;
			token_list.push_back(new_token);
		}
		else if(IS_STRING_HEAD(res[ptr]))
		{
			token_str=string_gen(res,ptr,line);
			token new_token;
			new_token.line=line;
			new_token.type=__token_string;
			new_token.str=token_str;
			token_list.push_back(new_token);
		}
		else if(IS_SINGLE_OPRATOR(res[ptr]))
		{
			token_str="";
			token_str+=res[ptr];
			token new_token;
			new_token.line=line;
			new_token.type=__token_operator;
			new_token.str=token_str;
			token_list.push_back(new_token);
			++ptr;
		}
		else if(IS_CALC_OPERATOR(res[ptr]))
		{
			// get calculation operator
			token_str="";
			token_str+=res[ptr];
			++ptr;
			if(ptr<res.size() && res[ptr]=='=')
			{
				token_str+=res[ptr];
				++ptr;
			}
			token new_token;
			new_token.line=line;
			new_token.type=__token_operator;
			new_token.str=token_str;
			token_list.push_back(new_token);
		}
		else if(IS_NOTE_HEAD(res[ptr]))
		{
			// avoid note
			while(ptr<res.size() && res[ptr]!='\n')
				++ptr;
			// after this process ptr will point to a '\n'
			// don't ++ptr then the counter for line can work correctly
		}
		else
		{
			++error;
			std::cout<<">> [Lexer] line "<<line<<": unknown char."<<std::endl;
			++ptr;
		}
	}
	std::cout<<">> [Lexer] complete scanning. "<<error<<" error(s)."<<std::endl;
	return;
}
void nasal_lexer::generate_detail_token()
{
	token detail_token;
	detail_token_list.clear();
	for(std::list<token>::iterator i=token_list.begin();i!=token_list.end();++i)
	{
		if(i->type==__token_number)
		{
			detail_token.line=i->line;
			detail_token.str =i->str;
			detail_token.type=__number;
			detail_token_list.push_back(detail_token);
		}
		else if(i->type==__token_string)
		{
			detail_token.line=i->line;
			detail_token.str =i->str;
			detail_token.type=__string;
			detail_token_list.push_back(detail_token);
		}
		else if(i->type==__token_reserve_word)
		{
			detail_token.line=i->line;
			detail_token.str ="";
			if     (i->str=="for"     ) detail_token.type=__for;
			else if(i->str=="foreach" ) detail_token.type=__foreach;
			else if(i->str=="forindex") detail_token.type=__forindex;
			else if(i->str=="while"   ) detail_token.type=__while;
			else if(i->str=="var"     ) detail_token.type=__var;
			else if(i->str=="func"    ) detail_token.type=__func;
			else if(i->str=="break"   ) detail_token.type=__break;
			else if(i->str=="continue") detail_token.type=__continue;
			else if(i->str=="return"  ) detail_token.type=__return;
			else if(i->str=="if"      ) detail_token.type=__if;
			else if(i->str=="else"    ) detail_token.type=__else;
			else if(i->str=="elsif"   ) detail_token.type=__elsif;
			else if(i->str=="nil"     ) detail_token.type=__nil;
			else if(i->str=="and"     ) detail_token.type=__and_operator;
			else if(i->str=="or"      ) detail_token.type=__or_operator;
			detail_token_list.push_back(detail_token);
		}
		else if(i->type==__token_identifier)
		{
			detail_token.line=i->line;
			detail_token.str =i->str;
			if(i->str.length()<=3)
				detail_token.type=__id;
			else
			{
				std::string tempstr=i->str;
				int strback=tempstr.length()-1;
				if(tempstr.length()>3 &&tempstr[strback]=='.' && tempstr[strback-1]=='.' && tempstr[strback-2]=='.')
				{
					detail_token.str="";
					for(int j=0;j<tempstr.length()-3;++j)
						detail_token.str+=tempstr[j];
					detail_token.type=__dynamic_id;
				}
				else
					detail_token.type=__id;
			}
			detail_token_list.push_back(detail_token);
		}
		else if(i->type==__token_operator)
		{
			detail_token.line=i->line;
			detail_token.str ="";
			if     (i->str=="+" ) detail_token.type=__add_operator;
			else if(i->str=="-" ) detail_token.type=__sub_operator;
			else if(i->str=="*" ) detail_token.type=__mul_operator;
			else if(i->str=="/" ) detail_token.type=__div_operator;
			else if(i->str=="~" ) detail_token.type=__link_operator;
			else if(i->str=="+=") detail_token.type=__add_equal;
			else if(i->str=="-=") detail_token.type=__sub_equal;
			else if(i->str=="*=") detail_token.type=__mul_equal;
			else if(i->str=="/=") detail_token.type=__div_equal;
			else if(i->str=="~=") detail_token.type=__link_equal;
			else if(i->str=="=" ) detail_token.type=__equal;
			else if(i->str=="==") detail_token.type=__cmp_equal;
			else if(i->str=="!=") detail_token.type=__cmp_not_equal;
			else if(i->str=="<" ) detail_token.type=__cmp_less;
			else if(i->str=="<=") detail_token.type=__cmp_less_or_equal;
			else if(i->str==">" ) detail_token.type=__cmp_more;
			else if(i->str==">=") detail_token.type=__cmp_more_or_equal;
			else if(i->str==";" ) detail_token.type=__semi;
			else if(i->str=="." ) detail_token.type=__dot;
			else if(i->str==":" ) detail_token.type=__colon;
			else if(i->str=="," ) detail_token.type=__comma;
			else if(i->str=="?" ) detail_token.type=__ques_mark;
			else if(i->str=="!" ) detail_token.type=__nor_operator;
			else if(i->str=="[" ) detail_token.type=__left_bracket;
			else if(i->str=="]" ) detail_token.type=__right_bracket;
			else if(i->str=="(" ) detail_token.type=__left_curve;
			else if(i->str==")" ) detail_token.type=__right_curve;
			else if(i->str=="{" ) detail_token.type=__left_brace;
			else if(i->str=="}" ) detail_token.type=__right_brace;
			else
			{
				++error;
				std::cout<<">> [Lexer] line "<<detail_token.line<<": unknown operator \'"<<i->str<<"\'."<<std::endl;
				detail_token.type=__unknown_operator;
			}
			detail_token_list.push_back(detail_token);
		}
	}
	std::cout<<">> [Lexer] complete generating. "<<error<<" error(s)."<<std::endl;
	return;
}
int nasal_lexer::get_error()
{
	return error;
}
std::list<token>& nasal_lexer::get_detail_token_list()
{
	return detail_token_list;
}
#endif
