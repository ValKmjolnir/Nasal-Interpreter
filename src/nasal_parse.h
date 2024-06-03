#pragma once

#include <unordered_map>

#include "nasal.h"
#include "nasal_ast.h"
#include "nasal_lexer.h"
#include "nasal_err.h"

namespace nasal {

class parse {

#define thisspan (toks[ptr].loc)
#define prevspan (ptr!=0? toks[ptr-1].loc:toks[ptr].loc)

private:
    u64 ptr;
    u64 in_func_depth; // count function block
    u64 in_loop_depth; // count loop block
    const token* toks;
    code_block* root;
    error err;

private:
    const std::unordered_map<tok, std::string> token_name_mapper = {
        {tok::tk_true    , "true"    },
        {tok::tk_false   , "false"   },
        {tok::tk_use     , "use"     },
        {tok::tk_for     , "for"     },
        {tok::tk_forindex, "forindex"},
        {tok::tk_foreach , "foreach" },
        {tok::tk_while   , "while"   },
        {tok::tk_var     , "var"     },
        {tok::tk_func    , "func"    },
        {tok::tk_brk     , "break"   },
        {tok::tk_cont    , "continue"},
        {tok::tk_ret     , "return"  },
        {tok::tk_if      , "if"      },
        {tok::tk_elsif   , "elsif"   },
        {tok::tk_else    , "else"    },
        {tok::tk_nil     , "nil"     },
        {tok::tk_lcurve  , "("       },
        {tok::tk_rcurve  , ")"       },
        {tok::tk_lbracket, "["       },
        {tok::tk_rbracket, "]"       },
        {tok::tk_lbrace  , "{"       },
        {tok::tk_rbrace  , "}"       },
        {tok::tk_semi    , ";"       },
        {tok::tk_and     , "and"     },
        {tok::tk_or      , "or"      },
        {tok::tk_comma   , ","       },
        {tok::tk_dot     , "."       },
        {tok::tk_ellipsis, "..."     },
        {tok::tk_quesmark, "?"       },
        {tok::tk_quesques, "??"      },
        {tok::tk_quesdot , "?."      },
        {tok::tk_colon   , ":"       },
        {tok::tk_add     , "+"       },
        {tok::tk_sub     , "-"       },
        {tok::tk_mult    , "*"       },
        {tok::tk_div     , "/"       },
        {tok::tk_floater , "~"       },
        {tok::tk_btand   , "&"       },
        {tok::tk_btor    , "|"       },
        {tok::tk_btxor   , "^"       },
        {tok::tk_not     , "!"       },
        {tok::tk_eq      , "="       },
        {tok::tk_addeq   , "+="      },
        {tok::tk_subeq   , "-="      },
        {tok::tk_multeq  , "*="      },
        {tok::tk_diveq   , "/="      },
        {tok::tk_lnkeq   , "~="      },
        {tok::tk_btandeq , "&="      },
        {tok::tk_btoreq  , "|="      },
        {tok::tk_btxoreq , "^="      },
        {tok::tk_cmpeq   , "=="      },
        {tok::tk_neq     , "!="      },
        {tok::tk_less    , "<"       },
        {tok::tk_leq     , "<="      },
        {tok::tk_grt     , ">"       },
        {tok::tk_geq     , ">="      }
    };

private:
    void die(const span&, const std::string&);
    void next();
    void match(tok, const char* info = nullptr);
    bool lookahead(tok);
    bool is_call(tok);
    bool check_comma(const tok*);
    bool check_tuple();
    bool check_func_end(expr*);
    bool check_in_curve_multi_definition();
    bool check_special_call();
    bool need_semi_check(expr*);
    void update_location(expr*);

private:
    use_stmt* use_stmt_gen();
    null_expr* null();
    nil_expr* nil();
    number_literal* num();
    string_literal* str();
    identifier* id();
    bool_literal* bools();
    vector_expr* vec();
    hash_expr* hash();
    hash_pair* pair();
    function* func();
    void params(function*);
    expr* lcurve_expr();
    expr* expression();
    code_block* expression_block();
    expr* calc();
    expr* bitwise_or();
    expr* bitwise_xor();
    expr* bitwise_and();
    expr* or_expr();
    expr* and_expr();
    expr* cmp_expr();
    expr* null_chain_expr();
    expr* additive_expr();
    expr* multive_expr();
    unary_operator* unary();
    expr* scalar();
    call* call_scalar();
    call_hash* callh();
    null_access* null_access_call();
    call_vector* callv();
    call_function* callf();
    slice_vector* subvec();
    expr* definition();
    multi_identifier* incurve_def();
    multi_identifier* outcurve_def();
    multi_identifier* multi_id();
    tuple_expr* multi_scalar();
    multi_assign* multi_assignment();
    expr* loop();
    while_expr* while_loop();
    for_expr* for_loop();
    forei_expr* forei_loop();
    iter_expr* iter_gen();
    condition_expr* cond();
    continue_expr* continue_expression();
    break_expr* break_expression();
    return_expr* return_expression();

public:
    code_block* tree() {return root;}

    // swap root pointer with another pointer(maybe nullptr)
    code_block* swap(code_block* another) {
        auto res = root;
        root = another;
        return res;
    }

public:
    parse(): ptr(0), in_func_depth(0),
             in_loop_depth(0), toks(nullptr),
             root(nullptr) {}
    ~parse() {delete root;}
    const error& compile(const lexer&);
    static void easter_egg();
};

}
