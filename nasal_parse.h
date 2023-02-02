#pragma once

#include <unordered_map>

#include "nasal.h"
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_err.h"

/*
              _,,,_                                       
            .'     `'.                                    
           /     ____ \      Fucking Nasal Parser
           |  .-'_  _\/    /                              
           \_/   a  a|    /                               
           (,`     \ |         .----.                     
            |     -' |        /|     '--.                 
             \   '=  /        ||    ]|   `-.              
             /`-.__.'         ||    ]|    ::|             
          .-'`-.__ \__        ||    ]|    ::|             
         /        ``  `.      ||    ]|    ::|             
       _ |     \     \  \     \|    ]|   .-'              
      / \|      \    |   \     L.__  .--'(                
     |   |\      `.  |    \  ,---|_      \---------,      
     |   | '.      './\    \/ .--._|=-    |_      /|      
     |   \   '.     `'.'. /`\/ .-'          '.   / |      
     |   |     `'.     `;-:-;`)|             |-./  |      
     |   /_       `'--./_  ` )/'-------------')/)  |      
     \   | `""""----"`\//`""`/,===..'`````````/ (  |      
      |  |            / `---` `==='          /   ) |      
      /  \           /                      /   (  |      
     |    '------.  |'--------------------'|     ) |      
      \           `-|                      |    /  |      
       `--...,______|                      |   (   |      
              | |   |                      |    ) ,|      
              | |   |                      |   ( /||      
              | |   |                      |   )/ `"      
             /   \  |                      |  (/          
           .' /I\ '.|                      |  /)          
        .-'_.'/ \'. |                      | /            
        ```  `"""` `| .-------------------.||             
                    `"`                   `"`             
*/

class parse {
#define thisline (toks[ptr].tk_end_line)
#define thiscol (toks[ptr].tk_end_column)
#define thislen (toks[ptr].str.length())
private:
    u32 ptr;
    u32 in_func; // count function block
    u32 in_loop; // count loop block
    const token* toks;
    ast root;
    error& err;
    std::unordered_map<tok,string> tokname {
        {tok::rfor     ,"for"     },
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
        {tok::link    ,"~"       },
        {tok::opnot   ,"!"       },
        {tok::eq      ,"="       },
        {tok::addeq   ,"+="      },
        {tok::subeq   ,"-="      },
        {tok::multeq  ,"*="      },
        {tok::diveq   ,"/="      },
        {tok::lnkeq   ,"~="      },
        {tok::cmpeq   ,"=="      },
        {tok::neq     ,"!="      },
        {tok::less    ,"<"       },
        {tok::leq     ,"<="      },
        {tok::grt     ,">"       },
        {tok::geq     ,">="      }
    };

    void die(u32,u32,u32,string,bool);
    void next() {++ptr;};
    void match(tok type,const char* info=nullptr);
    bool lookahead(tok);
    bool is_call(tok);
    bool check_comma(const tok*);
    bool check_tuple();
    bool check_func_end(const ast&);
    bool check_special_call();
    bool need_semi_check(const ast&);
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
    parse(error& e):
        ptr(0),in_func(0),in_loop(0),
        toks(nullptr),root(0,0,ast_root,""),
        err(e) {}
    const error& compile(const lexer&);
    ast& tree() {return root;}
    const ast& tree() const {return root;}
};

const error& parse::compile(const lexer& lexer) {
    toks=lexer.result().data();
    ptr=in_func=in_loop=0;

    root={0,0,ast_root,toks[0].file};
    while(!lookahead(tok::eof)) {
        root.add(expr());
        if (lookahead(tok::semi)) {
            match(tok::semi);
        } else if (need_semi_check(root.child().back()) && !lookahead(tok::eof)) {
            // the last expression can be recognized without semi
            die(thisline,thiscol,thislen,"expected \";\"",true);
        }
    }
    return err;
}

