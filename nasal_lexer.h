#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#include <sstream>
#include <sys/stat.h>

#ifdef _MSC_VER
#define S_ISREG(m) (((m)&0xF000)==0x8000)
#endif

#define ID(c)      ((c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')||(c<0))
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

enum tok:u32{
    tok_null=0,  // null token (default token type)
    tok_num,     // number literal
    tok_str,     // string literal
    tok_id,      // identifier
    tok_for,     // loop keyword for
    tok_forindex,// loop keyword forindex
    tok_foreach, // loop keyword foreach
    tok_while,   // loop keyword while
    tok_var,     // keyword for definition
    tok_func,    // keyword for definition of function
    tok_break,   // loop keyword break
    tok_continue,// loop keyword continue
    tok_ret,     // function keyword return
    tok_if,      // condition expression keyword if
    tok_elsif,   // condition expression keyword elsif
    tok_else,    // condition expression keyword else
    tok_nil,     // nil literal
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

struct{
    const char* str;
    const u32 type;
}tok_table[]={
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
    u32 line;
    u32 col;
    u32 type;
    string str;
    token(u32 l=0,u32 c=0,u32 t=tok_null,const string& s=""):str(s)
    {
        line=l;
        col=c;
        type=t;
    }
};

class lexer
{
private:
    u32    line;
    u32    column;
    usize  ptr;
    string res;
    error& err;
    std::vector<token> tokens;

    u32 get_type(const string&);
    void die(const string& info){err.err("lexer",line,column,info);}
    void open(const string&);
    string utf8_gen();
    string id_gen();
    string num_gen();
    string str_gen();
public:
    lexer(error& e):
        line(1),column(0),
        ptr(0),res(""),
        err(e){}
    void scan(const string&);
    void print();
    const std::vector<token>& result() const {return tokens;}
};

void lexer::open(const string& file)
{
    struct stat buffer;
    if(stat(file.c_str(),&buffer)==0 && !S_ISREG(buffer.st_mode))
    {
        err.err("lexer","<"+file+"> is not a regular file");
        err.chkerr();
    }
    std::ifstream fin(file,std::ios::binary);
    if(fin.fail())
        err.err("lexer","failed to open <"+file+">");
    else
        err.load(file);
    std::stringstream ss;
    ss<<fin.rdbuf();
    res=ss.str();
}

u32 lexer::get_type(const string& str)
{
    for(u32 i=0;tok_table[i].str;++i)
        if(str==tok_table[i].str)
            return tok_table[i].type;
    return tok_null;
}

string lexer::utf8_gen()
{
    string str="";
    while(ptr<res.size() && res[ptr]<0)
    {
        string tmp="";
        u32 nbytes=utf8_hdchk(res[ptr]);
        if(nbytes)
        {
            tmp+=res[ptr++];
            for(u32 i=0;i<nbytes;++i,++ptr)
                if(ptr<res.size() && (res[ptr]&0xc0)==0x80)
                    tmp+=res[ptr];
            if(tmp.length()!=1+nbytes)
            {
                ++column;
                string utf_info="0x"+chrhex(tmp[0]);
                for(u32 i=1;i<tmp.size();++i)
                    utf_info+=" 0x"+chrhex(tmp[i]);
                die("invalid utf-8 character `"+utf_info+"`, make sure it is utf8-text file");
                std::exit(1);
            }
            str+=tmp;
            column+=2; // may have some problems because not all the unicode takes 2 space
        }
        else
        {
            ++ptr;
            ++column;
        }
    }
    return str;
}

string lexer::id_gen()
{
    string str="";
    while(ptr<res.size() && (ID(res[ptr])||DIGIT(res[ptr])))
    {
        if(res[ptr]<0) // utf-8
            str+=utf8_gen();
        else // ascii
        {
            str+=res[ptr++];
            ++column;
        }
    }
    return str;
}

string lexer::num_gen()
{
    // generate hex number
    if(ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='x')
    {
        string str="0x";
        ptr+=2;
        while(ptr<res.size() && HEX(res[ptr]))
            str+=res[ptr++];
        column+=str.length();
        if(str.length()<3)// "0x"
            die("invalid number `"+str+"`");
        return str;
    }
    // generate oct number
    else if(ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='o')
    {
        string str="0o";
        ptr+=2;
        while(ptr<res.size() && OCT(res[ptr]))
            str+=res[ptr++];
        column+=str.length();
        if(str.length()<3)// "0o"
            die("invalid number `"+str+"`");
        return str;
    }
    // generate dec number
    // dec number -> [0~9][0~9]*(.[0~9]*)(e|E(+|-)0|[1~9][0~9]*)
    string str="";
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
            column+=str.length();
            die("invalid number `"+str+"`");
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
            column+=str.length();
            die("invalid number `"+str+"`");
            return "0";
        }
    }
    column+=str.length();
    return str;
}

