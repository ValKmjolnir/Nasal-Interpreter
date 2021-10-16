#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#define IS_ID(c)      ((c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z'))
#define IS_HEX(c)     (('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c && c<='F'))
#define IS_OCT(c)     ('0'<=c&&c<='7')
#define IS_DIGIT(c)   ('0'<=c&&c<='9')
#define IS_STR(c)     (c=='\''||c=='\"'||c=='`')
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
    const uint32_t tok_type;
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
    {nullptr   ,0            }
};

struct token
{
    uint32_t line;
    uint32_t type;
    std::string str;
    token(uint32_t l=0,uint32_t t=tok_null,std::string s=""){line=l;type=t;str=s;}
};

class nasal_lexer
{
private:
    uint32_t    error;
    uint32_t    line;
    uint32_t    ptr;
    size_t      size;
    std::string code;
    std::string res;
    std::vector<token> tokens;
    uint32_t get_type(const std::string&);
    void die(const char*);
    std::string id_gen();
    std::string num_gen();
    std::string str_gen();
public:
    void open(const std::string&);
    void scan();
    void print();
    uint32_t err(){return error;}
    const std::vector<token>& get_tokens(){return tokens;}
};

void nasal_lexer::open(const std::string& filename)
{
    error=0;
    res.clear();
    std::ifstream fin(filename,std::ios::binary);
    if(fin.fail())
    {
        ++error;
        std::cout<<"[lexer] cannot open file <"<<filename<<">.\n";
    }
    std::stringstream ss;
    ss<<fin.rdbuf();
    res=ss.str();
}

uint32_t nasal_lexer::get_type(const std::string& tk_str)
{
    for(int i=0;token_table[i].str;++i)
        if(tk_str==token_table[i].str)
            return token_table[i].tok_type;
    return tok_null;
}

void nasal_lexer::die(const char* info)
{
    ++error;
    std::cout<<"[lexer] line "<<line<<" column "<<code.length()<<": \n"<<code<<'\n';
    for(auto i:code)
        std::cout<<char(" \t"[i=='\t']);
    std::cout<<'^'<<info<<'\n';
}

std::string nasal_lexer::id_gen()
{
    std::string str="";
    while(ptr<size && (IS_ID(res[ptr])||IS_DIGIT(res[ptr])))
        str+=res[ptr++];
    code+=str;
    return str;
    // after running this process, ptr will point to the next token's beginning character
}

std::string nasal_lexer::num_gen()
{
    // generate hex number
    if(ptr+1<size && res[ptr]=='0' && res[ptr+1]=='x')
    {
        std::string str="0x";
        ptr+=2;
        while(ptr<size && IS_HEX(res[ptr]))
            str+=res[ptr++];
        code+=str;
        if(str.length()<3)// "0x"
            die("incorrect number.");
        return str;
    }
    // generate oct number
    else if(ptr+1<size && res[ptr]=='0' && res[ptr+1]=='o')
    {
        std::string str="0o";
        ptr+=2;
        while(ptr<size && IS_OCT(res[ptr]))
            str+=res[ptr++];
        code+=str;
        if(str.length()<3)// "0o"
            die("incorrect number.");
        return str;
    }
    // generate dec number
    // dec number -> [0~9][0~9]*(.[0~9]*)(e|E(+|-)0|[1~9][0~9]*)
    std::string str="";
    while(ptr<size && IS_DIGIT(res[ptr]))
        str+=res[ptr++];
    if(ptr<size && res[ptr]=='.')
    {
        str+=res[ptr++];
        while(ptr<size && IS_DIGIT(res[ptr]))
            str+=res[ptr++];
        // "xxxx." is not a correct number
        if(str.back()=='.')
        {
            code+=str;
            die("incorrect number.");
            return "0";
        }
    }
    if(ptr<size && (res[ptr]=='e' || res[ptr]=='E'))
    {
        str+=res[ptr++];
        if(ptr<size && (res[ptr]=='-' || res[ptr]=='+'))
            str+=res[ptr++];
        while(ptr<size && IS_DIGIT(res[ptr]))
            str+=res[ptr++];
        // "xxxe(-|+)" is not a correct number
        if(str.back()=='e' || str.back()=='E' || str.back()=='-' || str.back()=='+')
        {
            code+=str;
            die("incorrect number.");
            return "0";
        }
    }
    code+=str;
    return str;
}

