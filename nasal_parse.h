#pragma once

#include <unordered_map>

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

class parse
{
#define err_line (toks[ptr].line)
private:
    u32 ptr;
    u32 in_func; // count function block
    u32 in_loop; // count loop block
    const token* toks;
    ast root;
    error& err;
    std::unordered_map<u32,string> tokname {
        {tok_for     ,"for"     },
        {tok_forindex,"forindex"},
        {tok_foreach ,"foreach" },
        {tok_while   ,"while"   },
        {tok_var     ,"var"     },
        {tok_func    ,"func"    },
        {tok_break   ,"break"   },
        {tok_continue,"continue"},
        {tok_ret     ,"return"  },
        {tok_if      ,"if"      },
        {tok_elsif   ,"elsif"   },
        {tok_else    ,"else"    },
        {tok_nil     ,"nil"     },
        {tok_lcurve  ,"("       },
        {tok_rcurve  ,")"       },
        {tok_lbracket,"["       },
        {tok_rbracket,"]"       },
        {tok_lbrace  ,"{"       },
        {tok_rbrace  ,"}"       },
        {tok_semi    ,";"       },
        {tok_and     ,"and"     },
        {tok_or      ,"or"      },
        {tok_comma   ,","       },
        {tok_dot     ,"."       },
        {tok_ellipsis,"..."     },
        {tok_quesmark,"?"       },
        {tok_colon   ,":"       },
        {tok_add     ,"+"       },
        {tok_sub     ,"-"       },
        {tok_mult    ,"*"       },
        {tok_div     ,"/"       },
        {tok_link    ,"~"       },
        {tok_not     ,"!"       },
        {tok_eq      ,"="       },
        {tok_addeq   ,"+="      },
        {tok_subeq   ,"-="      },
        {tok_multeq  ,"*="      },
        {tok_diveq   ,"/="      },
        {tok_lnkeq   ,"~="      },
        {tok_cmpeq   ,"=="      },
        {tok_neq     ,"!="      },
        {tok_less    ,"<"       },
        {tok_leq     ,"<="      },
        {tok_grt     ,">"       },
        {tok_geq     ,">="      }
    };