void parse::die(u32 line,u32 col,u32 len,string info,bool prev=false) {
    // tok::str's str has no \"
    if (lookahead(tok::str)) {
        col-=2;
        len+=2;
    }
    // used to report lack of ',' ';'
    if (prev && ptr) {
        line=toks[ptr-1].tk_end_line;
        col=toks[ptr-1].tk_end_column;
        len=toks[ptr-1].str.length();
        len+=toks[ptr-1].type==tok::str?2:0;
    }
    err.err("parse",line,col,lookahead(tok::eof)?1:len,info);
}

void parse::match(tok type,const char* info) {
    if (!lookahead(type)) {
        if (info) {
            die(thisline,thiscol,thislen,info);
            return;
        }
        switch(type) {
            case tok::num:die(thisline,thiscol,thislen,"expected number");    break;
            case tok::str:die(thisline,thiscol,thislen,"expected string");    break;
            case tok::id: die(thisline,thiscol,thislen,"expected identifier");break;
            default:      die(thisline,thiscol,thislen,"expected '"+tokname[type]+"'"); break;
        }
        return;
    }
    if (lookahead(tok::eof)) {
        return;
    }
    next();
}

bool parse::lookahead(tok type) {
    return toks[ptr].type==type;
}

bool parse::is_call(tok type) {
    return type==tok::lcurve || type==tok::lbracket || type==tok::dot;
}

bool parse::check_comma(const tok* panic_set) {
    for(u32 i=0;panic_set[i]!=tok::null;++i) {
        if (lookahead(panic_set[i])) {
            die(thisline,thiscol,thislen,"expected ',' between scalars",true);
            return true;
        }
    }
    return false;
}

bool parse::check_tuple() {
    u32 check_ptr=ptr,curve=1,bracket=0,brace=0;
    while(toks[++check_ptr].type!=tok::eof && curve) {
        switch(toks[check_ptr].type) {
            case tok::lcurve:   ++curve;   break;
            case tok::lbracket: ++bracket; break;
            case tok::lbrace:   ++brace;   break;
            case tok::rcurve:   --curve;   break;
            case tok::rbracket: --bracket; break;
            case tok::rbrace:   --brace;   break;
            default: break;
        }
        if (curve==1 && !bracket && !brace && toks[check_ptr].type==tok::comma) {
            return true;
        }
    }
    return false;
}

bool parse::check_func_end(const ast& node) {
    u32 type=node.type();
    if (type==ast_func) {
        return true;
    } else if (type==ast_num || type==ast_id || type==ast_str || type==ast_nil || type==ast_vec || type==ast_hash) {
        return false;
    }
    if (node.child().empty() || (
            type!=ast_def &&
            type!=ast_equal &&
            type!=ast_addeq &&
            type!=ast_subeq &&
            type!=ast_multeq &&
            type!=ast_diveq &&
            type!=ast_lnkeq
        )
    ) {
        return false;
    } else {
        return check_func_end(node.child().back());
    }
    return false;
}

bool parse::check_special_call() {
    // special call means like this: function_name(a:1,b:2,c:3);
    u32 check_ptr=ptr,curve=1,bracket=0,brace=0;
    while(toks[++check_ptr].type!=tok::eof && curve) {
        switch(toks[check_ptr].type) {
            case tok::lcurve:   ++curve;  break;
            case tok::lbracket: ++bracket;break;
            case tok::lbrace:   ++brace;  break;
            case tok::rcurve:   --curve;  break;
            case tok::rbracket: --bracket;break;
            case tok::rbrace:   --brace;  break;
            default: break;
        }
        // m?1:0 will be recognized as normal parameter
        if (curve==1 && !bracket && !brace && toks[check_ptr].type==tok::quesmark) {
            return false;
        }
        if (curve==1 && !bracket && !brace && toks[check_ptr].type==tok::colon) {
            return true;
        }
    }
    return false;
}

bool parse::need_semi_check(const ast& node) {
    u32 type=node.type();
    if (type==ast_for || type==ast_foreach || type==ast_forindex || type==ast_while || type==ast_cond) {
        return false;
    }
    return !check_func_end(node);
}

