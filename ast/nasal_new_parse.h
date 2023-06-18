#pragma once

#include <unordered_map>

#include "nasal.h"
#include "nasal_new_ast.h"
#include "nasal_lexer.h"
#include "nasal_ast.h"
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
    error& err;

private:
    const std::unordered_map<tok,string> tokname {
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
    void next() {++ptr;}
    void match(tok type, const char* info=nullptr);
    bool lookahead(tok);
    bool is_call(tok);
    bool check_comma(const tok*);
    bool check_tuple();
    bool check_func_end(const ast&);
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
    expr* unary();
    expr* scalar();
    expr* call_scalar();
    expr* callh();
    expr* callv();
    expr* callf();
    expr* subvec();
    expr* definition();
    expr* incurve_def();
    expr* outcurve_def();
    expr* multi_id();
    expr* multi_scalar();
    expr* multi_assgin();
    expr* loop();
    expr* while_loop();
    expr* for_loop();
    expr* forei_loop();
    expr* iter_gen();
    expr* cond();
    continue_expr* continue_expression();
    break_expr* break_expression();
    return_expr* return_expression();

public:
    inline code_block* tree() {return root;}

public:
    parse(error& e):
        ptr(0), in_func(0), in_loop(0),
        toks(nullptr),
        root(nullptr),
        err(e) {}
    const error& compile(const lexer&);
    void easter_egg() const;
};