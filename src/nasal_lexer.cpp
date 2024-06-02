#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4102)
#endif

#include "nasal_lexer.h"
#include "repl/repl.h"
#include "util/util.h"
#include "util/fs.h"

namespace nasal {

bool lexer::skip(char c) {
    return c==' ' || c=='\n' || c=='\t' || c=='\r' || c==0;
}

bool lexer::is_id(char c) {
    return (c=='_') || std::isalpha(c) || (c<0);   
}

bool lexer::is_hex(char c) {
    return std::isxdigit(c);
}

bool lexer::is_oct(char c) {
    return '0'<=c && c<='7';
}

bool lexer::is_dec(char c) {
    return std::isdigit(c);
}

bool lexer::is_str(char c) {
    return c=='\'' || c=='\"' || c=='`';
}

bool lexer::is_single_opr(char c) {
    return (
        c=='(' || c==')' || c=='[' || c==']' ||
        c=='{' || c=='}' || c==',' || c==';' ||
        c==':' || c=='?' || c=='`' || c=='@' ||
        c=='%' || c=='$' || c=='\\'
    );
}

bool lexer::is_calc_opr(char c) {
    return (
        c=='=' || c=='+' || c=='-' || c=='*' ||
        c=='!' || c=='/' || c=='<' || c=='>' ||
        c=='~' || c=='|' || c=='&' || c=='^'
    );
}

void lexer::skip_note() {
    // avoid note, after this process ptr will point to '\n'
    // so next loop line counter+1
    while(++ptr<res.size() && res[ptr]!='\n') {}
}

void lexer::err_char() {
    ++column;
    char c = res[ptr++];
    err.err("lexer",
        {line, column-1, line, column, filename},
        "invalid character 0x" + util::char_to_hex(c)
    );
    ++invalid_char;
}

void lexer::open(const std::string& file) {
    if (repl::info::instance()->in_repl_mode &&
        repl::info::instance()->repl_file_name==file) {
        err.load(file);
        filename = file;
        res = repl::info::instance()->repl_file_source;
        return;
    }

    if (file.empty()) {
        err.err("lexer", "empty input file");
        err.chkerr();
    }

    // check file exsits and it is a regular file
    if (!fs::is_regular(file)) {
        err.err("lexer", "<"+file+"> is not a regular file");
        err.chkerr();
    }

    // load
    filename = file;
    std::ifstream in(file, std::ios::binary);
    if (in.fail()) {
        err.err("lexer", "failed to open <" + file + ">");
        res = "";
        return;
    }
    err.load(file);
    std::stringstream ss;
    ss << in.rdbuf();
    res = ss.str();
}

tok lexer::get_type(const std::string& str) {
    return token_mapper.count(str)? token_mapper.at(str):tok::tk_null;
}

std::string lexer::utf8_gen() {
    std::string str = "";
    while(ptr<res.size() && res[ptr]<0) {
        std::string tmp = "";
        u32 nbytes = util::utf8_hdchk(res[ptr]);
        if (!nbytes) {
            ++ptr;
            ++column;
            continue;
        }

        tmp += res[ptr++];
        for(u32 i = 0; i<nbytes; ++i, ++ptr) {
            if (ptr<res.size() && (res[ptr]&0xc0)==0x80) {
                tmp += res[ptr];
            }
        }

        // utf8 character's total length is 1+nbytes
        if (tmp.length()!=1+nbytes) {
            ++column;
            std::string utf_info = "0x" + util::char_to_hex(tmp[0]);
            for(u32 i = 1; i<tmp.size(); ++i) {
                utf_info += " 0x" + util::char_to_hex(tmp[i]);
            }
            err.err("lexer",
                {line, column-1, line, column, filename},
                "invalid utf-8 <"+utf_info+">"
            );
            ++invalid_char;
        }
        str += tmp;
        // may have some problems because not all the unicode takes 2 space
        column += 2;
    }
    return str;
}

token lexer::id_gen() {
    u64 begin_line = line;
    u64 begin_column = column;
    std::string str = "";
    while(ptr<res.size() && (is_id(res[ptr]) || is_dec(res[ptr]))) {
        if (res[ptr]<0) { // utf-8
            str += utf8_gen();
        } else { // ascii
            str += res[ptr++];
            ++column;
        }
    }
    tok type = get_type(str);
    return {
        {begin_line, begin_column, line, column, filename},
        (type!=tok::tk_null)? type:tok::tk_id,
        str
    };
}

token lexer::num_gen() {
    u64 begin_line = line;
    u64 begin_column = column;
    // generate hex number
    if (ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='x') {
        std::string str = "0x";
        ptr += 2;
        while(ptr<res.size() && is_hex(res[ptr])) {
            str += res[ptr++];
        }
        column += str.length();
        // "0x"
        if (str.length()<3) {
            err.err("lexer",
                {begin_line, begin_column, line, column, filename},
                "invalid number `"+str+"`"
            );
        }
        return {
            {begin_line, begin_column, line, column, filename},
            tok::tk_num,
            str
        };
    } else if (ptr+1<res.size() && res[ptr]=='0' && res[ptr+1]=='o') { // generate oct number
        std::string str = "0o";
        ptr += 2;
        while(ptr<res.size() && is_oct(res[ptr])) {
            str += res[ptr++];
        }
        bool erfmt = false;
        while(ptr<res.size() && (is_dec(res[ptr]) || is_hex(res[ptr]))) {
            erfmt = true;
            str += res[ptr++];
        }
        column += str.length();
        if (str.length()==2 || erfmt) {
            err.err("lexer",
                {begin_line, begin_column, line, column, filename},
                "invalid number `"+str+"`"
            );
        }
        return {
            {begin_line, begin_column, line, column, filename},
            tok::tk_num,
            str
        };
    }
    // generate dec number
    // dec number -> [0~9][0~9]*(.[0~9]*)(e|E(+|-)0|[1~9][0~9]*)
    std::string str = "";
    while(ptr<res.size() && is_dec(res[ptr])) {
        str += res[ptr++];
    }
    if (ptr<res.size() && res[ptr]=='.') {
        str += res[ptr++];
        while(ptr<res.size() && is_dec(res[ptr])) {
            str += res[ptr++];
        }
        // "xxxx." is not a correct number
        if (str.back()=='.') {
            column += str.length();
            err.err("lexer",
                {begin_line, begin_column, line, column, filename},
                "invalid number `"+str+"`"
            );
            return {
                {begin_line, begin_column, line, column, filename},
                tok::tk_num,
                "0"
            };
        }
    }
    if (ptr<res.size() && (res[ptr]=='e' || res[ptr]=='E')) {
        str += res[ptr++];
        if (ptr<res.size() && (res[ptr]=='-' || res[ptr]=='+')) {
            str += res[ptr++];
        }
        while(ptr<res.size() && is_dec(res[ptr])) {
            str += res[ptr++];
        }
        // "xxxe(-|+)" is not a correct number
        if (str.back()=='e' || str.back()=='E' || str.back()=='-' || str.back()=='+') {
            column += str.length();
            err.err("lexer",
                {begin_line, begin_column, line, column, filename},
                "invalid number `"+str+"`"
            );
            return {
                {begin_line, begin_column, line, column, filename},
                tok::tk_num,
                "0"
            };
        }
    }
    column += str.length();
    return {
        {begin_line, begin_column, line, column, filename},
        tok::tk_num,
        str
    };
}

token lexer::str_gen() {
    u64 begin_line = line;
    u64 begin_column = column;
    std::string str = "";
    const char begin = res[ptr];
    ++column;
    while(++ptr<res.size() && res[ptr]!=begin) {
        ++column;
        if (res[ptr]=='\n') {
            column = 0;
            ++line;
        }
        if (res[ptr]=='\\' && ptr+1<res.size()) {
            ++column;
            ++ptr;
            switch(res[ptr]) {
                case '0': str += '\0';    break;
                case 'a': str += '\a';    break;
                case 'b': str += '\b';    break;
                case 'e': str += '\033';  break;
                case 't': str += '\t';    break;
                case 'n': str += '\n';    break;
                case 'v': str += '\v';    break;
                case 'f': str += '\f';    break;
                case 'r': str += '\r';    break;
                case '?': str += '\?';    break;
                case '\\':str += '\\';    break;
                case '\'':str += '\'';    break;
                case '\"':str += '\"';    break;
                default:  str += res[ptr];break;
            }
            if (res[ptr]=='\n') {
                column = 0;
                ++line;
            }
            continue;
        }
        str += res[ptr];
    }
    // check if this string ends with a " or '
    if (ptr++>=res.size()) {
        err.err("lexer",
            {begin_line, begin_column, line, column, filename},
            "get EOF when generating string"
        );
        return {
            {begin_line, begin_column, line, column, filename},
            tok::tk_str,
            str
        };
    }
    ++column;

    // if is not utf8, 1+utf8_hdchk should be 1
    if (begin=='`' && str.length()!=1+util::utf8_hdchk(str[0])) {
        err.err("lexer",
            {begin_line, begin_column, line, column, filename},
            "\'`\' is used for string including one character"
        );
    }
    return {
        {begin_line, begin_column, line, column, filename},
        tok::tk_str,
        str
    };
}

token lexer::single_opr() {
    u64 begin_line = line;
    u64 begin_column = column;
    std::string str(1, res[ptr]);
    ++column;
    tok type = get_type(str);
    if (type==tok::tk_null) {
        err.err("lexer",
            {begin_line, begin_column, line, column, filename},
            "invalid operator `"+str+"`"
        );
    }
    ++ptr;
    return {{begin_line, begin_column, line, column, filename}, type, str};
}

token lexer::dots() {
    u64 begin_line = line;
    u64 begin_column = column;
    std::string str = ".";
    if (ptr+2<res.size() && res[ptr+1]=='.' && res[ptr+2]=='.') {
        str += "..";
    }
    ptr += str.length();
    column += str.length();
    return {{begin_line, begin_column, line, column, filename}, get_type(str), str};
}

token lexer::calc_opr() {
    u64 begin_line = line;
    u64 begin_column = column;
    // get calculation operator
    std::string str(1, res[ptr++]);
    if (ptr<res.size() && res[ptr]=='=') {
        str += res[ptr++];
    }
    column += str.length();
    return {{begin_line, begin_column, line, column, filename}, get_type(str), str};
}

const error& lexer::scan(const std::string& file) {
    line = 1;
    column = 0;
    ptr = 0;
    toks = {};
    open(file);

    while(ptr<res.size()) {
        while(ptr<res.size() && skip(res[ptr])) {
            // these characters will be ignored, and '\n' will cause ++line
            ++column;
            if (res[ptr++]=='\n') {
                ++line;
                column = 0;
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
        if (invalid_char>10) {
            err.err("lexer", "too many invalid characters, stop");
            break;
        }
    }
    if (toks.size()) {
        // eof token's location is the last token's location
        toks.push_back({toks.back().loc, tok::tk_eof, "<eof>"});
    } else {
        // if token sequence is empty, generate a default location
        toks.push_back({
            {line, column, line, column, filename},
            tok::tk_eof,
            "<eof>"
        });
    }
    res = "";
    return err;
}

}
