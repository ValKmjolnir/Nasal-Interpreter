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

#include "nasal.hpp"
#include "error/error.hpp"
#include "lexer/token.hpp"

namespace nasal {

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