std::string nasal_lexer::str_gen()
{
    std::string str="";
    char begin=res[ptr];
    code+=begin;
    while(++ptr<size && res[ptr]!=begin)
    {
        code+=res[ptr];
        if(res[ptr]=='\n')
        {
            code="";
            ++line;
        }
        if(res[ptr]=='\\' && ptr+1<size)
        {
            code+=res[++ptr];
            switch(res[ptr])
            {
                case 'a': str+='\a';    break;
                case 'b': str+='\b';    break;
                case 'f': str+='\f';    break;
                case 'n': str+='\n';    break;
                case 'r': str+='\r';    break;
                case 't': str+='\t';    break;
                case 'v': str+='\v';    break;
                case '?': str+='\?';    break;
                case '0': str+='\0';    break;
                case '\\':str+='\\';    break;
                case '\'':str+='\'';    break;
                case '\"':str+='\"';    break;
                default:  str+=res[ptr];break;
            }
            continue;
        }
        str+=res[ptr];
    }
    // check if this string ends with a " or '
    if(ptr++>=size)
        die("get EOF when generating string.");
    code+=res[ptr-1];
    if(begin=='`' && str.length()!=1)
        die("\'`\' is used for string that includes one character.");
    return str;
}

void nasal_lexer::scan()
{
    tokens.clear();
    line=1;
    ptr=0;
    code="";
    size=res.size();

    std::string str;
    while(ptr<size)
    {
        while(ptr<size && (res[ptr]==' ' || res[ptr]=='\n' || res[ptr]=='\t' || res[ptr]=='\r' || res[ptr]<0))
        {
            // these characters will be ignored, and '\n' will cause ++line
            code+=res[ptr];
            if(res[ptr++]=='\n')
            {
                ++line;
                code="";
            }
        }
        if(ptr>=size) break;
        if(IS_ID(res[ptr]))
        {
            str=id_gen();
            tokens.push_back({line,get_type(str),str});
            if(!tokens.back().type)
                tokens.back().type=tok_id;
        }
        else if(IS_DIGIT(res[ptr]))
            tokens.push_back({line,tok_num,num_gen()});
        else if(IS_STR(res[ptr]))
            tokens.push_back({line,tok_str,str_gen()});
        else if(IS_SINGLE_OPERATOR(res[ptr]))
        {
            str=res[ptr];
            code+=res[ptr];
            uint32_t type=get_type(str);
            if(!type)
                die("incorrect operator.");
            tokens.push_back({line,type,str});
            ++ptr;
        }
        else if(res[ptr]=='.')
        {
            if(ptr+2<size && res[ptr+1]=='.' && res[ptr+2]=='.')
            {
                str="...";
                ptr+=3;
            }
            else
            {
                str=".";
                ++ptr;
            }
            code+=str;
            tokens.push_back({line,get_type(str),str});
        }
        else if(IS_CALC_OPERATOR(res[ptr]))
        {
            // get calculation operator
            str=res[ptr++];
            if(ptr<size && res[ptr]=='=')
                str+=res[ptr++];
            code+=str;
            tokens.push_back({line,get_type(str),str});
        }
        else if(IS_NOTE(res[ptr]))// avoid note, after this process ptr will point to a '\n', so next loop line counter+1
            while(++ptr<size && res[ptr]!='\n');
        else
        {
            code+=res[ptr++];
            die("unknown character.");
        }
    }
    tokens.push_back({line,tok_eof,""});
    res.clear();
    return;
}

void nasal_lexer::print()
{
    for(auto& tok:tokens)
        std::cout<<"("<<tok.line<<" | "<<tok.str<<")\n";
}

#endif