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
			0xdeadbeef (hex)
			0o170001   (oct)
	__token_operator:
		! + - * / ~
		= += -= *= /= ~=
		== != > >= < <= ('and'  'or' are operators too but they are recognized as operator in generate_detail_token())
		() [] {} ; , . : ?
		others: __unknown_operator
*/

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

bool check_number(std::string str)
{
	if(str.length()>1 && str[0]=='-')
	{
		// this statements only used in "input" function
		// but in lexer this statements are useless
		// because lexer judge a number that begins with 0~9 (or 0x for hex & 0o for oct)
		std::string temp="";
		for(int i=1;i<str.length();++i)
			temp+=str[i];
		str=temp;
	}
	if(str.length()==1)
		return true;
	else if(str.length()==2 && '0'<str[0] && str[0]<='9' && '0'<=str[1] && str[1]<='9')
		return true;
	else if(str.length()>=3 && str[0]=='0' && str[1]=='x')
	{
		for(int i=2;i<str.length();++i)
		{
			if('0'<=str[i] && str[i]<='9' || 'a'<=str[i] && str[i]<='f' || 'A'<=str[i] && str[i]<='F')
				;
			else
				return false;
		}
		return true;
	}
	else if(str.length()>=3 && str[0]=='0' && str[1]=='o')
	{
		for(int i=2;i<str.length();++i)
		{
			if('0'<=str[i] && str[i]<='7')
				;
			else
				return false;
		}
		return true;
	}
	else
	{
		int dotcnt=0;
		for(int i=0;i<str.length();++i)
		{
			if(str[i]=='.')
				++dotcnt;
			else if(!('0'<=str[i] && str[i]<='9'))
				return false;
		}
		if(dotcnt>1)
			return false;
		if(str[0]=='.')
			return false;
		if(!dotcnt && str[0]=='0')
			return false;
		return true;
	}
	return false;
}

