#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

/*
	__token_reserve_word:
		for,foreach,forindex,while : loop
		var,func : definition
		break,continue : in loop
		return : in function
		if,else,elsif : if-else statement
		and,or : calculation
		nil : special type
	__token_identifier:
		must begin with '_' or 'a'~'z' or 'A'~'Z'
		can include '_' or 'a'~'z' or 'A'~'Z' or '0'~'9'
	__token_string:
		example:
			"string"
			'string'
		if a string does not end with " or ' then lexer will throw an error
	__token_number:
		example:
			2147483647 (integer)
			2.71828    (float)
			0xdeadbeef (hex) or 0xDEADBEEF (hex)
			0o170001   (oct)
	__token_operator:
		! + - * / ~
		= += -= *= /= ~=
		== != > >= < <= ('and'  'or' are operators too but they are recognized as operator in generate_detail_token())
		() [] {} ; , . : ?
		others: __unknown_operator
*/

const std::string lib_filename[10]=
{
	"lib/base.nas",
	"lib/bits.nas",
	"lib/io.nas",
	"lib/math.nas",
	"lib/readline.nas",
	"lib/regex.nas",
	"lib/sqlite.nas",
	"lib/thread.nas",
	"lib/unix.nas",
	"lib/utf8.nas"
};

std::string reserve_word[15]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","and","or","nil"
};

int is_reserve_word(std::string str)
{
	for(int i=0;i<15;++i)
		if(reserve_word[i]==str)
			return __token_reserve_word;
	return __token_identifier;
}

class resource_file
{
	private:
		std::list<char> resource;
	public:
		/*
		resource_file();
		~resource_file();
		void delete_all_source();
		void input_file(std::string);
		void load_lib_file();
		std::list<char>& get_source();
		void print_resource();
		*/
		resource_file()
		{
			resource.clear();
			return;
		}
		~resource_file()
		{
			resource.clear();
			return;
		}
		void delete_all_source()
		{
			resource.clear();
			return;
		}
		void input_file(std::string filename)
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
				if(fin.eof())
					break;
				resource.push_back(c);
			}
			fin.close();
			return;
		}
		void load_lib_file()
		{
			resource.clear();
			for(int i=0;i<10;++i)
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
						if(fin.eof())
							break;
						resource.push_back(c);
					}
				}
				fin.close();
			}
			return;
		}
		std::list<char>& get_source()
		{
			return resource;
		}
		void print_resource()
		{
			int line=1;
			std::cout<<line<<"\t";
			for(std::list<char>::iterator i=resource.begin();i!=resource.end();++i)
			{
				if(32<=*i)
					std::cout<<*i;
				else
					std::cout<<" ";
				if(*i=='\n')
				{
					++line;
					std::cout<<std::endl<<line<<"\t";
				}
			}
			std::cout<<std::endl;
			return;
		}
};

struct token
{
	int line;
	int type;
	std::string str;
	token& operator=(const token& tmp)
	{
		line=tmp.line;
		type=tmp.type;
		str=tmp.str;
		return *this;
	}
};

