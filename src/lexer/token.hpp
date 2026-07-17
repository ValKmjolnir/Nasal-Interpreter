#pragma once

#include <string>

#include "error/span.hpp"

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

}