class resource_file
{
	private:
		std::list<char> libsource;
		std::list<char> resource;
		std::list<char> total;
	public:
		/*
		resource_file();
		~resource_file();
		void delete_all_source();
		void input_file(std::string);
		void load_lib_file();
		std::list<char>& get_source();
		void print_resource(bool);
		*/
		resource_file()
		{
			libsource.clear();
			resource.clear();
			total.clear();
			return;
		}
		~resource_file()
		{
			libsource.clear();
			resource.clear();
			total.clear();
			return;
		}
		void delete_all_source()
		{
			libsource.clear();
			resource.clear();
			total.clear();
			return;
		}
		void input_file(std::string filename)
		{
			char c=0;
			std::ifstream fin(filename,std::ios::binary);
			if(fin.fail())
			{
				std::cout<<">>[Resource] cannot open file \'"<<filename<<"\' ."<<std::endl;
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
			libsource.clear();
			return;
		}
		std::list<char>& get_source()
		{
			total.clear();
			for(std::list<char>::iterator i=libsource.begin();i!=libsource.end();++i)
				total.push_back(*i);
			for(std::list<char>::iterator i=resource.begin();i!=resource.end();++i)
				total.push_back(*i);
			return total;
		}
		void print_resource(bool withlib)
		{
			std::list<char> tmp;
			if(withlib)
				for(std::list<char>::iterator i=libsource.begin();i!=libsource.end();++i)
					tmp.push_back(*i);
			for(std::list<char>::iterator i=resource.begin();i!=resource.end();++i)
				tmp.push_back(*i);
			
			int line=1;
			std::cout<<line<<"\t";
			for(std::list<char>::iterator i=tmp.begin();i!=tmp.end();++i)
			{
				if(32<=*i && *i<128)
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
};

class nasal_lexer
{
	private:
		std::list<token> token_list;
		std::list<token> detail_token;
		int error;
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
			detail_token.clear();
			error=0;
			return;
		}
		~nasal_lexer()
		{
			token_list.clear();
			detail_token.clear();
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
			detail_token.clear();
			error=0;
			
			int line=1;
			std::string token_str;
			std::list<char>::iterator ptr=res.begin();
			while(ptr!=res.end())
			{
				while(*ptr==' ' || *ptr=='\n' || *ptr=='\t' || *ptr=='\r' || *ptr<0 || *ptr>127)
				{
					if(*ptr=='\n')
						++line;
					++ptr;
					if(ptr==res.end())
						break;
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
					if(!check_number(token_str))
					{
						++error;
						std::cout<<">>[Lexer-error] line "<<line<<": error number "<<token_str<<"."<<std::endl;
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
					while(*ptr!=str_begin && *ptr!='\n')
					{
						token_str+=*ptr;
						if(*ptr=='\\')
						{
							++ptr;
							if(ptr!=res.end())
							{
								token_str+=*ptr;
								++ptr;
							}
						}
						else
							++ptr;
						if(ptr==res.end())
							break;
					}
					// check if this string ends with a " or '
					if(ptr==res.end() || *ptr!=str_begin)
					{
						++error;
						std::cout<<">>[Lexer-error] line "<<line<<": this string must have a \' "<<str_begin<<" \' as its end."<<std::endl;
						if(ptr==res.end())
							break;
					}
					else
					{
						token new_token;
						new_token.line=line;
						new_token.type=__token_string;
						new_token.str=token_str;
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
					std::cout<<">>[Lexer-error] line "<<line<<": unknown char."<<std::endl;
					++ptr;
				}
			}
			std::cout<<">>[Pre-lexer] complete scanning. "<<error<<" error(s)."<<std::endl;
			return;
		}
		void generate_detail_token()
		{
			token detail;
			detail_token.clear();
			for(std::list<token>::iterator i=token_list.begin();i!=token_list.end();++i)
			{
				if(i->type==__token_number)
				{
					detail.line=i->line;
					detail.str =i->str;
					detail.type=__number;
					detail_token.push_back(detail);
				}
				else if(i->type==__token_string)
				{
					detail.line=i->line;
					detail.str =i->str;
					detail.type=__string;
					detail_token.push_back(detail);
				}
				else if(i->type==__token_reserve_word)
				{
					detail.line=i->line;
					detail.str=i->str;
					if(i->str=="for")
						detail.type=__for;
					else if(i->str=="foreach")
						detail.type=__foreach;
					else if(i->str=="forindex")
						detail.type=__forindex;
					else if(i->str=="while")
						detail.type=__while;
					else if(i->str=="var")
						detail.type=__var;
					else if(i->str=="func")
						detail.type=__func;
					else if(i->str=="break")
						detail.type=__break;
					else if(i->str=="continue")
						detail.type=__continue;
					else if(i->str=="return")
						detail.type=__return;
					else if(i->str=="if")
						detail.type=__if;
					else if(i->str=="else")
						detail.type=__else;
					else if(i->str=="elsif")
						detail.type=__elsif;
					else if(i->str=="nil")
						detail.type=__nil;
					else if(i->str=="and")
						detail.type=__and_operator;
					else if(i->str=="or")
						detail.type=__or_operator;
					detail_token.push_back(detail);
				}
				else if(i->type==__token_identifier)
				{
					detail.line=i->line;
					detail.str=i->str;
					if(i->str.length()<=3)
						detail.type=__id;
					else
					{
						std::string tempstr=i->str;
						int strback=tempstr.length()-1;
						if(tempstr.length()>3 &&tempstr[strback]=='.' && tempstr[strback-1]=='.' && tempstr[strback-2]=='.')
							detail.type=__dynamic_id;
						else
							detail.type=__id;
					}
					detail_token.push_back(detail);
				}
				else if(i->type==__token_operator)
				{
					detail.line=i->line;
					detail.str=i->str;
					if(i->str=="+")
						detail.type=__add_operator;
					else if(i->str=="-")
						detail.type=__sub_operator;
					else if(i->str=="*")
						detail.type=__mul_operator;
					else if(i->str=="/")
						detail.type=__div_operator;
					else if(i->str=="~")
						detail.type=__link_operator;
					else if(i->str=="+=")
						detail.type=__add_equal;
					else if(i->str=="-=")
						detail.type=__sub_equal;
					else if(i->str=="*=")
						detail.type=__mul_equal;
					else if(i->str=="/=")
						detail.type=__div_equal;
					else if(i->str=="~=")
						detail.type=__link_equal;
					else if(i->str=="=")
						detail.type=__equal;
					else if(i->str=="==")
						detail.type=__cmp_equal;
					else if(i->str=="!=")
						detail.type=__cmp_not_equal;
					else if(i->str=="<")
						detail.type=__cmp_less;
					else if(i->str=="<=")
						detail.type=__cmp_less_or_equal;
					else if(i->str==">")
						detail.type=__cmp_more;
					else if(i->str==">=")
						detail.type=__cmp_more_or_equal;
					else if(i->str==";")
						detail.type=__semi;
					else if(i->str==".")
						detail.type=__dot;
					else if(i->str==":")
						detail.type=__colon;
					else if(i->str==",")
						detail.type=__comma;
					else if(i->str=="?")
						detail.type=__ques_mark;
					else if(i->str=="!")
						detail.type=__nor_operator;
					else if(i->str=="[")
						detail.type=__left_bracket;
					else if(i->str=="]")
						detail.type=__right_bracket;
					else if(i->str=="(")
						detail.type=__left_curve;
					else if(i->str==")")
						detail.type=__right_curve;
					else if(i->str=="{")
						detail.type=__left_brace;
					else if(i->str=="}")
						detail.type=__right_brace;
					else
					{
						++error;
						std::cout<<">>[Lexer-error] line "<<detail.line<<": unknown operator \'"<<i->str<<"\'."<<std::endl;
					}
					detail_token.push_back(detail);
				}
			}
			std::cout<<">>[Lexer] complete generating. "<<error<<" error(s)."<<std::endl;
			return;
		}
		int get_error()
		{
			return error;
		}
		std::list<token>& get_detail_token()
		{
			return detail_token;
		}
};

#endif
