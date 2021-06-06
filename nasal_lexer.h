#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#define IS_IDENTIFIER(c)      ((c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z'))
#define IS_HEX_NUMBER(c)      (('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c && c<='F'))
#define IS_OCT_NUMEBR(c)      ('0'<=c&&c<='7')
#define IS_DIGIT(c)           ('0'<=c&&c<='9')
#define IS_STRING(c)          (c=='\''||c=='\"'||c=='`')
// single operators have only one character
#define IS_SINGLE_OPERATOR(c) (c=='('||c==')'||c=='['||c==']'||c=='{'||c=='}'||c==','||c==';'||c=='|'||c==':'||\
							   c=='?'||c=='`'||c=='&'||c=='@'||c=='%'||c=='$'||c=='^'||c=='\\')
// calculation operators may have two chars, for example: += -= *= /= ~= != == >= <=
#define IS_CALC_OPERATOR(c)   (c=='='||c=='+'||c=='-'||c=='*'||c=='!'||c=='/'||c=='<'||c=='>'||c=='~')
#define IS_NOTE(c)            (c=='#')

enum token_type
{
    tok_null=0,
    tok_num,tok_str,tok_id,
    tok_for,tok_forindex,tok_foreach,tok_while,
    tok_var,tok_func,tok_break,tok_continue,
    tok_ret,tok_if,tok_elsif,tok_else,tok_nil,
    tok_lcurve,tok_rcurve,
    tok_lbracket,tok_rbracket,
    tok_lbrace,tok_rbrace,
    tok_semi,tok_and,tok_or,tok_comma,tok_dot,tok_ellipsis,tok_quesmark,
    tok_colon,tok_add,tok_sub,tok_mult,tok_div,tok_link,tok_not,
    tok_eq,
    tok_addeq,tok_subeq,tok_multeq,tok_diveq,tok_lnkeq,
    tok_cmpeq,tok_neq,tok_less,tok_leq,tok_grt,tok_geq,
	tok_eof
};

struct
{
    const char* str;
    int tok_type;
}token_table[]=
{
    {"for"     ,tok_for      },
    {"forindex",tok_forindex },
    {"foreach" ,tok_foreach  },
    {"while"   ,tok_while    },
    {"var"     ,tok_var      },
    {"func"    ,tok_func     },
    {"break"   ,tok_break    },
    {"continue",tok_continue },
    {"return"  ,tok_ret      },
    {"if"      ,tok_if       },
    {"elsif"   ,tok_elsif    },
    {"else"    ,tok_else     },
    {"nil"     ,tok_nil      },
    {"("       ,tok_lcurve   },
    {")"       ,tok_rcurve   },
    {"["       ,tok_lbracket },
    {"]"       ,tok_rbracket },
    {"{"       ,tok_lbrace   },
    {"}"       ,tok_rbrace   },
    {";"       ,tok_semi     },
    {"and"     ,tok_and      },
    {"or"      ,tok_or       },
    {","       ,tok_comma    },
    {"."       ,tok_dot      },
    {"..."     ,tok_ellipsis },
    {"?"       ,tok_quesmark },
    {":"       ,tok_colon    },
    {"+"       ,tok_add      },
    {"-"       ,tok_sub      },
    {"*"       ,tok_mult     },
    {"/"       ,tok_div      },
    {"~"       ,tok_link     },
    {"!"       ,tok_not      },
	{"="       ,tok_eq       },
    {"+="      ,tok_addeq    },
    {"-="      ,tok_subeq    },
    {"*="      ,tok_multeq   },
    {"/="      ,tok_diveq    },
    {"~="      ,tok_lnkeq    },
    {"=="      ,tok_cmpeq    },
    {"!="      ,tok_neq      },
    {"<"       ,tok_less     },
	{"<="      ,tok_leq      },
    {">"       ,tok_grt      },
    {">="      ,tok_geq      },
	{NULL      ,-1           }
};

