#pragma once

#include <unordered_map>

#include "nasal.h"
#include "nasal_ast.h"
#include "nasal_lexer.h"
#include "nasal_err.h"

class parse {

#define thisspan (toks[ptr].loc)
#define prevspan (ptr!=0? toks[ptr-1].loc:toks[ptr].loc)

private:
    u32 ptr;
    u32 in_func; // count function block
    u32 in_loop; // count loop block
    const token* toks;
    code_block* root;
    error err;

private:
    const std::unordered_map<tok, std::string> tokname {
        {tok::rfor    ,"for"     },
        {tok::forindex,"forindex"},
        {tok::foreach ,"foreach" },
        {tok::rwhile  ,"while"   },
        {tok::var     ,"var"     },
        {tok::func    ,"func"    },
        {tok::brk     ,"break"   },
        {tok::cont    ,"continue"},
        {tok::ret     ,"return"  },
        {tok::rif     ,"if"      },
        {tok::elsif   ,"elsif"   },
        {tok::relse   ,"else"    },
        {tok::tknil   ,"nil"     },
        {tok::lcurve  ,"("       },
        {tok::rcurve  ,")"       },
        {tok::lbracket,"["       },
        {tok::rbracket,"]"       },
        {tok::lbrace  ,"{"       },
        {tok::rbrace  ,"}"       },
        {tok::semi    ,";"       },
        {tok::opand   ,"and"     },
        {tok::opor    ,"or"      },
        {tok::comma   ,","       },
        {tok::dot     ,"."       },
        {tok::ellipsis,"..."     },
        {tok::quesmark,"?"       },
        {tok::colon   ,":"       },
        {tok::add     ,"+"       },
        {tok::sub     ,"-"       },
        {tok::mult    ,"*"       },
        {tok::div     ,"/"       },
        {tok::floater ,"~"       },
        {tok::btand   ,"&"       },
        {tok::btor    ,"|"       },
        {tok::btxor   ,"^"       },
        {tok::opnot   ,"!"       },
        {tok::eq      ,"="       },
        {tok::addeq   ,"+="      },
        {tok::subeq   ,"-="      },
        {tok::multeq  ,"*="      },
        {tok::diveq   ,"/="      },
        {tok::lnkeq   ,"~="      },
        {tok::btandeq ,"&="      },
        {tok::btoreq  ,"|="      },
        {tok::btxoreq ,"^="      },
        {tok::cmpeq   ,"=="      },
        {tok::neq     ,"!="      },
        {tok::less    ,"<"       },
        {tok::leq     ,"<="      },
        {tok::grt     ,">"       },
        {tok::geq     ,">="      }
    };

private:
    void die(const span&,std::string);
    void next();
    void match(tok, const char* info=nullptr);
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
    expr* additive_expr();
    expr* multive_expr();
    unary_operator* unary();
    expr* scalar();
    call* call_scalar();
    call_hash* callh();
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
    parse(): ptr(0), in_func(0), in_loop(0), toks(nullptr), root(nullptr) {}
    ~parse() {
        if (root) {
            delete root;
        }
    }
    const error& compile(const lexer&);
    static void easter_egg();
};