    void die(u32,string,bool);
    void next() {++ptr;};
    void match(u32 type,const char* info=nullptr);
    bool lookahead(u32);
    bool is_call(u32);
    bool check_comma(const u32*);
    bool check_tuple();
    bool check_func_end(const ast&);
    bool check_special_call();
    bool need_semi_check(const ast&);
    void check_memory_reachable(const ast&);
    ast null();
    ast nil();
    ast num();
    ast str();
    ast id();
    ast vec();
    ast hash();
    ast pair();
    ast func();
    ast args();
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
    ast multi_scalar(bool);
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
        toks(nullptr),root(0,0,ast_root),
        err(e){}
    void print(){root.print_tree();}
    const error& compile(const lexer&);
    ast& tree(){return root;}
    const ast& tree() const {return root;}
};
const error& parse::compile(const lexer& lexer)
{
    toks=lexer.result().data();
    ptr=in_func=in_loop=0;

    root={0,0,ast_root};
    while(!lookahead(tok_eof))
    {
        root.add(expr());
        if(lookahead(tok_semi))
            match(tok_semi);
        // the last expression can be recognized without semi
        else if(need_semi_check(root.child().back()) && !lookahead(tok_eof))
            die(err_line,"expected \";\"",true);
    }
    return err;
}
void parse::die(u32 line,string info,bool report_prev=false)
{
    i32 col=(i32)toks[ptr].col-(lookahead(tok_eof)?0:(i32)toks[ptr].str.length());
    if(lookahead(tok_str))
        col-=2; // tok_str's str has no \"
    if(report_prev && ptr) // used to report lack of ',' ';'
    {
        line=toks[ptr-1].line;
        col=toks[ptr-1].col+1;
    }
    err.err("parse",line,col<0?0:col,info);
}
void parse::match(u32 type,const char* info)
{
    if(!lookahead(type))
    {
        if(info)
        {
            die(err_line,info);
            return;
        }
        switch(type)
        {
            case tok_num:die(err_line,"expected number");    break;
            case tok_str:die(err_line,"expected string");    break;
            case tok_id: die(err_line,"expected identifier");break;
            default:     die(err_line,"expected '"+tokname[type]+"'"); break;
        }
        return;
    }
    if(lookahead(tok_eof))
        return;
    next();
}
bool parse::lookahead(u32 type)
{
    return toks[ptr].type==type;
}
bool parse::is_call(u32 type)
{
    return type==tok_lcurve || type==tok_lbracket || type==tok_dot;
}
bool parse::check_comma(const u32* panic_set)
{
    for(u32 i=0;panic_set[i];++i)
        if(lookahead(panic_set[i]))
        {
            die(err_line,"expected ',' between scalars",true);
            return true;
        }
    return false;
}
bool parse::check_tuple()
{
    u32 check_ptr=ptr,curve=1,bracket=0,brace=0;
    while(toks[++check_ptr].type!=tok_eof && curve)
    {
        switch(toks[check_ptr].type)
        {
            case tok_lcurve:   ++curve;   break;
            case tok_lbracket: ++bracket; break;
            case tok_lbrace:   ++brace;   break;
            case tok_rcurve:   --curve;   break;
            case tok_rbracket: --bracket; break;
            case tok_rbrace:   --brace;   break;
        }
        if(curve==1 && !bracket && !brace && toks[check_ptr].type==tok_comma)
            return true;
    }
    return false;
}
bool parse::check_func_end(const ast& node)
{
    u32 type=node.type();
    if(type==ast_func)
        return true;
    else if(type==ast_num || type==ast_id || type==ast_str || type==ast_nil || type==ast_vec || type==ast_hash)
        return false;
    if(
        node.child().empty() || 
        (
            type!=ast_def &&
            type!=ast_equal &&
            type!=ast_addeq &&
            type!=ast_subeq &&
            type!=ast_multeq &&
            type!=ast_diveq &&
            type!=ast_lnkeq
        )
    )
        return false;
    else
        return check_func_end(node.child().back());
    return false;
}
bool parse::check_special_call()
{
    // special call means like this: function_name(a:1,b:2,c:3);
    u32 check_ptr=ptr,curve=1,bracket=0,brace=0;
    while(toks[++check_ptr].type!=tok_eof && curve)
    {
        switch(toks[check_ptr].type)
        {
            case tok_lcurve:   ++curve;  break;
            case tok_lbracket: ++bracket;break;
            case tok_lbrace:   ++brace;  break;
            case tok_rcurve:   --curve;  break;
            case tok_rbracket: --bracket;break;
            case tok_rbrace:   --brace;  break;
        }
        // m?1:0 will be recognized as normal parameter
        if(curve==1 && !bracket && !brace && toks[check_ptr].type==tok_quesmark)
            return false;
        if(curve==1 && !bracket && !brace && toks[check_ptr].type==tok_colon)
            return true;
    }
    return false;
}
bool parse::need_semi_check(const ast& node)
{
    u32 type=node.type();
    if(type==ast_for || type==ast_foreach || type==ast_forindex || type==ast_while || type==ast_cond)
        return false;
    return !check_func_end(node);
}
void parse::check_memory_reachable(const ast& node)
{
    if(node.type()==ast_call)
    {
        const ast& tmp=node.child().back();
        if(tmp.type()==ast_callf)
            die(tmp.line(),"bad left-value");
        if(tmp.type()==ast_callv && (tmp.size()==0 || tmp.size()>1 || tmp[0].type()==ast_subvec))
            die(tmp.line(),"bad left-value");
    }
    else if(node.type()!=ast_id)
        die(node.line(),"bad left-value");
}
ast parse::null()
{
    return {toks[ptr].line,toks[ptr].col,ast_null};
}
ast parse::nil()
{
    return {toks[ptr].line,toks[ptr].col,ast_nil};
}
ast parse::num()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_num);
    node.set_num(str2num(toks[ptr].str.c_str()));
    match(tok_num);
    return node;
}
ast parse::str()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_str);
    node.set_str(toks[ptr].str);
    match(tok_str);
    return node;
}
ast parse::id()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_id);
    node.set_str(toks[ptr].str);
    match(tok_id);
    return node;
}
ast parse::vec()
{
    // panic set for this token is not ','
    // this is the FIRST set of calculation
    // array end with tok_null=0
    const u32 panic_set[]={
        tok_id,tok_str,tok_num,
        tok_not,tok_sub,tok_nil,
        tok_func,tok_var,tok_lcurve,
        tok_lbrace,tok_lbracket,tok_null
    };
    ast node(toks[ptr].line,toks[ptr].col,ast_vec);
    match(tok_lbracket);
    while(!lookahead(tok_rbracket))
    {
        node.add(calc());
        if(lookahead(tok_comma))
            match(tok_comma);
        else if(lookahead(tok_eof))
            break;
        else if(!lookahead(tok_rbracket) && !check_comma(panic_set))
            break;
    }
    match(tok_rbracket,"expected ']' when generating vector");
    return node;
}
ast parse::hash()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_hash);
    match(tok_lbrace);
    while(!lookahead(tok_rbrace))
    {
        node.add(pair());
        if(lookahead(tok_comma))
            match(tok_comma);
        else if(lookahead(tok_id) || lookahead(tok_str))// first set of hashmember
            die(err_line,"expected ',' between hash members",true);
        else
            break;
    }
    match(tok_rbrace,"expected '}' when generating hash");
    return node;
}
ast parse::pair()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_pair);
    if(lookahead(tok_id))
        node.add(id());
    else if(lookahead(tok_str))
        node.add(str());
    else
        match(tok_id,"expected hashmap key");
    match(tok_colon);
    node.add(calc());
    return node;
}
ast parse::func()
{
    ++in_func;
    ast node(toks[ptr].line,toks[ptr].col,ast_func);
    match(tok_func);
    if(lookahead(tok_lcurve))
        node.add(args());
    else
        node.add(null());
    node.add(exprs());
    --in_func;
    return node;
}
ast parse::args()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_args);
    match(tok_lcurve);
    while(!lookahead(tok_rcurve))
    {
        ast tmp=id();
        if(lookahead(tok_eq) || lookahead(tok_ellipsis))
        {
            ast special_arg(toks[ptr].line,toks[ptr].col,ast_null);
            if(lookahead(tok_eq))
            {
                match(tok_eq);
                special_arg=std::move(tmp);
                special_arg.set_type(ast_default);
                special_arg.add(calc());
            }
            else
            {
                match(tok_ellipsis);
                special_arg=std::move(tmp);
                special_arg.set_type(ast_dynamic);
            }
            node.add(std::move(special_arg));
        }
        else
            node.add(std::move(tmp));
        if(lookahead(tok_comma))
            match(tok_comma);
        else if(lookahead(tok_id))// first set of identifier
            die(err_line,"expected ',' between identifiers",true);
        else
            break;
    }
    match(tok_rcurve,"expected ')' after parameter list");

    string format="func(";
    for(auto& tmp:node.child())
    {
        format+=tmp.str();
        switch(tmp.type())
        {
            case ast_id:                     break;
            case ast_default: format+="=val";break;
            case ast_dynamic: format+="..."; break;
        }
        format+=",)"[&tmp==&node.child().back()];
    }
    bool checked_default=false,checked_dynamic=false;
    for(auto& tmp:node.child())
    {
        if(tmp.type()==ast_default)
            checked_default=true;
        else if(tmp.type()==ast_dynamic)
            checked_dynamic=true;
        if(checked_default && tmp.type()!=ast_default)
            die(tmp.line(),"must use default paras after using once: "+format);
        if(checked_dynamic && &tmp!=&node.child().back())
            die(tmp.line(),"dynamic para must be the end: "+format);
    }
    std::unordered_map<string,bool> argname;
    for(auto& tmp:node.child())
    {
        string name;
        switch(tmp.type())
        {
            case ast_dynamic:
            case ast_id:     name=tmp.str();break;
            case ast_default:name=tmp.str();break;
        }
        if(argname.count(name))
            die(tmp.line(),"parameter's name repeats: "+name);
        else
            argname[name]=true;
    }
    return node;
}
ast parse::lcurve_expr()
{
    if(toks[ptr+1].type==tok_var)
        return definition();
    return check_tuple()?multi_assgin():calc();
}
ast parse::expr()
{
    u32 type=toks[ptr].type;
    if((type==tok_break || type==tok_continue) && !in_loop)
        die(err_line,"should use break/continue in loops");
    if(type==tok_ret && !in_func)
        die(err_line,"should use return in functions");
    switch(type)
    {
        case tok_nil:
        case tok_num:
        case tok_str:
        case tok_id:
        case tok_func:
        case tok_lbracket:
        case tok_lbrace:
        case tok_sub:
        case tok_not:      return calc();          break;
        case tok_var:      return definition();    break;
        case tok_lcurve:   return lcurve_expr();   break;
        case tok_for:
        case tok_forindex:
        case tok_foreach:
        case tok_while:    return loop();          break;
        case tok_if:       return cond();          break;
        case tok_continue: return continue_expr(); break;
        case tok_break:    return break_expr();    break;
        case tok_ret:      return ret_expr();      break;
        case tok_semi:                             break;
        default:
            die(err_line,"incorrect token <"+toks[ptr].str+">");
            next();
            break;
    }
    return {toks[ptr].line,toks[ptr].col,ast_null};
}
ast parse::exprs()
{
    if(lookahead(tok_eof))
    {
        die(err_line,"expected expression block");
        return null();
    }
    ast node(toks[ptr].line,toks[ptr].col,ast_block);
    if(lookahead(tok_lbrace))
    {
        match(tok_lbrace);
        while(!lookahead(tok_rbrace) && !lookahead(tok_eof))
        {
            node.add(expr());
            if(lookahead(tok_semi))
                match(tok_semi);
            // the last expression can be recognized without semi
            else if(need_semi_check(node.child().back()) && !lookahead(tok_rbrace))
                die(err_line,"expected ';'",true);
        }
        match(tok_rbrace,"expected '}' when generating expressions");
    }
    else
    {
        node.add(expr());
        if(lookahead(tok_semi))
            match(tok_semi);
    }
    return node;
}
ast parse::calc()
{
    ast node=or_expr();
    if(lookahead(tok_quesmark))
    {
        // trinocular calculation
        ast tmp(toks[ptr].line,toks[ptr].col,ast_trino);
        match(tok_quesmark);
        tmp.add(std::move(node));
        tmp.add(calc());
        match(tok_colon);
        tmp.add(calc());
        node=std::move(tmp);
    }
    else if(tok_eq<=toks[ptr].type && toks[ptr].type<=tok_lnkeq)
    {
        check_memory_reachable(node);
        // tok_eq~tok_lnkeq is 37 to 42,ast_equal~ast_lnkeq is 21~26
        ast tmp(toks[ptr].line,toks[ptr].col,toks[ptr].type-tok_eq+ast_equal);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(calc());
        node=std::move(tmp);
    }
    return node;
}
ast parse::or_expr()
{
    ast node=and_expr();
    while(lookahead(tok_or))
    {
        ast tmp(toks[ptr].line,toks[ptr].col,ast_or);
        tmp.add(std::move(node));
        match(tok_or);
        tmp.add(and_expr());
        node=std::move(tmp);
    }
    return node;
}
ast parse::and_expr()
{
    ast node=cmp_expr();
    while(lookahead(tok_and))
    {
        ast tmp(toks[ptr].line,toks[ptr].col,ast_and);
        tmp.add(std::move(node));
        match(tok_and);
        tmp.add(cmp_expr());
        node=std::move(tmp);
    }
    return node;
}
ast parse::cmp_expr()
{
    ast node=additive_expr();
    while(tok_cmpeq<=toks[ptr].type && toks[ptr].type<=tok_geq)
    {
        // tok_cmpeq~tok_geq is 43~48,ast_cmpeq~ast_geq is 27~32
        ast tmp(toks[ptr].line,toks[ptr].col,toks[ptr].type-tok_cmpeq+ast_cmpeq);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(additive_expr());
        node=std::move(tmp);
    }
    return node;
}
ast parse::additive_expr()
{
    ast node=multive_expr();
    while(lookahead(tok_add) || lookahead(tok_sub) || lookahead(tok_link))
    {
        ast tmp(toks[ptr].line,toks[ptr].col,ast_null);
        switch(toks[ptr].type)
        {
            case tok_add:  tmp.set_type(ast_add);  break;
            case tok_sub:  tmp.set_type(ast_sub);  break;
            case tok_link: tmp.set_type(ast_link); break;
        }
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add(multive_expr());
        node=std::move(tmp);
    }
    return node;
}
ast parse::multive_expr()
{
    ast node=(lookahead(tok_sub) || lookahead(tok_not))?unary():scalar();
    while(lookahead(tok_mult) || lookahead(tok_div))
    {
        ast tmp(toks[ptr].line,toks[ptr].col,toks[ptr].type-tok_mult+ast_mult);
        tmp.add(std::move(node));
        match(toks[ptr].type);
        tmp.add((lookahead(tok_sub) || lookahead(tok_not))?unary():scalar());
        node=std::move(tmp);
    }
    return node;
}
ast parse::unary()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_null);
    switch(toks[ptr].type)
    {
        case tok_sub:node.set_type(ast_neg);match(tok_sub);break;
        case tok_not:node.set_type(ast_not);match(tok_not);break;
    }
    node.add((lookahead(tok_sub) || lookahead(tok_not))?unary():scalar());
    return node;
}
ast parse::scalar()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_null);
    if(lookahead(tok_nil)) {node=nil();match(tok_nil);}
    else if(lookahead(tok_num)) node=num();
    else if(lookahead(tok_str)) node=str();
    else if(lookahead(tok_id)) node=id();
    else if(lookahead(tok_func)) node=func();
    else if(lookahead(tok_lbracket)) node=vec();
    else if(lookahead(tok_lbrace)) node=hash();
    else if(lookahead(tok_lcurve))
    {
        match(tok_lcurve);
        node=calc();
        match(tok_rcurve);
    }
    else if(lookahead(tok_var))
    {
        match(tok_var);
        node.set_type(ast_def);
        node.add(id());
        match(tok_eq);
        node.add(calc());
    }
    else
    {
        die(err_line,"expected scalar");
        return node;
    }
    // check call and avoid ambiguous syntax
    if(is_call(toks[ptr].type) && !(lookahead(tok_lcurve) && toks[ptr+1].type==tok_var))
    {
        ast tmp=std::move(node);
        node={toks[ptr].line,toks[ptr].col,ast_call};
        node.add(std::move(tmp));
        while(is_call(toks[ptr].type))
            node.add(call_scalar());
    }
    return node;
}
ast parse::call_scalar()
{
    switch(toks[ptr].type)
    {
        case tok_lcurve:   return callf(); break;
        case tok_lbracket: return callv(); break;
        case tok_dot:      return callh(); break;
    }
    // should never run this expression
    return {toks[ptr].line,toks[ptr].col,ast_nil};
}
ast parse::callh()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_callh);
    match(tok_dot);
    node.set_str(toks[ptr].str);
    match(tok_id,"expected hashmap key"); // get key
    return node;
}
ast parse::callv()
{
    // panic set for this token is not ','
    // this is the FIRST set of subvec
    // array end with tok_null=0
    const u32 panic_set[]={
        tok_id,tok_str,tok_num,
        tok_not,tok_sub,tok_nil,
        tok_func,tok_var,tok_lcurve,
        tok_lbrace,tok_lbracket,tok_colon,
        tok_null
    };
    ast node(toks[ptr].line,toks[ptr].col,ast_callv);
    match(tok_lbracket);
    while(!lookahead(tok_rbracket))
    {
        node.add(subvec());
        if(lookahead(tok_comma))
            match(tok_comma);
        else if(lookahead(tok_eof))
            break;
        else if(!lookahead(tok_rbracket) && !check_comma(panic_set))
            break;
    }
    if(node.size()==0)
        die(node.line(),"expected index value");
    match(tok_rbracket,"expected ']' when calling vector");
    return node;
}
ast parse::callf()
{
    // panic set for this token is not ','
    // this is the FIRST set of calculation/hashmember
    // array end with tok_null=0
    const u32 panic_set[]={
        tok_id,tok_str,tok_num,
        tok_not,tok_sub,tok_nil,
        tok_func,tok_var,tok_lcurve,
        tok_lbrace,tok_lbracket,tok_null
    };
    ast node(toks[ptr].line,toks[ptr].col,ast_callf);
    bool special_call=check_special_call();
    match(tok_lcurve);
    while(!lookahead(tok_rcurve))
    {
        node.add(special_call?pair():calc());
        if(lookahead(tok_comma))
            match(tok_comma);
        else if(lookahead(tok_eof))
            break;
        else if(!lookahead(tok_rcurve) && !check_comma(panic_set))
            break;
    }
    match(tok_rcurve,"expected ')' when calling function");
    return node;
}
ast parse::subvec()
{
    ast node=lookahead(tok_colon)?nil():calc();
    if(lookahead(tok_colon))
    {
        ast tmp(node.line(),node.col(),ast_subvec);
        match(tok_colon);
        tmp.add(std::move(node));
        tmp.add((lookahead(tok_comma) || lookahead(tok_rbracket))?nil():calc());
        node=std::move(tmp);
    }
    return node;
}
ast parse::definition()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_def);
    if(lookahead(tok_var))
    {
        match(tok_var);
        switch(toks[ptr].type)
        {
            case tok_id:     node.add(id());break;
            case tok_lcurve: node.add(outcurve_def());break;
            default:         die(err_line,"expected identifier");break;
        }
    }
    else if(lookahead(tok_lcurve))
        node.add(incurve_def());
    match(tok_eq);
    if(lookahead(tok_lcurve))
        node.add(check_tuple()?multi_scalar(false):calc());
    else
        node.add(calc());
    if(node[0].type()==ast_id && node[1].type()==ast_tuple)
        die(node[1].line(),"one variable cannot accept too many values");
    else if(node[0].type()==ast_multi_id && node[1].type()==ast_tuple && node[0].size()!=node[1].size())
        die(node[0].line(),"too much or lack values in multi-definition");
    return node;
}
ast parse::incurve_def()
{
    match(tok_lcurve);
    match(tok_var);
    ast node=multi_id();
    match(tok_rcurve);
    return node;
}
ast parse::outcurve_def()
{
    match(tok_lcurve);
    ast node=multi_id();
    match(tok_rcurve);
    return node;
}
ast parse::multi_id()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_multi_id);
    while(!lookahead(tok_eof))
    {
        node.add(id());
        if(is_call(toks[ptr].type))
        {
            call_scalar();// recognize calls but this is still a syntax error
            die(err_line,"cannot call identifier in multi-definition");
        }
        if(lookahead(tok_comma))
            match(tok_comma);
        else if(lookahead(tok_id))// first set of identifier
            die(err_line,"expected ',' between identifiers",true);
        else
            break;
    }
    return node;
}
ast parse::multi_scalar(bool check_call_memory)
{
    // if check_call_memory is true,we will check if value called here can reach a memory space
    const u32 panic_set[]={
        tok_id,tok_str,tok_num,
        tok_not,tok_sub,tok_nil,
        tok_func,tok_var,tok_lcurve,
        tok_lbrace,tok_lbracket,tok_null
    };
    ast node(toks[ptr].line,toks[ptr].col,ast_tuple);
    match(tok_lcurve);
    while(!lookahead(tok_rcurve))
    {
        node.add(calc());
        if(check_call_memory)
            check_memory_reachable(node.child().back());
        if(lookahead(tok_comma))
            match(tok_comma);
        else if(lookahead(tok_eof))
            break;
        else if(!lookahead(tok_rcurve) && !check_comma(panic_set))
            break;
    }
    match(tok_rcurve,"expected ')' after multi-scalar");
    return node;
}
ast parse::multi_assgin()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_multi_assign);
    node.add(multi_scalar(true));
    match(tok_eq);
    if(lookahead(tok_eof))
    {
        die(err_line,"expected value list");
        return node;
    }
    if(lookahead(tok_lcurve))
        node.add(check_tuple()?multi_scalar(false):calc());
    else
        node.add(calc());
    if(node[1].type()==ast_tuple
        && node[0].size()!=node[1].size())
        die(node[0].line(),"too much or lack values in multi-assignment");
    return node;
}
ast parse::loop()
{
    ++in_loop;
    ast node(0,0,ast_null);
    switch(toks[ptr].type)
    {
        case tok_while:   node=while_loop(); break;
        case tok_for:     node=for_loop();   break;
        case tok_forindex:
        case tok_foreach: node=forei_loop(); break;
    }
    --in_loop;
    return node;
}
ast parse::while_loop()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_while);
    match(tok_while);
    match(tok_lcurve);
    node.add(calc());
    match(tok_rcurve);
    node.add(exprs());
    return node;
}
ast parse::for_loop()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_for);
    match(tok_for);
    match(tok_lcurve);
    // first expression
    if(lookahead(tok_eof))
        die(err_line,"expected definition");
    if(lookahead(tok_semi))
        node.add(null());
    else if(lookahead(tok_var))
        node.add(definition());
    else if(lookahead(tok_lcurve))
        node.add(lcurve_expr());
    else
        node.add(calc());
    match(tok_semi,"expected ';' in for(;;)");
    // conditional expression
    if(lookahead(tok_eof))
        die(err_line,"expected conditional expr");
    if(lookahead(tok_semi))
        node.add(null());
    else
        node.add(calc());
    match(tok_semi,"expected ';' in for(;;)");
    //after loop expression
    if(lookahead(tok_eof))
        die(err_line,"expected calculation");
    if(lookahead(tok_rcurve))
        node.add(null());
    else
        node.add(calc());
    match(tok_rcurve);
    node.add(exprs());
    return node;
}
ast parse::forei_loop()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_null);
    switch(toks[ptr].type)
    {
        case tok_forindex:node.set_type(ast_forindex);match(tok_forindex);break;
        case tok_foreach: node.set_type(ast_foreach); match(tok_foreach); break;
    }
    match(tok_lcurve);
    // first expression
    // foreach/forindex must have an iterator to loop through
    if(!lookahead(tok_var) && !lookahead(tok_id))
        die(err_line,"expected iterator");
    node.add(iter_gen());
    match(tok_semi,"expected ';' in foreach/forindex(iter;vector)");
    if(lookahead(tok_eof))
        die(err_line,"expected vector");
    node.add(calc());
    match(tok_rcurve);
    node.add(exprs());
    return node;
}
ast parse::iter_gen()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_null);
    if(lookahead(tok_var))
    {
        match(tok_var);
        node.set_type(ast_iter);
        node.add(id());
    }
    else
    {
        node.set_type(ast_call);
        node.add(id());
        while(is_call(toks[ptr].type))
            node.add(call_scalar());
        check_memory_reachable(node);
    }
    return node;
}
ast parse::cond()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_cond);
    ast ifnode(toks[ptr].line,toks[ptr].col,ast_if);
    match(tok_if);
    match(tok_lcurve);
    ifnode.add(calc());
    match(tok_rcurve);
    ifnode.add(exprs());
    node.add(std::move(ifnode));
    while(lookahead(tok_elsif))
    {
        ast elsifnode(toks[ptr].line,toks[ptr].col,ast_elsif);
        match(tok_elsif);
        match(tok_lcurve);
        elsifnode.add(calc());
        match(tok_rcurve);
        elsifnode.add(exprs());
        node.add(std::move(elsifnode));
    }
    if(lookahead(tok_else))
    {
        ast elsenode(toks[ptr].line,toks[ptr].col,ast_else);
        match(tok_else);
        elsenode.add(exprs());
        node.add(std::move(elsenode));
    }
    return node;
}
ast parse::continue_expr()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_continue);
    match(tok_continue);
    return node;
}
ast parse::break_expr()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_break);
    match(tok_break);
    return node;
}
ast parse::ret_expr()
{
    ast node(toks[ptr].line,toks[ptr].col,ast_ret);
    match(tok_ret);
    u32 type=toks[ptr].type;
    if(type==tok_nil || type==tok_num ||
       type==tok_str || type==tok_id ||
       type==tok_func || type==tok_sub ||
       type==tok_not || type==tok_lcurve ||
       type==tok_lbracket || type==tok_lbrace)
        node.add(calc());
    return node;
}
