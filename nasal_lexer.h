#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#define IS_IDENTIFIER_HEAD(c) ((c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z'))
#define IS_IDENTIFIER_BODY(c) ((c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')||('0'<=c&&c<='9'))
#define IS_HEX_NUMBER(c)      (('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c && c<='F'))
#define IS_OCT_NUMEBR(c)      ('0'<=c&&c<='7')
#define IS_DIGIT(c)           ('0'<=c&&c<='9')
#define IS_STRING_HEAD(c)     (c=='\''||c=='\"')
// single operators have only one character
#define IS_SINGLE_OPRATOR(c)  (c=='('||c==')'||c=='['||c==']'||c=='{'||c=='}'||c==','||c==';'||c=='|'||c==':'||\
							   c=='?'||c=='`'||c=='&'||c=='@'||c=='%'||c=='$'||c=='^'||c=='\\')
// calculation operators may have two chars, for example: += -= *= /= ~= != == >= <=
#define IS_CALC_OPERATOR(c)   (c=='='||c=='+'||c=='-'||c=='*'||c=='!'||c=='/'||c=='<'||c=='>'||c=='~')
#define IS_NOTE_HEAD(c)       (c=='#')

#ifndef TOKEN_TABLE_SIZE
#define TOKEN_TABLE_SIZE 45
struct token_table
{
    std::string str;
    int tok_type;
}tok_tbl[TOKEN_TABLE_SIZE]=
{
    {"for"     ,tok_for          },
    {"forindex",tok_forindex     },
    {"foreach" ,tok_foreach      },
    {"while"   ,tok_while        },
    {"var"     ,tok_var          },
    {"func"    ,tok_func         },
    {"break"   ,tok_break        },
    {"continue",tok_continue     },
    {"return"  ,tok_return       },
    {"if"      ,tok_if           },
    {"elsif"   ,tok_elsif        },
    {"else"    ,tok_else         },
    {"nil"     ,tok_nil          },
    {"("       ,tok_left_curve   },
    {")"       ,tok_right_curve  },
    {"["       ,tok_left_bracket },
    {"]"       ,tok_right_bracket},
    {"{"       ,tok_left_brace   },
    {"}"       ,tok_right_brace  },
    {";"       ,tok_semi         },
    {"and"     ,tok_and          },
    {"or"      ,tok_or           },
    {","       ,tok_comma        },
    {"."       ,tok_dot          },
    {"..."     ,tok_ellipsis     },
    {"?"       ,tok_quesmark     },
    {":"       ,tok_colon        },
    {"+"       ,tok_add          },
    {"-"       ,tok_sub          },
    {"*"       ,tok_mult         },
    {"/"       ,tok_div          },
    {"~"       ,tok_link         },
    {"!"       ,tok_not          },
	{"="       ,tok_equal        },
    {"+="      ,tok_add_equal    },
    {"-="      ,tok_sub_equal    },
    {"*="      ,tok_mult_equal   },
    {"/="      ,tok_div_equal    },
    {"~="      ,tok_link_equal   },
    {"=="      ,tok_cmp_equal    },
    {"!="      ,tok_cmp_not_equal},
    {"<"       ,tok_less_than    },
    {">"       ,tok_greater_than },
    {"<="      ,tok_less_equal   },
    {">="      ,tok_greater_equal},
};
#endif

struct token
{
    int line;
    int type;
    std::string str;
};

class nasal_lexer
{
private:
    int error;
	int res_size;
	int line;
	int ptr;
	std::vector<char> res;
    std::vector<token> token_list;
	std::string identifier_gen();
	void generate_number_error(int,std::string);
    std::string number_gen();
    std::string string_gen();
public:
    void clear();
	void openfile(std::string);
    void scanner();
    void print_token();
    int  get_error();
	std::vector<token>& get_token_list();
};

void nasal_lexer::clear()
{
	error=0;
	res_size=0;
	line=0;
	ptr=0;
	res.clear();
    token_list.clear();
    return;
}

void nasal_lexer::openfile(std::string filename)
{
	error=0;
	res.clear();
    std::ifstream fin(filename,std::ios::binary);
    if(fin.fail())
    {
		++error;
        std::cout<<">> [lexer] cannot open file \""<<filename<<"\".\n";
        fin.close();
        return;
    }
    while(!fin.eof())
    {
        char c=fin.get();
        if(fin.eof())
            break;
        res.push_back(c);
    }
    fin.close();
	res_size=res.size();
    return;
}