ast parse::null() {
    return {toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file};
}

ast parse::nil() {
    return {toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_nil,toks[ptr].file};
}

ast parse::num() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_num,toks[ptr].file);
    node.set_num(str2num(toks[ptr].str.c_str()));
    match(tok::num);
    return node;
}

ast parse::str() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_str,toks[ptr].file);
    node.set_str(toks[ptr].str);
    match(tok::str);
    return node;
}

ast parse::id() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_id,toks[ptr].file);
    node.set_str(toks[ptr].str);
    match(tok::id);
    return node;
}

ast parse::bools() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_bool,toks[ptr].file);
    node.set_str(toks[ptr].str);
    if (lookahead(tok::tktrue)) {
        match(tok::tktrue);
    } else {
        match(tok::tkfalse);
    }
    return node;
}

ast parse::vec() {
    // panic set for this token is not ','
    // this is the FIRST set of calculation
    // array end with tok::null=0
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,
        tok::lbrace,tok::lbracket,tok::null
    };
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_vec,toks[ptr].file);
    match(tok::lbracket);
    while(!lookahead(tok::rbracket)) {
        node.add(calc());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::eof)) {
            break;
        } else if (!lookahead(tok::rbracket) && !check_comma(panic)) {
            break;
        }
    }
    match(tok::rbracket,"expected ']' when generating vector");
    return node;
}

ast parse::hash() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_hash,toks[ptr].file);
    match(tok::lbrace);
    while(!lookahead(tok::rbrace)) {
        node.add(pair());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id) || lookahead(tok::str)) { // first set of hashmember
            die(thisline,thiscol,thislen,"expected ',' between hash members",true);
        } else {
            break;
        }
    }
    match(tok::rbrace,"expected '}' when generating hash");
    return node;
}

ast parse::pair() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_pair,toks[ptr].file);
    if (lookahead(tok::id)) {
        node.add(id());
    } else if (lookahead(tok::str)) {
        node.add(str());
    } else {
        match(tok::id,"expected hashmap key");
    }
    match(tok::colon);
    node.add(calc());
    return node;
}

ast parse::func() {
    ++in_func;
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_func,toks[ptr].file);
    match(tok::func);
    if (lookahead(tok::lcurve)) {
        node.add(params());
    } else {
        node.add(null());
    }
    node.add(exprs());
    --in_func;
    return node;
}

ast parse::params() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_params,toks[ptr].file);
    match(tok::lcurve);
    while(!lookahead(tok::rcurve)) {
        ast tmp=id();
        if (lookahead(tok::eq) || lookahead(tok::ellipsis)) {
            ast special_arg(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file);
            if (lookahead(tok::eq)) {
                match(tok::eq);
                special_arg=std::move(tmp);
                special_arg.set_type(ast_default);
                special_arg.add(calc());
            } else {
                match(tok::ellipsis);
                special_arg=std::move(tmp);
                special_arg.set_type(ast_dynamic);
            }
            node.add(std::move(special_arg));
        } else {
            node.add(std::move(tmp));
        }
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id)) { // first set of identifier
            die(thisline,thiscol,thislen,"expected ',' between identifiers",true);
        } else {
            break;
        }
    }
    match(tok::rcurve,"expected ')' after parameter list");
    return node;
}

