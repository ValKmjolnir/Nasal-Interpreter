#ifndef __NASAL_PARSE_H__
#define __NASAL_PARSE_H__

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

class nasal_parse
{
#define error_line (tokens[ptr].line)
#define is_call(type) ((type)==tok_lcurve || (type)==tok_lbracket || (type)==tok_dot)
private:
    u32 ptr;
    u32 in_func; // count function block
    u32 in_loop; // count loop block
    const token* tokens;// ref from nasal_lexer
    nasal_ast root;
    nasal_err& nerr;

    void die(u32,std::string,bool);
    void match(u32 type,const char* info=nullptr);
    bool check_comma(const u32*);
    bool check_multi_scalar();
    bool check_func_end(const nasal_ast&);
    bool check_special_call();
    bool need_semi_check(const nasal_ast&);
    void check_memory_reachable(const nasal_ast&);
    nasal_ast null();
    nasal_ast nil();
    nasal_ast num();
    nasal_ast str();
    nasal_ast id();
    nasal_ast vec();
    nasal_ast hash();
    nasal_ast pair();
    nasal_ast func();
    nasal_ast args();
    nasal_ast lcurve_expr();
    nasal_ast expr();
    nasal_ast exprs();
    nasal_ast calc();
    nasal_ast or_expr();
    nasal_ast and_expr();
    nasal_ast cmp_expr();
    nasal_ast additive_expr();
    nasal_ast multive_expr();
    nasal_ast unary();
    nasal_ast scalar();
    nasal_ast call_scalar();
    nasal_ast callh();
    nasal_ast callv();
    nasal_ast callf();
    nasal_ast subvec();
    nasal_ast definition();
    nasal_ast incurve_def();
    nasal_ast outcurve_def();
    nasal_ast multi_id();
    nasal_ast multi_scalar(bool);
    nasal_ast multi_assgin();
    nasal_ast loop();
    nasal_ast while_loop();
    nasal_ast for_loop();
    nasal_ast forei_loop();
    nasal_ast iter_gen();
    nasal_ast conditional();
    nasal_ast continue_expr();
    nasal_ast break_expr();
    nasal_ast ret_expr();
public:
    nasal_parse(nasal_err& e):ptr(0),in_func(0),in_loop(0),tokens(nullptr),nerr(e){}
    void print(){root.print(0);}
    void compile(const nasal_lexer&);
    nasal_ast& ast(){return root;}
    const nasal_ast& ast() const {return root;}
};
void nasal_parse::compile(const nasal_lexer& lexer)
{
    tokens=lexer.result().data();
    ptr=in_func=in_loop=0;

    root={1,ast_root};
    while(tokens[ptr].type!=tok_eof)
    {
        root.add(expr());
        if(tokens[ptr].type==tok_semi)
            match(tok_semi);
        // the last expression can be recognized without semi
        else if(need_semi_check(root.child().back()) && tokens[ptr].type!=tok_eof)
            die(error_line,"expected \";\"",true);
    }
    nerr.chkerr();
}
void nasal_parse::die(u32 line,std::string info,bool report_prev=false)
{
    int col=(int)tokens[ptr].col-(int)tokens[ptr].str.length();
    if(tokens[ptr].type==tok_str)
        col-=2; // tok_str's str has no \"
    if(report_prev && ptr) // used to report lack of ',' ';'
    {
        line=tokens[ptr-1].line;
        col=tokens[ptr-1].col+1;
    }
    nerr.err("parse",line,col<0?0:col,info);
}
void nasal_parse::match(u32 type,const char* info)
{
    if(tokens[ptr].type!=type)
    {
        if(info)
        {
            die(error_line,info);
            return;
        }
        switch(type)
        {
            case tok_num:die(error_line,"expected number");    break;
            case tok_str:die(error_line,"expected string");    break;
            case tok_id: die(error_line,"expected identifier");break;
            default:     die(error_line,"expected \'"+std::string(tok_table[type-tok_for].str)+"\'"); break;
        }
        return;
    }
    if(tokens[ptr].type==tok_eof)
        return;
    ++ptr;
}
bool nasal_parse::check_comma(const u32* panic_set)
{
    for(u32 i=0;panic_set[i];++i)
        if(tokens[ptr].type==panic_set[i])
        {
            die(error_line,"expected \',\' between scalars",true);
            return true;
        }
    return false;
}
bool nasal_parse::check_multi_scalar()
{
    u32 check_ptr=ptr,curve=1,bracket=0,brace=0;
    while(tokens[++check_ptr].type!=tok_eof && curve)
    {
        switch(tokens[check_ptr].type)
        {
            case tok_lcurve:   ++curve;   break;
            case tok_lbracket: ++bracket; break;
            case tok_lbrace:   ++brace;   break;
            case tok_rcurve:   --curve;   break;
            case tok_rbracket: --bracket; break;
            case tok_rbrace:   --brace;   break;
        }
        if(curve==1 && !bracket && !brace && tokens[check_ptr].type==tok_comma)
            return true;
    }
    return false;
}
bool nasal_parse::check_func_end(const nasal_ast& node)
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
bool nasal_parse::check_special_call()
{
    // special call means like this: function_name(a:1,b:2,c:3);
    u32 check_ptr=ptr,curve=1,bracket=0,brace=0;
    while(tokens[++check_ptr].type!=tok_eof && curve)
    {
        switch(tokens[check_ptr].type)
        {
            case tok_lcurve:   ++curve;  break;
            case tok_lbracket: ++bracket;break;
            case tok_lbrace:   ++brace;  break;
            case tok_rcurve:   --curve;  break;
            case tok_rbracket: --bracket;break;
            case tok_rbrace:   --brace;  break;
        }
        // m?1:0 will be recognized as normal parameter
        if(curve==1 && !bracket && !brace && tokens[check_ptr].type==tok_quesmark)
            return false;
        if(curve==1 && !bracket && !brace && tokens[check_ptr].type==tok_colon)
            return true;
    }
    return false;
}
bool nasal_parse::need_semi_check(const nasal_ast& node)
{
    u32 type=node.type();
    if(type==ast_for || type==ast_foreach || type==ast_forindex || type==ast_while || type==ast_conditional)
        return false;
    return !check_func_end(node);
}
void nasal_parse::check_memory_reachable(const nasal_ast& node)
{
    if(node.type()==ast_call)
    {
        const nasal_ast& tmp=node.child().back();
        if(tmp.type()==ast_callf)
            die(tmp.line(),"bad left-value");
        if(tmp.type()==ast_callv && (tmp.size()>1 || tmp[0].type()==ast_subvec))
            die(tmp.line(),"bad left-value");
    }
    else if(node.type()!=ast_id)
        die(node.line(),"bad left-value");
}
nasal_ast nasal_parse::null()
{
    return {tokens[ptr].line,ast_null};
}
nasal_ast nasal_parse::nil()
{
    return {tokens[ptr].line,ast_nil};
}
nasal_ast nasal_parse::num()
{
    nasal_ast node(tokens[ptr].line,ast_num);
    node.set_num(str2num(tokens[ptr].str.c_str()));
    return node;
}
nasal_ast nasal_parse::str()
{
    nasal_ast node(tokens[ptr].line,ast_str);
    node.set_str(tokens[ptr].str);
    return node;
}
nasal_ast nasal_parse::id()
{
    nasal_ast node(tokens[ptr].line,ast_id);
    node.set_str(tokens[ptr].str);
    return node;
}
nasal_ast nasal_parse::vec()
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
    nasal_ast node(tokens[ptr].line,ast_vec);
    match(tok_lbracket);
    while(tokens[ptr].type!=tok_rbracket)
    {
        node.add(calc());
        if(tokens[ptr].type==tok_comma)
            match(tok_comma);
        else if(tokens[ptr].type==tok_eof)
            break;
        else if(tokens[ptr].type!=tok_rbracket && !check_comma(panic_set))
            break;
    }
    match(tok_rbracket,"expected \']\' when generating vector");
    return node;
}
nasal_ast nasal_parse::hash()
{
    nasal_ast node(tokens[ptr].line,ast_hash);
    match(tok_lbrace);
    while(tokens[ptr].type!=tok_rbrace)
    {
        node.add(pair());
        if(tokens[ptr].type==tok_comma)
            match(tok_comma);
        else if(tokens[ptr].type==tok_id || tokens[ptr].type==tok_str)// first set of hashmember
            die(error_line,"expected \',\' between hash members",true);
        else
            break;
    }
    match(tok_rbrace,"expected \'}\' when generating hash");
    return node;
}
nasal_ast nasal_parse::pair()
{
    nasal_ast node(tokens[ptr].line,ast_pair);
    if(tokens[ptr].type==tok_id)
    {
        node.add(id());
        match(tok_id);
    }
    else if(tokens[ptr].type==tok_str)
    {
        node.add(str());
        match(tok_str);
    }
    else
        match(tok_id);
    match(tok_colon);
    node.add(calc());
    return node;
}
nasal_ast nasal_parse::func()
{
    nasal_ast node(tokens[ptr].line,ast_func);
    match(tok_func);
    if(tokens[ptr].type==tok_lcurve)
        node.add(args());
    else
        node.add(null());
    node.add(exprs());
    return node;
}
nasal_ast nasal_parse::args()
{
    nasal_ast node(tokens[ptr].line,ast_args);
    match(tok_lcurve);
    while(tokens[ptr].type!=tok_rcurve)
    {
        nasal_ast tmp=id();
        match(tok_id);
        if(tokens[ptr].type==tok_eq || tokens[ptr].type==tok_ellipsis)
        {
            nasal_ast special_arg(tokens[ptr].line,ast_null);
            if(tokens[ptr].type==tok_eq)
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
        if(tokens[ptr].type==tok_comma)
            match(tok_comma);
        else if(tokens[ptr].type==tok_id)// first set of identifier
            die(error_line,"expected \',\' between identifiers",true);
        else
            break;
    }
    match(tok_rcurve,"expected \')\' after parameter list");

    std::string format="func(";
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
    std::unordered_map<std::string,bool> argname;
    for(auto& tmp:node.child())
    {
        std::string name;
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
nasal_ast nasal_parse::lcurve_expr()
{
    if(tokens[ptr+1].type==tok_var)
        return definition();
    return check_multi_scalar()?multi_assgin():calc();
}
nasal_ast nasal_parse::expr()
{
    u32 type=tokens[ptr].type;
    if((type==tok_break || type==tok_continue) && !in_loop)
        die(error_line,"should use break/continue in loops");
    if(type==tok_ret && !in_func)
        die(error_line,"should use return in functions");
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
        case tok_if:       return conditional();   break;
        case tok_continue: return continue_expr(); break;
        case tok_break:    return break_expr();    break;
        case tok_ret:      return ret_expr();      break;
        case tok_semi:                             break;
        default:
            die(error_line,"incorrect token <"+tokens[ptr].str+">");
            ++ptr;
            break;
    }
    return {tokens[ptr].line,ast_null};
}
nasal_ast nasal_parse::exprs()
{
    if(tokens[ptr].type==tok_eof)
    {
        die(error_line,"expected expression block");
        return null();
    }
    nasal_ast node(tokens[ptr].line,ast_block);
    if(tokens[ptr].type==tok_lbrace)
    {
        match(tok_lbrace);
        while(tokens[ptr].type!=tok_rbrace && tokens[ptr].type!=tok_eof)
        {
            node.add(expr());
            if(tokens[ptr].type==tok_semi)
                match(tok_semi);
            // the last expression can be recognized without semi
            else if(need_semi_check(node.child().back()) && tokens[ptr].type!=tok_rbrace)
                die(error_line,"expected \';\'",true);
        }
        match(tok_rbrace,"expected \'}\' when generating expressions");
    }
    else
    {
        node.add(expr());
        if(tokens[ptr].type==tok_semi)
            match(tok_semi);
    }
    return node;
}
nasal_ast nasal_parse::calc()
{
    nasal_ast node=or_expr();
    if(tokens[ptr].type==tok_quesmark)
    {
        // trinocular calculation
        nasal_ast tmp(tokens[ptr].line,ast_trino);
        match(tok_quesmark);
        tmp.add(std::move(node));
        tmp.add(calc());
        match(tok_colon);
        tmp.add(calc());
        node=std::move(tmp);
    }
    else if(tok_eq<=tokens[ptr].type && tokens[ptr].type<=tok_lnkeq)
    {
        check_memory_reachable(node);
        // tok_eq~tok_lnkeq is 37 to 42,ast_equal~ast_lnkeq is 21~26
        nasal_ast tmp(tokens[ptr].line,tokens[ptr].type-tok_eq+ast_equal);
        tmp.add(std::move(node));
        match(tokens[ptr].type);
        tmp.add(calc());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::or_expr()
{
    nasal_ast node=and_expr();
    while(tokens[ptr].type==tok_or)
    {
        nasal_ast tmp(tokens[ptr].line,ast_or);
        tmp.add(std::move(node));
        match(tok_or);
        tmp.add(and_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::and_expr()
{
    nasal_ast node=cmp_expr();
    while(tokens[ptr].type==tok_and)
    {
        nasal_ast tmp(tokens[ptr].line,ast_and);
        tmp.add(std::move(node));
        match(tok_and);
        tmp.add(cmp_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::cmp_expr()
{
    nasal_ast node=additive_expr();
    while(tok_cmpeq<=tokens[ptr].type && tokens[ptr].type<=tok_geq)
    {
        // tok_cmpeq~tok_geq is 43~48,ast_cmpeq~ast_geq is 27~32
        nasal_ast tmp(tokens[ptr].line,tokens[ptr].type-tok_cmpeq+ast_cmpeq);
        tmp.add(std::move(node));
        match(tokens[ptr].type);
        tmp.add(additive_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::additive_expr()
{
    nasal_ast node=multive_expr();
    while(tokens[ptr].type==tok_add || tokens[ptr].type==tok_sub || tokens[ptr].type==tok_link)
    {
        nasal_ast tmp(tokens[ptr].line,ast_null);
        switch(tokens[ptr].type)
        {
            case tok_add:  tmp.set_type(ast_add);  break;
            case tok_sub:  tmp.set_type(ast_sub);  break;
            case tok_link: tmp.set_type(ast_link); break;
        }
        tmp.add(std::move(node));
        match(tokens[ptr].type);
        tmp.add(multive_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::multive_expr()
{
    nasal_ast node=(tokens[ptr].type==tok_sub || tokens[ptr].type==tok_not)?unary():scalar();
    while(tokens[ptr].type==tok_mult || tokens[ptr].type==tok_div)
    {
        nasal_ast tmp(tokens[ptr].line,tokens[ptr].type-tok_mult+ast_mult);
        tmp.add(std::move(node));
        match(tokens[ptr].type);
        tmp.add((tokens[ptr].type==tok_sub || tokens[ptr].type==tok_not)?unary():scalar());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::unary()
{
    nasal_ast node(tokens[ptr].line,ast_null);
    switch(tokens[ptr].type)
    {
        case tok_sub:node.set_type(ast_neg);match(tok_sub);break;
        case tok_not:node.set_type(ast_not);match(tok_not);break;
    }
    node.add((tokens[ptr].type==tok_sub || tokens[ptr].type==tok_not)?unary():scalar());
    return node;
}
nasal_ast nasal_parse::scalar()
{
    nasal_ast node(tokens[ptr].line,ast_null);
    if(tokens[ptr].type==tok_nil)     {node=nil(); match(tok_nil);}
    else if(tokens[ptr].type==tok_num){node=num(); match(tok_num);}
    else if(tokens[ptr].type==tok_str){node=str(); match(tok_str);}
    else if(tokens[ptr].type==tok_id) {node=id();  match(tok_id); }
    else if(tokens[ptr].type==tok_func)
    {
        ++in_func;
        node=func();
        --in_func;
    }
    else if(tokens[ptr].type==tok_lbracket)
        node=vec();
    else if(tokens[ptr].type==tok_lbrace)
        node=hash();
    else if(tokens[ptr].type==tok_lcurve)
    {
        match(tok_lcurve);
        node=calc();
        match(tok_rcurve);
    }
    else if(tokens[ptr].type==tok_var)
    {
        match(tok_var);
        node.set_type(ast_def);
        node.add(id());
        match(tok_id);
        match(tok_eq);
        node.add(calc());
    }
    else
    {
        die(error_line,"expected scalar");
        return node;
    }
    // check call and avoid ambiguous syntax
    if(is_call(tokens[ptr].type) && !(tokens[ptr].type==tok_lcurve && tokens[ptr+1].type==tok_var))
    {
        nasal_ast tmp=std::move(node);
        node={tokens[ptr].line,ast_call};
        node.add(std::move(tmp));
        while(is_call(tokens[ptr].type))
            node.add(call_scalar());
    }
    return node;
}
nasal_ast nasal_parse::call_scalar()
{
    switch(tokens[ptr].type)
    {
        case tok_lcurve:   return callf(); break;
        case tok_lbracket: return callv(); break;
        case tok_dot:      return callh(); break;
    }
    // should never run this expression
    return {tokens[ptr].line,ast_nil};
}
nasal_ast nasal_parse::callh()
{
    nasal_ast node(tokens[ptr].line,ast_callh);
    match(tok_dot);
    node.set_str(tokens[ptr].str);
    match(tok_id);
    return node;
}
nasal_ast nasal_parse::callv()
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
    nasal_ast node(tokens[ptr].line,ast_callv);
    match(tok_lbracket);
    while(tokens[ptr].type!=tok_rbracket)
    {
        node.add(subvec());
        if(tokens[ptr].type==tok_comma)
            match(tok_comma);
        else if(tokens[ptr].type==tok_eof)
            break;
        else if(tokens[ptr].type!=tok_rbracket && !check_comma(panic_set))
            break;
    }
    match(tok_rbracket,"expected \']\' when calling vector");
    return node;
}
nasal_ast nasal_parse::callf()
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
    nasal_ast node(tokens[ptr].line,ast_callf);
    bool special_call=check_special_call();
    match(tok_lcurve);
    while(tokens[ptr].type!=tok_rcurve)
    {
        node.add(special_call?pair():calc());
        if(tokens[ptr].type==tok_comma)
            match(tok_comma);
        else if(tokens[ptr].type==tok_eof)
            break;
        else if(tokens[ptr].type!=tok_rcurve && !check_comma(panic_set))
            break;
    }
    match(tok_rcurve,"expected \')\' when calling function");
    return node;
}
nasal_ast nasal_parse::subvec()
{
    nasal_ast node=tokens[ptr].type==tok_colon?nil():calc();
    if(tokens[ptr].type==tok_colon)
    {
        nasal_ast tmp(node.line(),ast_subvec);
        match(tok_colon);
        tmp.add(std::move(node));
        tmp.add((tokens[ptr].type==tok_comma || tokens[ptr].type==tok_rbracket)?nil():calc());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::definition()
{
    nasal_ast node(tokens[ptr].line,ast_def);
    if(tokens[ptr].type==tok_var)
    {
        match(tok_var);
        switch(tokens[ptr].type)
        {
            case tok_id:     node.add(id());match(tok_id);break;
            case tok_lcurve: node.add(outcurve_def());break;
            default:         die(error_line,"expected identifier");break;
        }
    }
    else if(tokens[ptr].type==tok_lcurve)
        node.add(incurve_def());
    match(tok_eq);
    if(tokens[ptr].type==tok_lcurve)
        node.add(check_multi_scalar()?multi_scalar(false):calc());
    else
        node.add(calc());
    if(node[0].type()==ast_id && node[1].type()==ast_multi_scalar)
        die(node[1].line(),"one identifier cannot accept too many values");
    else if(node[0].type()==ast_multi_id && node[1].type()==ast_multi_scalar)
        if(node[0].size()!=node[1].size())
            die(node[0].line(),"too much or lack values in multi-definition");
    return node;
}
nasal_ast nasal_parse::incurve_def()
{
    match(tok_lcurve);
    match(tok_var);
    nasal_ast node=multi_id();
    match(tok_rcurve);
    return node;
}
nasal_ast nasal_parse::outcurve_def()
{
    match(tok_lcurve);
    nasal_ast node=multi_id();
    match(tok_rcurve);
    return node;
}
nasal_ast nasal_parse::multi_id()
{
    nasal_ast node(tokens[ptr].line,ast_multi_id);
    while(tokens[ptr].type!=tok_eof)
    {
        node.add(id());
        match(tok_id);
        if(is_call(tokens[ptr].type))
        {
            call_scalar();// recognize calls but this is still a syntax error
            die(error_line,"cannot call identifier in multi-definition");
        }
        if(tokens[ptr].type==tok_comma)
            match(tok_comma);
        else if(tokens[ptr].type==tok_id)// first set of identifier
            die(error_line,"expected \',\' between identifiers",true);
        else
            break;
    }
    return node;
}
nasal_ast nasal_parse::multi_scalar(bool check_call_memory)
{
    // if check_call_memory is true,we will check if value called here can reach a memory space
    const u32 panic_set[]={
        tok_id,tok_str,tok_num,
        tok_not,tok_sub,tok_nil,
        tok_func,tok_var,tok_lcurve,
        tok_lbrace,tok_lbracket,tok_null
    };
    nasal_ast node(tokens[ptr].line,ast_multi_scalar);
    match(tok_lcurve);
    while(tokens[ptr].type!=tok_rcurve)
    {
        node.add(calc());
        if(check_call_memory)
            check_memory_reachable(node.child().back());
        if(tokens[ptr].type==tok_comma)
            match(tok_comma);
        else if(tokens[ptr].type==tok_eof)
            break;
        else if(tokens[ptr].type!=tok_rcurve && !check_comma(panic_set))
            break;
    }
    match(tok_rcurve,"expected \')\' after multi-scalar");
    return node;
}
nasal_ast nasal_parse::multi_assgin()
{
    nasal_ast node(tokens[ptr].line,ast_multi_assign);
    node.add(multi_scalar(true));
    match(tok_eq);
    if(tokens[ptr].type==tok_eof)
    {
        die(error_line,"expected value list");
        return node;
    }
    if(tokens[ptr].type==tok_lcurve)
        node.add(check_multi_scalar()?multi_scalar(false):calc());
    else
        node.add(calc());
    if(node[1].type()==ast_multi_scalar
        && node[0].size()!=node[1].size())
        die(node[0].line(),"too much or lack values in multi-assignment");
    return node;
}
nasal_ast nasal_parse::loop()
{
    ++in_loop;
    nasal_ast node;
    switch(tokens[ptr].type)
    {
        case tok_while:   node=while_loop(); break;
        case tok_for:     node=for_loop();   break;
        case tok_forindex:
        case tok_foreach: node=forei_loop(); break;
    }
    --in_loop;
    return node;
}
nasal_ast nasal_parse::while_loop()
{
    nasal_ast node(tokens[ptr].line,ast_while);
    match(tok_while);
    match(tok_lcurve);
    node.add(calc());
    match(tok_rcurve);
    node.add(exprs());
    return node;
}
nasal_ast nasal_parse::for_loop()
{
    nasal_ast node(tokens[ptr].line,ast_for);
    match(tok_for);
    match(tok_lcurve);
    // first expression
    if(tokens[ptr].type==tok_eof)
        die(error_line,"expected definition");
    if(tokens[ptr].type==tok_semi)
        node.add(null());
    else if(tokens[ptr].type==tok_var)
        node.add(definition());
    else if(tokens[ptr].type==tok_lcurve)
        node.add(lcurve_expr());
    else
        node.add(calc());
    match(tok_semi,"expected \';\' in for(;;)");
    // conditional expression
    if(tokens[ptr].type==tok_eof)
        die(error_line,"expected conditional expr");
    if(tokens[ptr].type==tok_semi)
        node.add(null());
    else
        node.add(calc());
    match(tok_semi,"expected \';\' in for(;;)");
    //after loop expression
    if(tokens[ptr].type==tok_eof)
        die(error_line,"expected calculation");
    if(tokens[ptr].type==tok_rcurve)
        node.add(null());
    else
        node.add(calc());
    match(tok_rcurve);
    node.add(exprs());
    return node;
}
nasal_ast nasal_parse::forei_loop()
{
    nasal_ast node(tokens[ptr].line,ast_null);
    switch(tokens[ptr].type)
    {
        case tok_forindex:node.set_type(ast_forindex);match(tok_forindex);break;
        case tok_foreach: node.set_type(ast_foreach); match(tok_foreach); break;
    }
    match(tok_lcurve);
    // first expression
    // foreach/forindex must have an iterator to loop through
    if(tokens[ptr].type!=tok_var && tokens[ptr].type!=tok_id)
        die(error_line,"expected iterator");
    node.add(iter_gen());
    match(tok_semi,"expected \';\' in foreach/forindex(iter;vector)");
    if(tokens[ptr].type==tok_eof)
        die(error_line,"expected vector");
    node.add(calc());
    match(tok_rcurve);
    node.add(exprs());
    return node;
}
nasal_ast nasal_parse::iter_gen()
{
    nasal_ast node(tokens[ptr].line,ast_null);
    if(tokens[ptr].type==tok_var)
    {
        match(tok_var);
        node.set_type(ast_iter);
        node.add(id());
        match(tok_id);
    }
    else
    {
        node.set_type(ast_call);
        node.add(id());
        match(tok_id);
        while(is_call(tokens[ptr].type))
            node.add(call_scalar());
        check_memory_reachable(node);
    }
    return node;
}
nasal_ast nasal_parse::conditional()
{
    nasal_ast node(tokens[ptr].line,ast_conditional);
    nasal_ast ifnode(tokens[ptr].line,ast_if);
    match(tok_if);
    match(tok_lcurve);
    ifnode.add(calc());
    match(tok_rcurve);
    ifnode.add(exprs());
    node.add(std::move(ifnode));
    while(tokens[ptr].type==tok_elsif)
    {
        nasal_ast elsifnode(tokens[ptr].line,ast_elsif);
        match(tok_elsif);
        match(tok_lcurve);
        elsifnode.add(calc());
        match(tok_rcurve);
        elsifnode.add(exprs());
        node.add(std::move(elsifnode));
    }
    if(tokens[ptr].type==tok_else)
    {
        nasal_ast elsenode(tokens[ptr].line,ast_else);
        match(tok_else);
        elsenode.add(exprs());
        node.add(std::move(elsenode));
    }
    return node;
}
nasal_ast nasal_parse::continue_expr()
{
    nasal_ast node(tokens[ptr].line,ast_continue);
    match(tok_continue);
    return node;
}
nasal_ast nasal_parse::break_expr()
{
    nasal_ast node(tokens[ptr].line,ast_break);
    match(tok_break);
    return node;
}
nasal_ast nasal_parse::ret_expr()
{
    nasal_ast node(tokens[ptr].line,ast_ret);
    match(tok_ret);
    u32 type=tokens[ptr].type;
    if(
        type==tok_nil ||
        type==tok_num ||
        type==tok_str ||
        type==tok_id ||
        type==tok_func ||
        type==tok_sub ||
        type==tok_not ||
        type==tok_lcurve ||
        type==tok_lbracket ||
        type==tok_lbrace)
        node.add(calc());
    return node;
}
#endif
