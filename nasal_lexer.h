#pragma once

#include <cstring>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>

#include "nasal.h"
#include "nasal_err.h"

#ifdef _MSC_VER
#define S_ISREG(m) (((m)&0xF000)==0x8000)
#endif

enum class tok:u32 {
    null=0,   // null token (default token type)
    num,      // number literal
    str,      // string literal
    id,       // identifier
    tktrue,   // keyword true
    tkfalse,  // keyword false
    rfor,     // loop keyword for
    forindex, // loop keyword forindex
    foreach,  // loop keyword foreach
    rwhile,   // loop keyword while
    var,      // keyword for definition
    func,     // keyword for definition of function
    brk,      // loop keyword break
    cont,     // loop keyword continue
    ret,      // function keyword return
    rif,      // condition expression keyword if
    elsif,    // condition expression keyword elsif
    relse,    // condition expression keyword else
    tknil,    // nil literal
    lcurve,   // (
    rcurve,   // )
    lbracket, // [
    rbracket, // ]
    lbrace,   // {
    rbrace,   // }
    semi,     // ;
    opand,    // operator and
    opor,     // operator or
    comma,    // ,
    dot,      // .
    ellipsis, // ...
    quesmark, // ?
    colon,    // :
    add,      // operator +
    sub,      // operator -
    mult,     // operator *
    div,      // operator /
    floater,  // operator ~ and binary operator ~
    btand,    // bitwise operator &
    btor,     // bitwise operator |
    btxor,    // bitwise operator ^
    opnot,    // operator !
    eq,       // operator =
    addeq,    // operator +=
    subeq,    // operator -=
    multeq,   // operator *=
    diveq,    // operator /=
    lnkeq,    // operator ~=
    btandeq,  // operator &=
    btoreq,   // operator |=
    btxoreq,  // operator ^=
    cmpeq,    // operator ==
    neq,      // operator !=
    less,     // operator <
    leq,      // operator <=
    grt,      // operator >
    geq,      // operator >=
    eof       // <eof> end of token list
};

struct token {
    span loc;    // location
    tok type;    // token type
    string str;  // content
    token() = default;
    token(const token&) = default;
};

class lexer {
private:
    u32    line;
    u32    column;
    usize  ptr;
    string filename;
    string res;
    error& err;
    std::vector<token> toks;
    const std::unordered_map<string,tok> typetbl {
        {"true"    ,tok::tktrue  },
        {"false"   ,tok::tkfalse },
        {"for"     ,tok::rfor    },
        {"forindex",tok::forindex},
        {"foreach" ,tok::foreach },
        {"while"   ,tok::rwhile  },
        {"var"     ,tok::var     },
        {"func"    ,tok::func    },
        {"break"   ,tok::brk     },
        {"continue",tok::cont    },
        {"return"  ,tok::ret     },
        {"if"      ,tok::rif     },
        {"elsif"   ,tok::elsif   },
        {"else"    ,tok::relse   },
        {"nil"     ,tok::tknil   },
        {"("       ,tok::lcurve  },
        {")"       ,tok::rcurve  },
        {"["       ,tok::lbracket},
        {"]"       ,tok::rbracket},
        {"{"       ,tok::lbrace  },
        {"}"       ,tok::rbrace  },
        {";"       ,tok::semi    },
        {"and"     ,tok::opand   },
        {"or"      ,tok::opor    },
        {","       ,tok::comma   },
        {"."       ,tok::dot     },
        {"..."     ,tok::ellipsis},
        {"?"       ,tok::quesmark},
        {":"       ,tok::colon   },
        {"+"       ,tok::add     },
        {"-"       ,tok::sub     },
        {"*"       ,tok::mult    },
        {"/"       ,tok::div     },
        {"~"       ,tok::floater },
        {"&"       ,tok::btand   },
        {"|"       ,tok::btor    },
        {"^"       ,tok::btxor   },
        {"!"       ,tok::opnot   },
        {"="       ,tok::eq      },
        {"+="      ,tok::addeq   },
        {"-="      ,tok::subeq   },
        {"*="      ,tok::multeq  },
        {"/="      ,tok::diveq   },
        {"~="      ,tok::lnkeq   },
        {"&="      ,tok::btandeq },
        {"|="      ,tok::btoreq  },
        {"^="      ,tok::btxoreq },
        {"=="      ,tok::cmpeq   },
        {"!="      ,tok::neq     },
        {"<"       ,tok::less    },
        {"<="      ,tok::leq     },
        {">"       ,tok::grt     },
        {">="      ,tok::geq     }
    };

    tok get_type(const string&);
    bool skip(char);
    bool is_id(char);
    bool is_hex(char);
    bool is_oct(char);
    bool is_dec(char);
    bool is_str(char);
    bool is_single_opr(char);
    bool is_calc_opr(char);

    void skip_note();
    void err_char();

    void open(const string&);
    string utf8_gen();
    token id_gen();
    token num_gen();
    token str_gen();
    token single_opr();
    token dots();
    token calc_opr();
public:
    lexer(error& e): line(1), column(0), ptr(0), filename(""), res(""), err(e) {}
    const error& scan(const string&);
    const std::vector<token>& result() const {return toks;}
};