ast parse::lcurve_expr() {
    if (toks[ptr+1].type==tok::var)
        return definition();
    return check_tuple()?multi_assgin():calc();
}
ast parse::expr()
{
    tok type=toks[ptr].type;
    if ((type==tok::brk || type==tok::cont) && !in_loop) {
        die(thisline,thiscol,thislen,"must use break/continue in loops");
    }
    if (type==tok::ret && !in_func) {
        die(thisline,thiscol,thislen,"must use return in functions");
    }
    switch(type) {
        case tok::tknil:
        case tok::num:
        case tok::str:
        case tok::id:
        case tok::tktrue:
        case tok::tkfalse:
        case tok::func:
        case tok::lbracket:
        case tok::lbrace:
        case tok::sub:
        case tok::opnot:   return calc();          break;
        case tok::var:     return definition();    break;
        case tok::lcurve:  return lcurve_expr();   break;
        case tok::rfor:
        case tok::forindex:
        case tok::foreach:
        case tok::rwhile:  return loop();          break;
        case tok::rif:     return cond();          break;
        case tok::cont:    return continue_expr(); break;
        case tok::brk:     return break_expr();    break;
        case tok::ret:     return ret_expr();      break;
        case tok::semi:                            break;
        default:
            die(thisline,thiscol,thislen,"incorrect token <"+toks[ptr].str+">");
            next();
            break;
    }
    return {toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file};
}

ast parse::exprs() {
    if (lookahead(tok::eof)) {
        die(thisline,thiscol,thislen,"expected expression block");
        return null();
    }
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_block,toks[ptr].file);
    if (lookahead(tok::lbrace)) {
        match(tok::lbrace);
        while(!lookahead(tok::rbrace) && !lookahead(tok::eof)) {
            node.add(expr());
            if (lookahead(tok::semi)) {
                match(tok::semi);
            } else if (need_semi_check(node.child().back()) && !lookahead(tok::rbrace)) {
                // the last expression can be recognized without semi
                die(thisline,thiscol,thislen,"expected ';'",true);
            }
        }
        match(tok::rbrace,"expected '}' when generating expressions");
    } else {
        node.add(expr());
        if (lookahead(tok::semi))
            match(tok::semi);
    }
    return node;
}

ast parse::calc() {
    ast node=or_expr();
    if (lookahead(tok::quesmark)) {
        // trinocular calculation
        ast tmp(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_trino,toks[ptr].file);
        match(tok::quesmark);
        tmp.add(std::move(node));
        tmp.add(calc());
        match(tok::colon);
        tmp.add(calc());
        node=std::move(tmp);
    } else if (tok::eq<=toks[ptr].type && toks[ptr].type<=tok::lnkeq) {
        // tok::eq~tok::lnkeq is 37 to 42,ast_equal~ast_lnkeq is 21~26
        ast tmp(toks[ptr].tk_end_line,toks[ptr].tk_end_column,(u32)toks[ptr].type-(u32)tok::eq+ast_equal,toks[ptr].file);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(calc());
        node=std::move(tmp);
    }
    return node;
}

ast parse::or_expr() {
    ast node=and_expr();
    while(lookahead(tok::opor)) {
        ast tmp(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_or,toks[ptr].file);
        tmp.add(std::move(node));
        match(tok::opor);
        tmp.add(and_expr());
        node=std::move(tmp);
    }
    return node;
}

ast parse::and_expr() {
    ast node=cmp_expr();
    while(lookahead(tok::opand)) {
        ast tmp(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_and,toks[ptr].file);
        tmp.add(std::move(node));
        match(tok::opand);
        tmp.add(cmp_expr());
        node=std::move(tmp);
    }
    return node;
}

ast parse::cmp_expr() {
    ast node=additive_expr();
    while(tok::cmpeq<=toks[ptr].type && toks[ptr].type<=tok::geq) {
        // tok::cmpeq~tok::geq is 43~48,ast_cmpeq~ast_geq is 27~32
        ast tmp(toks[ptr].tk_end_line,toks[ptr].tk_end_column,(u32)toks[ptr].type-(u32)tok::cmpeq+ast_cmpeq,toks[ptr].file);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(additive_expr());
        node=std::move(tmp);
    }
    return node;
}

ast parse::additive_expr() {
    ast node=multive_expr();
    while(lookahead(tok::add) || lookahead(tok::sub) || lookahead(tok::link)) {
        ast tmp(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file);
        switch(toks[ptr].type) {
            case tok::add:  tmp.set_type(ast_add);  break;
            case tok::sub:  tmp.set_type(ast_sub);  break;
            case tok::link: tmp.set_type(ast_link); break;
            default: break;
        }
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(multive_expr());
        node=std::move(tmp);
    }
    return node;
}