std::string nasal_lexer::identifier_gen()
{
	std::string token_str="";
	while(ptr<res_size && IS_IDENTIFIER_BODY(res[ptr]))
		token_str+=res[ptr++];
	return token_str;
	// after running this process, ptr will point to the next token's beginning character
}

void nasal_lexer::generate_number_error(int line,std::string token_str)
{
	++error;
	std::cout<<">> [lexer] line "<<line<<": \""<<token_str<<"\" is not a correct number.\n";
	return;
}
std::string nasal_lexer::number_gen()
{
	bool scientific_notation=false;// numbers like 1e8 are scientific_notation
	std::string token_str="";
	// generate hex number
	if(res[ptr]=='0' && ptr+1<res_size && res[ptr+1]=='x')
	{
		token_str="0x";
		ptr+=2;
		while(ptr<res_size && IS_HEX_NUMBER(res[ptr]))
			token_str+=res[ptr++];
		if(token_str=="0x")
		{
			generate_number_error(line,token_str);
			return "0";
		}
		return token_str;
	}
	// generate oct number
	else if(res[ptr]=='0' && ptr+1<res_size && res[ptr+1]=='o')
	{
		token_str="0o";
		ptr+=2;
		while(ptr<res_size && IS_OCT_NUMEBR(res[ptr]))
			token_str+=res[ptr++];
		if(token_str=="0o")
		{
			generate_number_error(line,token_str);
			return "0";
		}
		return token_str;
	}
	// generate dec number
	// dec number -> 0|[1~9][0~9]*(.[0~9]*)(e|E(+|-)0|[1~9][0~9]*)
	if(ptr<res_size && res[ptr]=='0')
		token_str+=res[ptr++];
	while(ptr<res_size && IS_DIGIT(res[ptr]))
		token_str+=res[ptr++];
	if(ptr<res_size && res[ptr]=='.')
	{
		token_str+=res[ptr++];
		// "xxxx." is not a correct number
		if(ptr>=res_size)
		{
			generate_number_error(line,token_str);
			return "0";
		}
		while(ptr<res_size && IS_DIGIT(res[ptr]))
			token_str+=res[ptr++];
		// "xxxx." is not a correct number
		if(token_str.back()=='.')
		{
			generate_number_error(line,token_str);
			return "0";
		}
	}
	if(ptr<res_size && (res[ptr]=='e' || res[ptr]=='E'))
	{
		token_str+=res[ptr++];
		// "xxxe" is not a correct number
		if(ptr>=res_size)
		{
			generate_number_error(line,token_str);
			return "0";
		}
		if(ptr<res_size && (res[ptr]=='-' || res[ptr]=='+'))
			token_str+=res[ptr++];
		if(ptr>=res_size)
		{
			generate_number_error(line,token_str);
			return "0";
		}
		if(ptr<res_size && res[ptr]=='0')
			token_str+=res[ptr++];
		while(ptr<res_size && IS_DIGIT(res[ptr]))
			token_str+=res[ptr++];
		// "xxxe(-|+)" is not a correct number
		if(token_str.back()=='e' || token_str.back()=='E' || token_str.back()=='-' || token_str.back()=='+')
		{
			generate_number_error(line,token_str);
			return "0";
		}
	}
	return token_str;
}

std::string nasal_lexer::string_gen()
{
	std::string token_str="";
	char str_begin=res[ptr++];
	if(ptr>=res_size) return token_str;
	while(ptr<res_size && res[ptr]!=str_begin)
	{
		if(res[ptr]=='\n') ++line;
		if(res[ptr]=='\\' && ptr+1<res.size())
		{
			++ptr;
			switch(res[ptr])
			{
				case 'a':token_str.push_back('\a');break;
				case 'b':token_str.push_back('\b');break;
				case 'f':token_str.push_back('\f');break;
				case 'n':token_str.push_back('\n');break;
				case 'r':token_str.push_back('\r');break;
				case 't':token_str.push_back('\t');break;
				case 'v':token_str.push_back('\v');break;
				case '?':token_str.push_back('\?');break;
				case '0':token_str.push_back('\0');break;
				case '\\':token_str.push_back('\\');break;
				case '\'':token_str.push_back('\'');break;
				case '\"':token_str.push_back('\"');break;
				default:  token_str.push_back(res[ptr]);break;
			}
		}
		else
			token_str+=res[ptr];
		++ptr;
	}
	// check if this string ends with a " or '
	if(ptr>=res_size)
	{
		++error;
		std::cout<<">> [lexer] line "<<line<<": get EOF when generating string.\n";
	}
	++ptr;
	return token_str;
}

