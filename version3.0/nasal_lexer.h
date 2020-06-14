#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#define IS_IDENTIFIER_HEAD(c) (c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')
#define IS_IDENTIFIER_BODY(c) (c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')||('0'<=c&&c<='9')
#define IS_NUMBER_HEAD(c)     ('0'<=c&&c<='9')
#define IS_NUMBER_BODY(c)     ('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c&&c<='F')||(c=='e'||c=='E'||c=='.'||c=='x'||c=='o')
#define IS_STRING_HEAD(c)     (c=='\''||c=='\"')
// single operators have only one character
#define IS_SINGLE_OPRATOR(c)  (c=='('||c==')'||c=='['||c==']'||c=='{'||c=='}'||c==','||c==';'||c=='|'||c==':'||\
							   c=='?'||c=='`'||c=='&'||c=='@'||c=='%'||c=='$'||c=='^'||c=='\\')
#define IS_DOT(c)             (c=='.')
// calculation operators may have two chars, for example: += -= *= /= ~= != == >= <=
#define IS_CALC_OPERATOR(c)   (c=='='||c=='+'||c=='-'||c=='*'||c=='!'||c=='/'||c=='<'||c=='>'||c=='~')
#define IS_NOTE_HEAD(c)       (c=='#')

#ifndef TOKEN_TABLE_SIZE
#define TOKEN_TABLE_SIZE 44
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
    std::vector<token> token_list;
	std::string identifier_gen(std::vector<char>&,int&,int&);
    std::string number_gen(std::vector<char>&,int&,int&);
    std::string string_gen(std::vector<char>&,int&,int&);
public:
    void clear();
    void scanner(std::vector<char>&);
    void print_token();
    int  get_error();
	std::vector<token>& get_token_list();
};

void nasal_lexer::clear()
{
    token_list.clear();
    return;
}

std::string nasal_lexer::identifier_gen(std::vector<char>& res,int& ptr,int& line)
{
	int res_size=res.size();
	std::string token_str="";
	while(ptr<res_size && IS_IDENTIFIER_BODY(res[ptr]))
		token_str+=res[ptr++];
	return token_str;
	// after running this process, ptr will point to the next token's beginning character
}

std::string nasal_lexer::number_gen(std::vector<char>& res,int& ptr,int& line)
{
	int res_size=res.size();
	bool scientific_notation=false;// numbers like 1e8 are scientific_notation
	std::string token_str="";
	while(ptr<res_size && IS_NUMBER_BODY(res[ptr]))
	{
		token_str+=res[ptr];
		if(res[ptr]=='e' || res[ptr]=='E')
		{
			scientific_notation=true;
			++ptr;
			break;
		}
		++ptr;
	}
	if(scientific_notation && ptr<res_size)
	{
		if(res[ptr]=='-')
		{
			token_str+='-';
			++ptr;
		}
		while(ptr<res_size && '0'<=res[ptr] && res[ptr]<='9')
		{
			token_str+=res[ptr];
			++ptr;
		}
	}
	if(!check_numerable_string(token_str))
	{
		++error;
		std::cout<<">> [lexer] line "<<line<<": \'"<<token_str<<"\' is not a numerable string."<<std::endl;
		token_str="0";
	}
	return token_str;
}

std::string nasal_lexer::string_gen(std::vector<char>& res,int& ptr,int& line)
{
	int res_size=res.size();
	std::string token_str="";
	char str_begin=res[ptr];
	++ptr;
	if(ptr>=res_size) return token_str;
	while(ptr<res_size && res[ptr]!=str_begin)
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
	}
	// check if this string ends with a " or '
	if(ptr>=res_size)
	{
		++error;
		std::cout<<">> [lexer] line "<<line<<": this string must have a \' "<<str_begin<<" \' as its end."<<std::endl;
	}
	++ptr;
	return token_str;
}

void nasal_lexer::scanner(std::vector<char>& res)
{
    error=0;
    token_list.clear();
    int line=1,ptr=0,res_size=res.size();
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
			token_str=identifier_gen(res,ptr,line);
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
		else if(IS_NUMBER_HEAD(res[ptr]))
		{
			token_str=number_gen(res,ptr,line);
			token new_token;
			new_token.line=line;
            new_token.str=token_str;
			new_token.type=tok_number;
			token_list.push_back(new_token);
		}
		else if(IS_STRING_HEAD(res[ptr]))
		{
			token_str=string_gen(res,ptr,line);
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
        else if(IS_DOT(res[ptr]))
        {
            if(ptr+2<res_size && IS_DOT(res[ptr+1]) && IS_DOT(res[ptr+2]))
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
			while(ptr<res_size && res[ptr++]!='\n');
			// after this process ptr will point to a '\n'
			// don't ++ptr then the counter for line can work correctly
		}
		else
		{
			++error;
			std::cout<<">> [lexer] line "<<line<<": unknown char."<<std::endl;
			++ptr;
		}
	}
	std::cout<<">> [lexer] complete scanning. "<<error<<" error(s)."<<std::endl;
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