ast parse::multive_expr() {
    ast node=(lookahead(tok::sub) || lookahead(tok::opnot))?unary():scalar();
    while(lookahead(tok::mult) || lookahead(tok::div)) {
        ast tmp(toks[ptr].tk_end_line,toks[ptr].tk_end_column,(u32)toks[ptr].type-(u32)tok::mult+ast_mult,toks[ptr].file);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add((lookahead(tok::sub) || lookahead(tok::opnot))?unary():scalar());
        node=std::move(tmp);
    }
    return node;
}

ast parse::unary() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file);
    switch(toks[ptr].type) {
        case tok::sub:   node.set_type(ast_neg);match(tok::sub);break;
        case tok::opnot: node.set_type(ast_not);match(tok::opnot);break;
        default: break;
    }
    node.add((lookahead(tok::sub) || lookahead(tok::opnot))?unary():scalar());
    return node;
}

ast parse::scalar() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file);
    if (lookahead(tok::tknil)) {
        node=nil();
        match(tok::tknil);
    } else if (lookahead(tok::num)) {
        node=num();
    } else if (lookahead(tok::str)) {
        node=str();
    } else if (lookahead(tok::id)) {
        node=id();
    } else if (lookahead(tok::tktrue) || lookahead(tok::tkfalse)) {
        node=bools();
    } else if (lookahead(tok::func)) {
        node=func();
    } else if (lookahead(tok::lbracket)) {
        node=vec();
    } else if (lookahead(tok::lbrace)) {
        node=hash();
    } else if (lookahead(tok::lcurve)) {
        match(tok::lcurve);
        node=calc();
        match(tok::rcurve);
    } else if (lookahead(tok::var)) {
        match(tok::var);
        node.set_type(ast_def);
        node.add(id());
        match(tok::eq);
        node.add(calc());
    } else {
        die(thisline,thiscol,thislen,"expected scalar");
        return node;
    }
    // check call and avoid ambiguous syntax
    if (is_call(toks[ptr].type) && !(lookahead(tok::lcurve) && toks[ptr+1].type==tok::var)) {
        ast tmp=std::move(node);
        node={toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_call,toks[ptr].file};
        node.add(std::move(tmp));
        while(is_call(toks[ptr].type)) {
            node.add(call_scalar());
        }
    }
    return node;
}

ast parse::call_scalar() {
    switch(toks[ptr].type) {
        case tok::lcurve:   return callf(); break;
        case tok::lbracket: return callv(); break;
        case tok::dot:      return callh(); break;
        default: break;
    }
    // should never run this expression
    return {toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_nil,toks[ptr].file};
}

ast parse::callh() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_callh,toks[ptr].file);
    match(tok::dot);
    node.set_str(toks[ptr].str);
    match(tok::id,"expected hashmap key"); // get key
    return node;
}

ast parse::callv() {
    // panic set for this token is not ','
    // this is the FIRST set of subvec
    // array end with tok::null=0
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,
        tok::lbrace,tok::lbracket,tok::colon,
        tok::null
    };
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_callv,toks[ptr].file);
    match(tok::lbracket);
    while(!lookahead(tok::rbracket)) {
        node.add(subvec());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::eof)) {
            break;
        } else if (!lookahead(tok::rbracket) && !check_comma(panic)) {
            break;
        }
    }
    if (node.size()==0) {
        die(node.line(),node.col(),1,"expected index value");
    }
    match(tok::rbracket,"expected ']' when calling vector");
    return node;
}

ast parse::callf() {
    // panic set for this token is not ','
    // this is the FIRST set of calculation/hashmember
    // array end with tok::null=0
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,
        tok::lbrace,tok::lbracket,tok::null
    };
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_callf,toks[ptr].file);
    bool special_call=check_special_call();
    match(tok::lcurve);
    while(!lookahead(tok::rcurve)) {
        node.add(special_call?pair():calc());
        if (lookahead(tok::comma))
            match(tok::comma);
        else if (lookahead(tok::eof))
            break;
        else if (!lookahead(tok::rcurve) && !check_comma(panic))
            break;
    }
    match(tok::rcurve,"expected ')' when calling function");
    return node;
}