string lexer::str_gen()
{
    string str="";
    const char begin=res[ptr];
    ++column;
    while(++ptr<res.size() && res[ptr]!=begin)
    {
        ++column;
        if(res[ptr]=='\n')
        {
            column=0;
            ++line;
        }
        if(res[ptr]=='\\' && ptr+1<res.size())
        {
            ++column;
            ++ptr;
            switch(res[ptr])
            {
                case '0': str+='\0';    break;
                case 'a': str+='\a';    break;
                case 'b': str+='\b';    break;
                case 'e': str+='\033';  break;
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
        die("get EOF when generating string");
        return str;
    }
    ++column;
    if(begin=='`' && str.length()!=1)
        die("\'`\' is used for string that includes one character");
    return str;
}

void lexer::scan(const string& file)
{
    line=1;
    column=0;
    ptr=0;
    open(file);

    string str;
    while(ptr<res.size())
    {
        while(ptr<res.size() && (res[ptr]==' ' || res[ptr]=='\n' || res[ptr]=='\t' || res[ptr]=='\r' || res[ptr]==0))
        {
            // these characters will be ignored, and '\n' will cause ++line
            ++column;
            if(res[ptr++]=='\n')
            {
                ++line;
                column=0;
            }
        }
        if(ptr>=res.size()) break;
        if(ID(res[ptr]))
        {
            str=id_gen();
            u32 type=get_type(str);
            tokens.push_back({line,column,type?type:tok_id,str});
        }
        else if(DIGIT(res[ptr]))
        {
            str=num_gen(); // make sure column is correct
            tokens.push_back({line,column,tok_num,str});
        }
        else if(STR(res[ptr]))
        {
            str=str_gen(); // make sure column is correct
            tokens.push_back({line,column,tok_str,str});
        }
        else if(SINGLE_OPERATOR(res[ptr]))
        {
            str=res[ptr];
            ++column;
            u32 type=get_type(str);
            if(!type)
                die("invalid operator `"+str+"`");
            tokens.push_back({line,column,type,str});
            ++ptr;
        }
        else if(res[ptr]=='.')
        {
            str=".";
            if(ptr+2<res.size() && res[ptr+1]=='.' && res[ptr+2]=='.')
                str+="..";
            ptr+=str.length();
            column+=str.length();
            tokens.push_back({line,column,get_type(str),str});
        }
        else if(CALC_OPERATOR(res[ptr]))
        {
            // get calculation operator
            str=res[ptr++];
            if(ptr<res.size() && res[ptr]=='=')
                str+=res[ptr++];
            column+=str.length();
            tokens.push_back({line,column,get_type(str),str});
        }
        else if(NOTE(res[ptr]))// avoid note, after this process ptr will point to a '\n', so next loop line counter+1
            while(++ptr<res.size() && res[ptr]!='\n');
        else
        {
            ++column;
            char c=res[ptr++];
            die("invalid character 0x"+chrhex(c));
        }
    }
    tokens.push_back({line,column,tok_eof,"eof"});
    res="";
    err.chkerr();
}

void lexer::print()
{
    for(auto& tok:tokens)
        std::cout<<"("<<tok.line<<" | "<<rawstr(tok.str,128)<<")\n";
}

#endif