void nasal_lexer::scanner()
{
    token_list.clear();
    line=1;
	ptr=0;

	std::string token_str;
	while(ptr<res_size)
	{
		while(ptr<res_size && (res[ptr]==' ' || res[ptr]=='\n' || res[ptr]=='\t' || res[ptr]=='\r' || res[ptr]<0))
		{
			// these characters will be ignored, and '\n' will cause ++line
			if(res[ptr]=='\n') ++line;
			++ptr;
		}
		if(ptr>=res_size) break;
		if(IS_IDENTIFIER_HEAD(res[ptr]))
		{
			token_str=identifier_gen();
			token new_token;
			new_token.line=line;
            new_token.str=token_str;
            new_token.type=0;
            for(int i=0;i<TOKEN_TABLE_SIZE;++i)
                if(token_str==tok_tbl[i].str)
                {
                    new_token.type=tok_tbl[i].tok_type;
                    break;
                }
            if(!new_token.type)
                new_token.type=tok_identifier;
			token_list.push_back(new_token);
		}
		else if(IS_DIGIT(res[ptr]))
		{
			token_str=number_gen();
			token new_token;
			new_token.line=line;
            new_token.str=token_str;
			new_token.type=tok_number;
			token_list.push_back(new_token);
		}
		else if(IS_STRING_HEAD(res[ptr]))
		{
			token_str=string_gen();
			token new_token;
			new_token.line=line;
			new_token.type=tok_string;
			new_token.str=token_str;
			token_list.push_back(new_token);
		}
		else if(IS_SINGLE_OPRATOR(res[ptr]))
		{
			token_str="";
			token_str+=res[ptr];
			token new_token;
			new_token.line=line;
            new_token.str=token_str;
            for(int i=0;i<TOKEN_TABLE_SIZE;++i)
                if(token_str==tok_tbl[i].str)
                {
                    new_token.type=tok_tbl[i].tok_type;
                    break;
                }
			token_list.push_back(new_token);
			++ptr;
		}
        else if(res[ptr]=='.')
        {
            if(ptr+2<res_size && res[ptr+1]=='.' && res[ptr+2]=='.')
            {
                token_str="...";
                ptr+=3;
            }
            else
            {
                token_str=".";
                ++ptr;
            }
            token new_token;
            new_token.line=line;
            new_token.str=token_str;
            for(int i=0;i<TOKEN_TABLE_SIZE;++i)
                if(token_str==tok_tbl[i].str)
                {
                    new_token.type=tok_tbl[i].tok_type;
                    break;
                }
			token_list.push_back(new_token);
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
			new_token.str=token_str;
            for(int i=0;i<TOKEN_TABLE_SIZE;++i)
                if(token_str==tok_tbl[i].str)
                {
                    new_token.type=tok_tbl[i].tok_type;
                    break;
                }
			token_list.push_back(new_token);
		}
		else if(IS_NOTE_HEAD(res[ptr]))
		{
			// avoid note
			while(ptr<res_size && res[ptr]!='\n') ++ptr;
			// after this process ptr will point to a '\n'
			// don't ++ptr then the counter for line can work correctly
		}
		else
		{
			++error;
			std::cout<<">> [lexer] line "<<line<<": unknown char "<<(int)res[ptr]<<'.'<<std::endl;
			++ptr;
		}
	}
	return;
}

void nasal_lexer::print_token()
{
    int size=token_list.size();
    for(int i=0;i<size;++i)
        std::cout<<"("<<token_list[i].line<<" | "<<token_list[i].str<<")\n";
    return;
}

int nasal_lexer::get_error()
{
    return error;
}

std::vector<token>& nasal_lexer::get_token_list()
{
	return token_list;
}
#endif