ast parse::subvec() {
    ast node=lookahead(tok::colon)?nil():calc();
    if (lookahead(tok::colon)) {
        ast tmp(node.line(),node.col(),ast_subvec,toks[ptr].file);
        match(tok::colon);
        tmp.add(std::move(node));
        tmp.add((lookahead(tok::comma) || lookahead(tok::rbracket))?nil():calc());
        node=std::move(tmp);
    }
    return node;
}

ast parse::definition() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_def,toks[ptr].file);
    if (lookahead(tok::var)) {
        match(tok::var);
        switch(toks[ptr].type) {
            case tok::id:     node.add(id());break;
            case tok::lcurve: node.add(outcurve_def());break;
            default:          die(thisline,thiscol,thislen,"expected identifier");break;
        }
    } else if (lookahead(tok::lcurve)) {
        node.add(incurve_def());
    }
    match(tok::eq);
    if (lookahead(tok::lcurve)) {
        node.add(check_tuple()?multi_scalar():calc());
    } else {
        node.add(calc());
    }
    return node;
}

ast parse::incurve_def() {
    match(tok::lcurve);
    match(tok::var);
    ast node=multi_id();
    match(tok::rcurve);
    return node;
}

ast parse::outcurve_def() {
    match(tok::lcurve);
    ast node=multi_id();
    match(tok::rcurve);
    return node;
}

ast parse::multi_id() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_multi_id,toks[ptr].file);
    while(!lookahead(tok::eof)) {
        node.add(id());
        if (is_call(toks[ptr].type)) {
            ast tmp=call_scalar();// recognize calls but this is still a syntax error
            die(tmp.line(),tmp.col(),1,"cannot call identifier in multi-definition");
        }
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::id)) { // first set of identifier
            die(thisline,thiscol,thislen,"expected ',' between identifiers",true);
        } else {
            break;
        }
    }
    return node;
}

ast parse::multi_scalar() {
    // if check_call_memory is true,we will check if value called here can reach a memory space
    const tok panic[]={
        tok::id,tok::str,tok::num,tok::tktrue,
        tok::tkfalse,tok::opnot,tok::sub,tok::tknil,
        tok::func,tok::var,tok::lcurve,
        tok::lbrace,tok::lbracket,tok::null
    };
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_tuple,toks[ptr].file);
    match(tok::lcurve);
    while(!lookahead(tok::rcurve)) {
        node.add(calc());
        if (lookahead(tok::comma)) {
            match(tok::comma);
        } else if (lookahead(tok::eof)) {
            break;
        } else if (!lookahead(tok::rcurve) && !check_comma(panic)) {
            break;
        }
    }
    match(tok::rcurve,"expected ')' after multi-scalar");
    return node;
}

ast parse::multi_assgin() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_multi_assign,toks[ptr].file);
    node.add(multi_scalar());
    match(tok::eq);
    if (lookahead(tok::eof)) {
        die(thisline,thiscol,thislen,"expected value list");
        return node;
    }
    if (lookahead(tok::lcurve)) {
        node.add(check_tuple()?multi_scalar():calc());
    } else {
        node.add(calc());
    }
    return node;
}

ast parse::loop() {
    ++in_loop;
    ast node(0,0,ast_null,toks[ptr].file);
    switch(toks[ptr].type) {
        case tok::rwhile:  node=while_loop(); break;
        case tok::rfor:    node=for_loop();   break;
        case tok::forindex:
        case tok::foreach: node=forei_loop(); break;
        default: break;
    }
    --in_loop;
    return node;
}

ast parse::while_loop() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_while,toks[ptr].file);
    match(tok::rwhile);
    match(tok::lcurve);
    node.add(calc());
    match(tok::rcurve);
    node.add(exprs());
    return node;
}