struct token
{
    int line;
    int type;
    std::string str;
	token(int l=0,int t=tok_null,std::string s=""){line=l;type=t;str=s;return;}
};

class nasal_lexer
{
private:
    int error;
	int res_size;
	int line;
	int ptr;
	std::string line_code;
	std::vector<char> res;
    std::vector<token> token_list;
	int  get_tok_type(std::string);
	void die(std::string,std::string,int,int);
	std::string id_gen();
    std::string num_gen();
    std::string str_gen();
public:
	void openfile(std::string);
    void scanner();
    void print_token();
    int  get_error();
	std::vector<token>& get_token_list();
};

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
		res_size=0;
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

int nasal_lexer::get_tok_type(std::string tk_str)
{
	for(int i=0;token_table[i].str;++i)
		if(tk_str==token_table[i].str)
			return token_table[i].tok_type;
	return tok_null;
}

void nasal_lexer::die(std::string code,std::string error_info,int line=-1,int column=-1)
{
	++error;
	std::cout<<">> [lexer] line "<<line<<" column "<<column<<": \n"<<code<<"\n";
	for(int i=0;i<column-1;++i)
		std::cout<<(code[i]=='\t'?'\t':' ');
	std::cout<<"^ "<<error_info<<'\n';
	return;
}

std::string nasal_lexer::id_gen()
{
	std::string token_str="";
	while(ptr<res_size && (IS_IDENTIFIER(res[ptr])||IS_DIGIT(res[ptr])))
		token_str+=res[ptr++];
	line_code+=token_str;
	return token_str;
	// after running this process, ptr will point to the next token's beginning character
}

std::string nasal_lexer::num_gen()
{
	bool scientific_notation=false;// numbers like 1e8 are scientific_notation
	std::string token_str="";
	// generate hex number
	if(ptr+1<res_size && res[ptr]=='0' && res[ptr+1]=='x')
	{
		token_str="0x";
		ptr+=2;
		while(ptr<res_size && IS_HEX_NUMBER(res[ptr]))
			token_str+=res[ptr++];
		line_code+=token_str;
		if(token_str=="0x")
		{
			die(line_code,"incorrect number.",line,line_code.length());
			return "0";
		}
		return token_str;
	}
	// generate oct number
	else if(ptr+1<res_size && res[ptr]=='0' && res[ptr+1]=='o')
	{
		token_str="0o";
		ptr+=2;
		while(ptr<res_size && IS_OCT_NUMEBR(res[ptr]))
			token_str+=res[ptr++];
		line_code+=token_str;
		if(token_str=="0o")
		{
			die(line_code,"incorrect number.",line,line_code.length());
			return "0";
		}
		return token_str;
	}
	// generate dec number
	// dec number -> [0~9][0~9]*(.[0~9]*)(e|E(+|-)0|[1~9][0~9]*)
	while(ptr<res_size && IS_DIGIT(res[ptr]))
		token_str+=res[ptr++];
	if(ptr<res_size && res[ptr]=='.')
	{
		token_str+=res[ptr++];
		// "xxxx." is not a correct number
		if(ptr>=res_size)
		{
			line_code+=token_str;
			die(line_code,"incorrect number.",line,line_code.length());
			return "0";
		}
		while(ptr<res_size && IS_DIGIT(res[ptr]))
			token_str+=res[ptr++];
		// "xxxx." is not a correct number
		if(token_str.back()=='.')
		{
			line_code+=token_str;
			die(line_code,"incorrect number.",line,line_code.length());
			return "0";
		}
	}
	if(ptr<res_size && (res[ptr]=='e' || res[ptr]=='E'))
	{
		token_str+=res[ptr++];
		// "xxxe" is not a correct number
		if(ptr>=res_size)
		{
			line_code+=token_str;
			die(line_code,"incorrect number.",line,line_code.length());
			return "0";
		}
		if(ptr<res_size && (res[ptr]=='-' || res[ptr]=='+'))
			token_str+=res[ptr++];
		if(ptr>=res_size)
		{
			line_code+=token_str;
			die(line_code,"incorrect number.",line,line_code.length());
			return "0";
		}
		while(ptr<res_size && IS_DIGIT(res[ptr]))
			token_str+=res[ptr++];
		// "xxxe(-|+)" is not a correct number
		if(token_str.back()=='e' || token_str.back()=='E' || token_str.back()=='-' || token_str.back()=='+')
		{
			line_code+=token_str;
			die(line_code,"incorrect number.",line,line_code.length());
			return "0";
		}
	}
	line_code+=token_str;
	return token_str;
}

