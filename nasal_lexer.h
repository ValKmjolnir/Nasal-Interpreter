#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#define ID(c)      ((c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z'))
#define HEX(c)     (('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c && c<='F'))
#define OCT(c)     ('0'<=c&&c<='7')
#define DIGIT(c)   ('0'<=c&&c<='9')
#define STR(c)     (c=='\''||c=='\"'||c=='`')
// single operators have only one character
#define SINGLE_OPERATOR(c) (c=='('||c==')'||c=='['||c==']'||c=='{'||c=='}'||c==','||c==';'||c=='|'||c==':'||\
                               c=='?'||c=='`'||c=='&'||c=='@'||c=='%'||c=='$'||c=='^'||c=='\\')
// calculation operators may have two chars, for example: += -= *= /= ~= != == >= <=
#define CALC_OPERATOR(c)   (c=='='||c=='+'||c=='-'||c=='*'||c=='!'||c=='/'||c=='<'||c=='>'||c=='~')
#define NOTE(c)            (c=='#')

enum token_type
{
    tok_null=0,// null token default token type
    tok_num,   // number     basic token type
    tok_str,   // string     basic token type
    tok_id,    // identifier basic token type
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
    tok_eof    // end of token list
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
    std::string filename;
    std::string code;
    std::string res;
    std::vector<token> tokens;

    uint32_t get_type(const std::string&);
    void die(const char*);
    void open(const std::string&);
    std::string id_gen();
    std::string num_gen();
    std::string str_gen();
public:
    uint32_t err(){return error;}
    void scan(const std::string&);
    void print();
    const std::vector<token>& get_tokens() const {return tokens;}
};

void nasal_lexer::open(const std::string& file)
{
    filename=file;
    std::ifstream fin(file,std::ios::binary);
    if(fin.fail())
    {
        ++error;
        std::cout<<"[lexer] cannot open file <"<<file<<">.\n";
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
    std::cout<<"[lexer] "<<filename<<":"<<line<<":"<<code.length()<<"\n"<<code<<'\n';
    for(int i=0;i<(int)code.size()-1;++i)
        std::cout<<char(" \t"[code[i]=='\t']);
    std::cout<<'^'<<info<<'\n';
}

std::string nasal_lexer::id_gen()
{
    std::string str="";
    while(ptr<res.size() && (ID(res[ptr])||DIGIT(res[ptr])))
        str+=res[ptr++];
    code+=str;
    return str;
}

std::string nasal_lexer::num_gen()
{
    // generate hex number
    if(ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='x')
    {
        std::string str="0x";
        ptr+=2;
        while(ptr<res.size() && HEX(res[ptr]))
            str+=res[ptr++];
        code+=str;
        if(str.length()<3)// "0x"
            die("invalid number.");
        return str;
    }
    // generate oct number
    else if(ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='o')
    {
        std::string str="0o";
        ptr+=2;
        while(ptr<res.size() && OCT(res[ptr]))
            str+=res[ptr++];
        code+=str;
        if(str.length()<3)// "0o"
            die("invalid number.");
        return str;
    }
    // generate dec number
    // dec number -> [0~9][0~9]*(.[0~9]*)(e|E(+|-)0|[1~9][0~9]*)
    std::string str="";
    while(ptr<res.size() && DIGIT(res[ptr]))
        str+=res[ptr++];
    if(ptr<res.size() && res[ptr]=='.')
    {
        str+=res[ptr++];
        while(ptr<res.size() && DIGIT(res[ptr]))
            str+=res[ptr++];
        // "xxxx." is not a correct number
        if(str.back()=='.')
        {
            code+=str;
            die("invalid number.");
            return "0";
        }
    }
    if(ptr<res.size() && (res[ptr]=='e' || res[ptr]=='E'))
    {
        str+=res[ptr++];
        if(ptr<res.size() && (res[ptr]=='-' || res[ptr]=='+'))
            str+=res[ptr++];
        while(ptr<res.size() && DIGIT(res[ptr]))
            str+=res[ptr++];
        // "xxxe(-|+)" is not a correct number
        if(str.back()=='e' || str.back()=='E' || str.back()=='-' || str.back()=='+')
        {
            code+=str;
            die("invalid number.");
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
    while(++ptr<res.size() && res[ptr]!=begin)
    {
        code+=res[ptr];
        if(res[ptr]=='\n')
        {
            code="";
            ++line;
        }
        if(res[ptr]=='\\' && ptr+1<res.size())
        {
            code+=res[++ptr];
            switch(res[ptr])
            {
                case '0': str+='\0';    break;
                case 'a': str+='\a';    break;
                case 'b': str+='\b';    break;
                case 't': str+='\t';    break;
                case 'n': str+='\n';    break;
                case 'v': str+='\v';    break;
                case 'f': str+='\f';    break;
                case 'r': str+='\r';    break;
                case '?': str+='\?';    break;
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
    if(ptr++>=res.size())
    {
        die("get EOF when generating string.");
        return str;
    }
    code+=res[ptr-1];
    if(begin=='`' && str.length()!=1)
        die("\'`\' is used for string that includes one character.");
    return str;
}

void nasal_lexer::scan(const std::string& file)
{
    line=1;
    error=ptr=0;
    open(file);

    std::string str;
    while(ptr<res.size())
    {
        while(ptr<res.size() && (res[ptr]==' ' || res[ptr]=='\n' || res[ptr]=='\t' || res[ptr]=='\r' || res[ptr]<0))
        {
            // these characters will be ignored, and '\n' will cause ++line
            code+=res[ptr];
            if(res[ptr++]=='\n')
            {
                ++line;
                code="";
            }
        }
        if(ptr>=res.size()) break;
        if(ID(res[ptr]))
        {
            str=id_gen();
            uint32_t type=get_type(str);
            tokens.push_back({line,type?type:tok_id,str});
        }
        else if(DIGIT(res[ptr]))
            tokens.push_back({line,tok_num,num_gen()});
        else if(STR(res[ptr]))
            tokens.push_back({line,tok_str,str_gen()});
        else if(SINGLE_OPERATOR(res[ptr]))
        {
            str=res[ptr];
            code+=res[ptr];
            uint32_t type=get_type(str);
            if(!type)
                die("invalid operator.");
            tokens.push_back({line,type,str});
            ++ptr;
        }
        else if(res[ptr]=='.')
        {
            str=".";
            if(ptr+2<res.size() && res[ptr+1]=='.' && res[ptr+2]=='.')
                str+="..";
            ptr+=str.length();
            code+=str;
            tokens.push_back({line,get_type(str),str});
        }
        else if(CALC_OPERATOR(res[ptr]))
        {
            // get calculation operator
            str=res[ptr++];
            if(ptr<res.size() && res[ptr]=='=')
                str+=res[ptr++];
            code+=str;
            tokens.push_back({line,get_type(str),str});
        }
        else if(NOTE(res[ptr]))// avoid note, after this process ptr will point to a '\n', so next loop line counter+1
            while(++ptr<res.size() && res[ptr]!='\n');
        else
        {
            code+=res[ptr++];
            die("unknown character.");
        }
    }
    tokens.push_back({line,tok_eof,"eof"});
    code=res="";
}

void nasal_lexer::print()
{
    for(auto& tok:tokens)
        std::cout<<"("<<tok.line<<" | "<<rawstr(tok.str)<<")\n";
}

#endif