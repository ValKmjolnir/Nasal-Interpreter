#ifndef __BALLOON_LEXER_H__
#define __BALLOON_LEXER_H__

std::string reserve_word[15]=
{
	"for","foreach","forindex","while",
	"var","func","break","continue","return",
	"if","else","elsif","nil","and","or"
};
int is_reserve_word(std::string str)
{
	for(int i=0;i<15;++i)
		if(reserve_word[i]==str)
			return __reserve_word;
	return __token_identifier;
}



class resource_file
{
	private:
		std::list<char> resource;
	public:
		void set_clear()
		{
			resource.clear();
			return;
		}
		void input_file(std::string filename)
		{
			std::ifstream fin(filename,std::ios::binary);
			char c;
			while(!fin.eof())
			{
				c=fin.get(); 
				if(fin.eof())
					break;
				resource.push_back(c);
			}
			resource.push_back('\n');
			return;
		}
		std::list<char>& get_resource()
		{
			return resource;
		}
		void print_file()
		{
			int line=1;
			std::cout<<line<<"	";
			for(std::list<char>::iterator i=resource.begin();i!=resource.end();++i)
			{
				std::cout<<*i;
				if(*i=='\n')
				{
					++line;
					std::cout<<line<<"	";
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

class balloon_lexer
{
	private:
		int error;
		std::list<token> token_list;
		std::list<token> detail_token;
	public:
		void set_clear()
		{
			token_list.clear();
			detail_token.clear();
			return;
		}
		void print_token_list()
		{
			for(std::list<token>::iterator i=token_list.begin();i!=token_list.end();++i)
			{
				std::cout<<"( ";
				print_token(i->type);
				std::cout<<" | "<<i->str<<" )"<<std::endl;
			}
			return;
		}
		void print_detail_token_list()
		{
			for(std::list<token>::iterator i=detail_token.begin();i!=detail_token.end();++i)
			{
				std::cout<<"( ";
				print_detail_token(i->type);
				std::cout<<" | "<<i->str<<" )"<<std::endl;
			}
			return;
		}
		void scanner(std::list<char>& res)
		{
			token_list.clear();
			error=0;
			
			int line=1;
			std::string token_str;
			std::list<char>::iterator ptr=res.begin();
			while(1)
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
					token_str="";
					while(*ptr=='_' || ('a'<=*ptr && *ptr<='z') || ('A'<=*ptr && *ptr<='Z') || ('0'<=*ptr && *ptr<='9'))
					{
						token_str+=*ptr;
						++ptr;
						if(ptr==res.end())
							break;
					}
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
					while(('0'<=*ptr && *ptr<='9') || *ptr=='.' || *ptr=='x' || *ptr=='o')
					{
						token_str+=*ptr;
						++ptr;
						if(ptr==res.end())
							break;
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
					if(ptr==res.end() || *ptr!=str_begin)
					{
						++error;
						std::cout<<">>[Lexer-error] line "<<line<<": this string must have a \' "<<str_begin<<" \' as its end."<<std::endl;
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
					while(ptr!=res.end() && *ptr!='\n')++ptr;
					if(ptr==res.end())
						break;
				}
			}
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
					detail.str=i->str;
					detail.type=__number;
					detail_token.push_back(detail);
				}
				else if(i->type==__token_string)
				{
					detail.line=i->line;
					detail.str=i->str;
					detail.type=__string;
					detail_token.push_back(detail);
				}
				else if(i->type==__reserve_word)
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
						if(tempstr[strback]=='.' && tempstr[strback-1]=='.' && tempstr[strback-2]=='.')
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
						std::cout<<">>[Lexer-error] line "<<detail.line<<": unknown operator."<<std::endl;
					}
					detail_token.push_back(detail);
				}
			}
			return;
		}
		std::list<token>& get_detail_token()
		{
			return detail_token;
		}
};
#endif
