#pragma once

#ifdef _MSC_VER
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4102)
#endif

#include <cstring>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "nasal.h"
#include "nasal_err.h"

namespace nasal {

enum class tok {
    tk_null = 0, // null token (default token type)
    tk_num,      // number literal
    tk_str,      // string literal
    tk_id,       // identifier
    tk_true,     // keyword true
    tk_false,    // keyword false
    tk_use,      // keyword use
    tk_for,      // loop keyword for
    tk_forindex, // loop keyword forindex
    tk_foreach,  // loop keyword foreach
    tk_while,    // loop keyword while
    tk_var,      // keyword for definition
    tk_func,     // keyword for definition of function
    tk_brk,      // loop keyword break
    tk_cont,     // loop keyword continue
    tk_ret,      // function keyword return
    tk_if,       // condition expression keyword if
    tk_elsif,    // condition expression keyword elsif
    tk_else,     // condition expression keyword else
    tk_nil,      // nil literal
    tk_lcurve,   // (
    tk_rcurve,   // )
    tk_lbracket, // [
    tk_rbracket, // ]
    tk_lbrace,   // {
    tk_rbrace,   // }
    tk_semi,     // ;
    tk_and,      // operator and
    tk_or,       // operator or
    tk_comma,    // ,
    tk_dot,      // .
    tk_ellipsis, // ...
    tk_quesmark, // ?
    tk_quesques, // ??
    tk_quesdot,  // ?.
    tk_colon,    // :
    tk_add,      // operator +
    tk_sub,      // operator -
    tk_mult,     // operator *
    tk_div,      // operator /
    tk_floater,  // operator ~ and binary operator ~
    tk_btand,    // bitwise operator &
    tk_btor,     // bitwise operator |
    tk_btxor,    // bitwise operator ^
    tk_not,      // operator !
    tk_eq,       // operator =
    tk_addeq,    // operator +=
    tk_subeq,    // operator -=
    tk_multeq,   // operator *=
    tk_diveq,    // operator /=
    tk_lnkeq,    // operator ~=
    tk_btandeq,  // operator &=
    tk_btoreq,   // operator |=
    tk_btxoreq,  // operator ^=
    tk_cmpeq,    // operator ==
    tk_neq,      // operator !=
    tk_less,     // operator <
    tk_leq,      // operator <=
    tk_grt,      // operator >
    tk_geq,      // operator >=
    tk_eof       // <eof> end of token list
};

struct token {
    span loc;        // location
    tok type;        // token type
    std::string str; // content

    token() = default;
    token(const token&) = default;
};

class lexer {
private:
    u64 line;
    u64 column;
    usize ptr;
    std::string filename;
    std::string res;

private:
    error err;
    u64 invalid_char;
    std::vector<token> toks;

private:
    const std::unordered_map<std::string, tok> token_mapper = {
        {"use"     , tok::tk_use     },
        {"true"    , tok::tk_true    },
        {"false"   , tok::tk_false   },
        {"for"     , tok::tk_for     },
        {"forindex", tok::tk_forindex},
        {"foreach" , tok::tk_foreach },
        {"while"   , tok::tk_while   },
        {"var"     , tok::tk_var     },
        {"func"    , tok::tk_func    },
        {"break"   , tok::tk_brk     },
        {"continue", tok::tk_cont    },
        {"return"  , tok::tk_ret     },
        {"if"      , tok::tk_if      },
        {"elsif"   , tok::tk_elsif   },
        {"else"    , tok::tk_else    },
        {"nil"     , tok::tk_nil     },
        {"("       , tok::tk_lcurve  },
        {")"       , tok::tk_rcurve  },
        {"["       , tok::tk_lbracket},
        {"]"       , tok::tk_rbracket},
        {"{"       , tok::tk_lbrace  },
        {"}"       , tok::tk_rbrace  },
        {";"       , tok::tk_semi    },
        {"and"     , tok::tk_and     },
        {"or"      , tok::tk_or      },
        {","       , tok::tk_comma   },
        {"."       , tok::tk_dot     },
        {"..."     , tok::tk_ellipsis},
        {"?"       , tok::tk_quesmark},
        {"??"      , tok::tk_quesques},
        {"?."      , tok::tk_quesdot },
        {":"       , tok::tk_colon   },
        {"+"       , tok::tk_add     },
        {"-"       , tok::tk_sub     },
        {"*"       , tok::tk_mult    },
        {"/"       , tok::tk_div     },
        {"~"       , tok::tk_floater },
        {"&"       , tok::tk_btand   },
        {"|"       , tok::tk_btor    },
        {"^"       , tok::tk_btxor   },
        {"!"       , tok::tk_not     },
        {"="       , tok::tk_eq      },
        {"+="      , tok::tk_addeq   },
        {"-="      , tok::tk_subeq   },
        {"*="      , tok::tk_multeq  },
        {"/="      , tok::tk_diveq   },
        {"~="      , tok::tk_lnkeq   },
        {"&="      , tok::tk_btandeq },
        {"|="      , tok::tk_btoreq  },
        {"^="      , tok::tk_btxoreq },
        {"=="      , tok::tk_cmpeq   },
        {"!="      , tok::tk_neq     },
        {"<"       , tok::tk_less    },
        {"<="      , tok::tk_leq     },
        {">"       , tok::tk_grt     },
        {">="      , tok::tk_geq     }
    };

private:
    tok get_type(const std::string&);
    bool skip(char);
    bool is_id(char);
    bool is_hex(char);
    bool is_oct(char);
    bool is_dec(char);
    bool is_str(char);
    bool is_quesmark(char);
    bool is_single_opr(char);
    bool is_calc_opr(char);

    void skip_note();
    void err_char();

    void open(const std::string&);
    std::string utf8_gen();
    token id_gen();
    token num_gen();
    token str_gen();
    token quesmark_gen();
    token single_opr();
    token dots();
    token calc_opr();

public:
    lexer(): line(1), column(0), ptr(0),
             filename(""), res(""),
             invalid_char(0) {}
    const error& scan(const std::string&);
    const auto& result() const {return toks;}
};

}