std::string nasal_lexer::str_gen()
{
	std::string token_str="";
	char str_begin=res[ptr];
	line_code+=str_begin;
	while(++ptr<res_size && res[ptr]!=str_begin)
	{
		line_code+=res[ptr];
		if(res[ptr]=='\n')
		{
			line_code="";
			++line;
		}
		if(res[ptr]=='\\' && ptr+1<res_size)
		{
			line_code+=res[++ptr];
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
			continue;
		}
		token_str+=res[ptr];
	}
	// check if this string ends with a " or '
	if(ptr++>=res_size)
		die(line_code,"get EOF when generating string.",line,line_code.length());
	if(str_begin=='`' && token_str.length()>1)
		die(line_code,"\'`\' is used for string that includes one character.",line,line_code.length());
	return token_str;
}

void nasal_lexer::scanner()
{
    token_list.clear();
    line=1;
	ptr=0;
	line_code="";

	std::string token_str;
	while(ptr<res_size)
	{
		while(ptr<res_size && (res[ptr]==' ' || res[ptr]=='\n' || res[ptr]=='\t' || res[ptr]=='\r' || res[ptr]<0))
		{
			// these characters will be ignored, and '\n' will cause ++line
			line_code+=res[ptr];
			if(res[ptr++]=='\n')
			{
				++line;
				line_code="";
			}
		}
		if(ptr>=res_size) break;
		if(IS_IDENTIFIER(res[ptr]))
		{
			token_str=id_gen();
			token new_token(line,get_tok_type(token_str),token_str);
            if(!new_token.type)
                new_token.type=tok_id;
			token_list.push_back(new_token);
		}
		else if(IS_DIGIT(res[ptr]))
		{
			token_str=num_gen();
			token new_token(line,tok_num,token_str);
			token_list.push_back(new_token);
		}
		else if(IS_STRING(res[ptr]))
		{
			token_str=str_gen();
			token new_token(line,tok_str,token_str);
			token_list.push_back(new_token);
		}
		else if(IS_SINGLE_OPERATOR(res[ptr]))
		{
			token_str="";
			token_str+=res[ptr];
			line_code+=res[ptr];
			token new_token(line,get_tok_type(token_str),token_str);
			if(!new_token.type)
				die(line_code,"incorrect operator.",line,line_code.length());
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
			line_code+=token_str;
            token new_token(line,get_tok_type(token_str),token_str);
			token_list.push_back(new_token);
        }
		else if(IS_CALC_OPERATOR(res[ptr]))
		{
			// get calculation operator
			token_str=res[ptr++];
			if(ptr<res_size && res[ptr]=='=')
				token_str+=res[ptr++];
			line_code+=token_str;
			token new_token(line,get_tok_type(token_str),token_str);
			token_list.push_back(new_token);
		}
		else if(IS_NOTE(res[ptr]))// avoid note, after this process ptr will point to a '\n', so next loop line counter+1
			while(++ptr<res_size && res[ptr]!='\n');
		else
		{
			line_code+=res[ptr++];
			die(line_code,"unknown character.",line,line_code.length());
		}
	}
	token tk(line,tok_eof,"");
	token_list.push_back(tk);
	res.clear();
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