class nasal_lexer
{
	private:
		std::list<token> token_list;
		std::list<token> detail_token_list;
		int error;
		std::string utf8_clear(std::string tmp)
		{
			/*
			0xxx xxxx 0x0  1 byte
			110x xxxx 0xc0 2 byte
			1110 xxxx 0xe0 3 byte
			1111 0xxx 0xf0 4 byte
			1111 10xx 0xf8 5 byte
			1111 110x 0xfc 6 byte
			bytes after it is:
			10xx xxxx 0x80
			
			so utf-8 format is:
			0xxxxxxx
			110xxxxx 10xxxxxx
			1110xxxx 10xxxxxx 10xxxxxx
			11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
			unsigned char utf8head[6]={0x0,0xc0,0xe0,0xf0,0xf8,0xfc};
			std::string ret="";
			for(int i=0;i<tmp.length();++i)
			{
				if(tmp[i]>=0)
					ret+=tmp[i];
				else
				{
					int utf8byte=0;
					for(int j=5;j>=0;--j)
						if((tmp[i] & utf8head[j])==utf8head[j])
						{
							utf8byte=j;
							break;
						}
					for(int j=0;j<utf8byte;++j)
						++i;
					ret+='?';
				}
			}
			return ret;
		}
	public:
		/*
		nasal_lexer();
		~nasal_lexer();
		void print_token_list();
		void scanner(std::list<char>&);
		void generate_detail_token();
		int get_error();
		std::list<token>& get_detail_token();
		*/
		nasal_lexer()
		{
			token_list.clear();
			detail_token_list.clear();
			error=0;
			return;
		}
		~nasal_lexer()
		{
			token_list.clear();
			detail_token_list.clear();
			return;
		}
		void delete_all_tokens()
		{
			token_list.clear();
			detail_token_list.clear();
			error=0;
			return;
		}
		void print_token_list()
		{
			for(std::list<token>::iterator i=token_list.begin();i!=token_list.end();++i)
			{
				std::cout<<"line "<<i->line<<" ( ";
				print_lexer_token(i->type);
				std::cout<<" | "<<i->str<<" )"<<std::endl;
			}
			return;
		}
		void scanner(std::list<char>& res)
		{
			token_list.clear();
			detail_token_list.clear();
			error=0;
			
			int line=1;
			std::string token_str;
			std::list<char>::iterator ptr=res.begin();
			while(ptr!=res.end())
			{
				while(ptr!=res.end() && (*ptr==' ' || *ptr=='\n' || *ptr=='\t' || *ptr=='\r' || *ptr<0 || *ptr>127))
				{
					if(*ptr=='\n')
						++line;
					++ptr;
				}
				if(ptr==res.end())
					break;
				
				if(*ptr=='_' || ('a'<=*ptr && *ptr<='z') || ('A'<=*ptr && *ptr<='Z'))
				{
					// get identifier or reserve word
					token_str="";
					while(*ptr=='_' || ('a'<=*ptr && *ptr<='z') || ('A'<=*ptr && *ptr<='Z') || ('0'<=*ptr && *ptr<='9'))
					{
						token_str+=*ptr;
						++ptr;
						if(ptr==res.end())
							break;
					}
					// check dynamic identifier "..."
					if(*ptr=='.')
					{
						++ptr;
						if(ptr!=res.end() && *ptr=='.')
						{
							++ptr;
							if(ptr!=res.end() && *ptr=='.')
							{
								token_str+="...";
								++ptr;
							}
							else
							{
								--ptr;
								--ptr;
							}
						}
						else
							--ptr;
					}
					token new_token;
					new_token.line=line;
					new_token.type=is_reserve_word(token_str);
					new_token.str=token_str;
					token_list.push_back(new_token);
					if(ptr==res.end())
						break;
				}
				else if('0'<=*ptr && *ptr<='9')
				{
					token_str="";
					while(('0'<=*ptr && *ptr<='9') || ('a'<=*ptr && *ptr<='f') || ('A'<=*ptr && *ptr<='F') || *ptr=='.' || *ptr=='x' || *ptr=='o')
					{
						token_str+=*ptr;
						++ptr;
						if(ptr==res.end())
							break;
					}
					if(!check_numerable_string(token_str))
					{
						++error;
						std::cout<<">> [Lexer] line "<<line<<": "<<token_str<<" is not a numerable string."<<std::endl;
						token_str="0";
					}
					token new_token;
					new_token.line=line;
					new_token.type=__token_number;
					new_token.str=token_str;
					token_list.push_back(new_token);
					if(ptr==res.end())
						break;
				}
				else if(*ptr=='(' || *ptr==')' || *ptr=='[' || *ptr==']' || *ptr=='{' ||
						*ptr=='}' || *ptr==',' || *ptr==';' || *ptr=='|' || *ptr==':' ||
						*ptr=='?' || *ptr=='.' || *ptr=='`' || *ptr=='&' || *ptr=='@' ||
						*ptr=='%' || *ptr=='$' || *ptr=='^' || *ptr=='\\')
				{
					token_str="";
					token_str+=*ptr;
					token new_token;
					new_token.line=line;
					new_token.type=__token_operator;
					new_token.str=token_str;
					token_list.push_back(new_token);
					++ptr;
					if(ptr==res.end())
						break;
				}
				else if(*ptr=='\'' || *ptr=='\"')
				{
					// get string
					char str_begin=*ptr;
					token_str="";
					++ptr;
					if(ptr==res.end())
						break;
					while(*ptr!=str_begin && ptr!=res.end())
					{
						token_str+=*ptr;
						if(*ptr=='\\')
						{
							++ptr;
							token_str+=*ptr;
						}
						++ptr;
						if(ptr==res.end())
							break;
					}
					// check if this string ends with a " or '
					if(ptr==res.end() || *ptr!=str_begin)
					{
						++error;
						std::cout<<">> [Lexer] line "<<line<<": this string must have a \' "<<str_begin<<" \' as its end."<<std::endl;
						--ptr;
					}
					else
					{
						token new_token;
						new_token.line=line;
						new_token.type=__token_string;
						new_token.str=utf8_clear(token_str);
						token_list.push_back(new_token);
					}
					++ptr;
					if(ptr==res.end())
						break;
				}
				else if(*ptr=='=' || *ptr=='+' || *ptr=='-' || *ptr=='*' || *ptr=='!' || *ptr=='/' || *ptr=='<' || *ptr=='>' || *ptr=='~')
				{
					// get calculation operator
					token_str="";
					token_str+=*ptr;
					++ptr;
					if(ptr!=res.end() && *ptr=='=')
					{
						token_str+=*ptr;
						++ptr;
					}
					token new_token;
					new_token.line=line;
					new_token.type=__token_operator;
					new_token.str=token_str;
					token_list.push_back(new_token);
					if(ptr==res.end())
						break;
				}
				else if(*ptr=='#')
				{
					// avoid note
					while(ptr!=res.end() && *ptr!='\n')
						++ptr;
					if(ptr==res.end())
						break;
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
		void generate_detail_token()
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
					if(i->str=="for")           detail_token.type=__for;
					else if(i->str=="foreach")  detail_token.type=__foreach;
					else if(i->str=="forindex") detail_token.type=__forindex;
					else if(i->str=="while")    detail_token.type=__while;
					else if(i->str=="var")      detail_token.type=__var;
					else if(i->str=="func")     detail_token.type=__func;
					else if(i->str=="break")    detail_token.type=__break;
					else if(i->str=="continue") detail_token.type=__continue;
					else if(i->str=="return")   detail_token.type=__return;
					else if(i->str=="if")       detail_token.type=__if;
					else if(i->str=="else")     detail_token.type=__else;
					else if(i->str=="elsif")    detail_token.type=__elsif;
					else if(i->str=="nil")      detail_token.type=__nil;
					else if(i->str=="and")      detail_token.type=__and_operator;
					else if(i->str=="or")       detail_token.type=__or_operator;
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
					if(i->str=="+")       detail_token.type=__add_operator;
					else if(i->str=="-")  detail_token.type=__sub_operator;
					else if(i->str=="*")  detail_token.type=__mul_operator;
					else if(i->str=="/")  detail_token.type=__div_operator;
					else if(i->str=="~")  detail_token.type=__link_operator;
					else if(i->str=="+=") detail_token.type=__add_equal;
					else if(i->str=="-=") detail_token.type=__sub_equal;
					else if(i->str=="*=") detail_token.type=__mul_equal;
					else if(i->str=="/=") detail_token.type=__div_equal;
					else if(i->str=="~=") detail_token.type=__link_equal;
					else if(i->str=="=")  detail_token.type=__equal;
					else if(i->str=="==") detail_token.type=__cmp_equal;
					else if(i->str=="!=") detail_token.type=__cmp_not_equal;
					else if(i->str=="<")  detail_token.type=__cmp_less;
					else if(i->str=="<=") detail_token.type=__cmp_less_or_equal;
					else if(i->str==">")  detail_token.type=__cmp_more;
					else if(i->str==">=") detail_token.type=__cmp_more_or_equal;
					else if(i->str==";")  detail_token.type=__semi;
					else if(i->str==".")  detail_token.type=__dot;
					else if(i->str==":")  detail_token.type=__colon;
					else if(i->str==",")  detail_token.type=__comma;
					else if(i->str=="?")  detail_token.type=__ques_mark;
					else if(i->str=="!")  detail_token.type=__nor_operator;
					else if(i->str=="[")  detail_token.type=__left_bracket;
					else if(i->str=="]")  detail_token.type=__right_bracket;
					else if(i->str=="(")  detail_token.type=__left_curve;
					else if(i->str==")")  detail_token.type=__right_curve;
					else if(i->str=="{")  detail_token.type=__left_brace;
					else if(i->str=="}")  detail_token.type=__right_brace;
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
		int get_error()
		{
			return error;
		}
		std::list<token>& get_detail_token_list()
		{
			return detail_token_list;
		}
};

#endif