bool lexer::skip(char c) {
    return c==' '||c=='\n'||c=='\t'||c=='\r'||c==0;
}

bool lexer::is_id(char c) {
    return (c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')||(c<0);   
}

bool lexer::is_hex(char c) {
    return ('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c && c<='F');
}

bool lexer::is_oct(char c) {
    return '0'<=c&&c<='7';
}

bool lexer::is_dec(char c) {
    return '0'<=c&&c<='9';
}

bool lexer::is_str(char c) {
    return c=='\''||c=='\"'||c=='`';
}

bool lexer::is_single_opr(char c) {
    return (
        c=='('||c==')'||c=='['||c==']'||
        c=='{'||c=='}'||c==','||c==';'||
        c==':'||c=='?'||c=='`'||c=='@'||
        c=='%'||c=='$'||c=='\\'
    );
}

bool lexer::is_calc_opr(char c) {
    return (
        c=='='||c=='+'||c=='-'||c=='*'||
        c=='!'||c=='/'||c=='<'||c=='>'||
        c=='~'||c=='|'||c=='&'||c=='^'
    );
}

void lexer::skip_note() {
    // avoid note, after this process ptr will point to a '\n', so next loop line counter+1
    while(++ptr<res.size() && res[ptr]!='\n') {}
}

void lexer::err_char() {
    ++column;
    char c=res[ptr++];
    err.err("lexer", {line, column-1, line, column, filename}, "invalid character 0x"+chrhex(c));
    err.fatal("lexer", "fatal error occurred, stop");
}

void lexer::open(const string& file) {
    // check file exsits and it is a regular file
    struct stat buffer;
    if (stat(file.c_str(), &buffer)==0 && !S_ISREG(buffer.st_mode)) {
        err.err("lexer", "<"+file+"> is not a regular file");
        err.chkerr();
    }

    // load
    filename=file;
    std::ifstream in(file, std::ios::binary);
    if (in.fail()) {
        err.err("lexer", "failed to open <"+file+">");
    } else {
        err.load(file);
    }
    std::stringstream ss;
    ss<<in.rdbuf();
    res=ss.str();
}

tok lexer::get_type(const string& str) {
    return typetbl.count(str)?typetbl.at(str):tok::null;
}

string lexer::utf8_gen() {
    string str="";
    while(ptr<res.size() && res[ptr]<0) {
        string tmp="";
        u32 nbytes=utf8_hdchk(res[ptr]);
        if (!nbytes) {
            ++ptr;
            ++column;
            continue;
        }

        tmp+=res[ptr++];
        for(u32 i=0;i<nbytes;++i,++ptr) {
            if (ptr<res.size() && (res[ptr]&0xc0)==0x80) {
                tmp+=res[ptr];
            }
        }

        // utf8 character's total length is 1+nbytes
        if (tmp.length()!=1+nbytes) {
            ++column;
            string utf_info="0x"+chrhex(tmp[0]);
            for(u32 i=1;i<tmp.size();++i) {
                utf_info+=" 0x"+chrhex(tmp[i]);
            }
            err.err("lexer", {line, column-1, line, column, filename}, "invalid utf-8 <"+utf_info+">");
            err.fatal("lexer", "fatal error occurred, stop");
        }
        str+=tmp;
        column+=2; // may have some problems because not all the unicode takes 2 space
    }
    return str;
}

token lexer::id_gen() {
    u32 begin_line=line;
    u32 begin_column=column;
    string str="";
    while(ptr<res.size() && (is_id(res[ptr])||is_dec(res[ptr]))) {
        if (res[ptr]<0) { // utf-8
            str+=utf8_gen();
        } else { // ascii
            str+=res[ptr++];
            ++column;
        }
    }
    tok type=get_type(str);
    return {{begin_line, begin_column, line, column, filename}, (type!=tok::null)?type:tok::id, str};
}

token lexer::num_gen() {
    u32 begin_line=line;
    u32 begin_column=column;
    // generate hex number
    if (ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='x') {
        string str="0x";
        ptr+=2;
        while(ptr<res.size() && is_hex(res[ptr])) {
            str+=res[ptr++];
        }
        column+=str.length();
        if (str.length()<3) { // "0x"
            err.err("lexer", {begin_line, begin_column, line, column, filename}, "invalid number `"+str+"`");
        }
        return {{begin_line, begin_column, line, column, filename}, tok::num, str};
    } else if (ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='o') { // generate oct number
        string str="0o";
        ptr+=2;
        while(ptr<res.size() && is_oct(res[ptr])) {
            str+=res[ptr++];
        }
        bool erfmt=false;
        while(ptr<res.size() && (is_dec(res[ptr]) || is_hex(res[ptr]))) {
            erfmt=true;
            str+=res[ptr++];
        }
        column+=str.length();
        if (str.length()==2 || erfmt) {
            err.err("lexer", {begin_line, begin_column, line, column, filename}, "invalid number `"+str+"`");
        }
        return {{begin_line, begin_column, line, column, filename}, tok::num, str};
    }
    // generate dec number
    // dec number -> [0~9][0~9]*(.[0~9]*)(e|E(+|-)0|[1~9][0~9]*)
    string str="";
    while(ptr<res.size() && is_dec(res[ptr])) {
        str+=res[ptr++];
    }
    if (ptr<res.size() && res[ptr]=='.') {
        str+=res[ptr++];
        while(ptr<res.size() && is_dec(res[ptr])) {
            str+=res[ptr++];
        }
        // "xxxx." is not a correct number
        if (str.back()=='.') {
            column+=str.length();
            err.err("lexer",{begin_line, begin_column, line, column, filename}, "invalid number `"+str+"`");
            return {{begin_line, begin_column, line, column, filename}, tok::num, "0"};
        }
    }
    if (ptr<res.size() && (res[ptr]=='e' || res[ptr]=='E')) {
        str+=res[ptr++];
        if (ptr<res.size() && (res[ptr]=='-' || res[ptr]=='+')) {
            str+=res[ptr++];
        }
        while(ptr<res.size() && is_dec(res[ptr])) {
            str+=res[ptr++];
        }
        // "xxxe(-|+)" is not a correct number
        if (str.back()=='e' || str.back()=='E' || str.back()=='-' || str.back()=='+') {
            column+=str.length();
            err.err("lexer",{begin_line, begin_column, line, column, filename}, "invalid number `"+str+"`");
            return {{begin_line, begin_column, line, column, filename}, tok::num, "0"};
        }
    }
    column+=str.length();
    return {{begin_line, begin_column, line, column, filename}, tok::num, str};
}

token lexer::str_gen() {
    u32 begin_line=line;
    u32 begin_column=column;
    string str="";
    const char begin=res[ptr];
    ++column;
    while(++ptr<res.size() && res[ptr]!=begin) {
        ++column;
        if (res[ptr]=='\n') {
            column=0;
            ++line;
        }
        if (res[ptr]=='\\' && ptr+1<res.size()) {
            ++column;
            ++ptr;
            switch(res[ptr]) {
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
            if (res[ptr]=='\n') {
                column=0;
                ++line;
            }
            continue;
        }
        str+=res[ptr];
    }
    // check if this string ends with a " or '
    if (ptr++>=res.size()) {
        err.err("lexer", {begin_line, begin_column, line, column, filename}, "get EOF when generating string");
        return {{begin_line, begin_column, line, column, filename}, tok::str, str};
    }
    ++column;
    if (begin=='`' && str.length()!=1) {
        err.err("lexer", {begin_line, begin_column, line, column, filename}, "\'`\' is used for string including one character");
    }
    return {{begin_line, begin_column, line, column, filename}, tok::str, str};
}

token lexer::single_opr() {
    u32 begin_line=line;
    u32 begin_column=column;
    string str(1,res[ptr]);
    ++column;
    tok type=get_type(str);
    if (type==tok::null) {
        err.err("lexer", {begin_line, begin_column, line, column, filename}, "invalid operator `"+str+"`");
    }
    ++ptr;
    return {{begin_line, begin_column, line, column, filename}, type, str};
}

token lexer::dots() {
    u32 begin_line=line;
    u32 begin_column=column;
    string str=".";
    if (ptr+2<res.size() && res[ptr+1]=='.' && res[ptr+2]=='.') {
        str+="..";
    }
    ptr+=str.length();
    column+=str.length();
    return {{begin_line, begin_column, line, column, filename}, get_type(str), str};
}

token lexer::calc_opr() {
    u32 begin_line=line;
    u32 begin_column=column;
    // get calculation operator
    string str(1,res[ptr++]);
    if (ptr<res.size() && res[ptr]=='=') {
        str+=res[ptr++];
    }
    column+=str.length();
    return {{begin_line, begin_column, line, column, filename}, get_type(str), str};
}

const error& lexer::scan(const string& file) {
    line=1;
    column=0;
    ptr=0;
    open(file);

    while(ptr<res.size()) {
        while(ptr<res.size() && skip(res[ptr])) {
            // these characters will be ignored, and '\n' will cause ++line
            ++column;
            if (res[ptr++]=='\n') {
                ++line;
                column=0;
            }
        }
        if (ptr>=res.size()) {
            break;
        }
        if (is_id(res[ptr])) {
            toks.push_back(id_gen());
        } else if (is_dec(res[ptr])) {
            toks.push_back(num_gen());
        } else if (is_str(res[ptr])) {
            toks.push_back(str_gen());
        } else if (is_single_opr(res[ptr])) {
            toks.push_back(single_opr());
        } else if (res[ptr]=='.') {
            toks.push_back(dots());
        } else if (is_calc_opr(res[ptr])) {
            toks.push_back(calc_opr());
        } else if (res[ptr]=='#') {
            skip_note();
        } else {
            err_char();
        }
    }
    toks.push_back({{line, column, line, column, filename}, tok::eof, "<eof>"});
    res="";
    return err;
}
