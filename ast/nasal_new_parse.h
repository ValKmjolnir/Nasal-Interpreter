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
    ast root;
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
    void die(const span&,string);
    void next() {++ptr;}
    void match(tok type, const char* info=nullptr);
    bool lookahead(tok);
    bool is_call(tok);
    bool check_comma(const tok*);
    bool check_tuple();
    bool check_func_end(const ast&);
    bool check_special_call();
    bool need_semi_check(const ast&);

private:
    ast null();
    ast nil();
    ast num();
    ast str();
    ast id();
    ast bools();
    ast vec();
    ast hash();
    ast pair();
    ast func();
    ast params();
    ast lcurve_expr();
    ast expr();
    ast exprs();
    ast calc();
    ast bitwise_or();
    ast bitwise_xor();
    ast bitwise_and();
    ast or_expr();
    ast and_expr();
    ast cmp_expr();
    ast additive_expr();
    ast multive_expr();
    ast unary();
    ast scalar();
    ast call_scalar();
    ast callh();
    ast callv();
    ast callf();
    ast subvec();
    ast definition();
    ast incurve_def();
    ast outcurve_def();
    ast multi_id();
    ast multi_scalar();
    ast multi_assgin();
    ast loop();
    ast while_loop();
    ast for_loop();
    ast forei_loop();
    ast iter_gen();
    ast cond();
    ast continue_expr();
    ast break_expr();
    ast ret_expr();

public:
    inline ast& tree() {return root;}
    inline const ast& tree() const {return root;}

public:
    parse(error& e):
        ptr(0), in_func(0), in_loop(0),
        toks(nullptr),
        root({0, 0, 0, 0, ""}, ast_root),
        err(e) {}
    const error& compile(const lexer&);
    void easter_egg() const;
};