ast parse::for_loop() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_for,toks[ptr].file);
    match(tok::rfor);
    match(tok::lcurve);
    // first expression
    if (lookahead(tok::eof)) {
        die(thisline,thiscol,thislen,"expected definition");
    }
    if (lookahead(tok::semi)) {
        node.add(null());
    } else if (lookahead(tok::var)) {
        node.add(definition());
    } else if (lookahead(tok::lcurve)) {
        node.add(lcurve_expr());
    } else {
        node.add(calc());
    }
    match(tok::semi,"expected ';' in for(;;)");
    // conditional expression
    if (lookahead(tok::eof)) {
        die(thisline,thiscol,thislen,"expected conditional expr");
    }
    if (lookahead(tok::semi)) {
        node.add(null());
    } else {
        node.add(calc());
    }
    match(tok::semi,"expected ';' in for(;;)");
    //after loop expression
    if (lookahead(tok::eof)) {
        die(thisline,thiscol,thislen,"expected calculation");
    }
    if (lookahead(tok::rcurve)) {
        node.add(null());
    } else {
        node.add(calc());
    }
    match(tok::rcurve);
    node.add(exprs());
    return node;
}

ast parse::forei_loop() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file);
    switch(toks[ptr].type) {
        case tok::forindex:node.set_type(ast_forindex);match(tok::forindex);break;
        case tok::foreach: node.set_type(ast_foreach); match(tok::foreach); break;
        default: break;
    }
    match(tok::lcurve);
    // first expression
    // foreach/forindex must have an iterator to loop through
    if (!lookahead(tok::var) && !lookahead(tok::id)) {
        die(thisline,thiscol,thislen,"expected iterator");
    }
    node.add(iter_gen());
    match(tok::semi,"expected ';' in foreach/forindex(iter;vector)");
    if (lookahead(tok::eof)) {
        die(thisline,thiscol,thislen,"expected vector");
    }
    node.add(calc());
    match(tok::rcurve);
    node.add(exprs());
    return node;
}

ast parse::iter_gen() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_null,toks[ptr].file);
    if (lookahead(tok::var)) {
        match(tok::var);
        node.set_type(ast_iter);
        node.add(id());
    } else {
        node.set_type(ast_call);
        node.add(id());
        while(is_call(toks[ptr].type)) {
            node.add(call_scalar());
        }
    }
    return node;
}

ast parse::cond() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_cond,toks[ptr].file);
    ast ifnode(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_if,toks[ptr].file);
    match(tok::rif);
    match(tok::lcurve);
    ifnode.add(calc());
    match(tok::rcurve);
    ifnode.add(exprs());
    node.add(std::move(ifnode));
    while(lookahead(tok::elsif)) {
        ast elsifnode(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_elsif,toks[ptr].file);
        match(tok::elsif);
        match(tok::lcurve);
        elsifnode.add(calc());
        match(tok::rcurve);
        elsifnode.add(exprs());
        node.add(std::move(elsifnode));
    }
    if (lookahead(tok::relse)) {
        ast elsenode(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_else,toks[ptr].file);
        match(tok::relse);
        elsenode.add(exprs());
        node.add(std::move(elsenode));
    }
    return node;
}

ast parse::continue_expr() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_continue,toks[ptr].file);
    match(tok::cont);
    return node;
}

ast parse::break_expr() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_break,toks[ptr].file);
    match(tok::brk);
    return node;
}

ast parse::ret_expr() {
    ast node(toks[ptr].tk_end_line,toks[ptr].tk_end_column,ast_ret,toks[ptr].file);
    match(tok::ret);
    tok type=toks[ptr].type;
    if (type==tok::tknil || type==tok::num || type==tok::str || type==tok::id ||
       type==tok::func || type==tok::sub || type==tok::opnot || type==tok::lcurve ||
       type==tok::lbracket || type==tok::lbrace || type==tok::tktrue || type==tok::tkfalse
    ) {
        node.add(calc());
    }
    return node;
}
