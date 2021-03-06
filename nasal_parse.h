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
#define error_line (tok_list[ptr].line)
#define is_call(type) ((type)==tok_lcurve || (type)==tok_lbracket || (type)==tok_dot)
private:
    int ptr;
    int error;
    nasal_ast root;
    std::vector<token> tok_list;
    std::vector<token> error_token;
    int in_function; // count when generating function block,used to check return-expression
    int in_loop;     // count when generating loop block,used to check break/continue-expression
    void die(int,std::string&&);
    void match(int type,const char* err_info="");
    bool check_comma(int*);
    bool check_multi_def();
    bool check_multi_scalar();
    bool check_function_end(nasal_ast&);
    bool check_special_call();
    bool need_semi_check(nasal_ast&);
    void check_memory_reachable(nasal_ast&);
    nasal_ast null_node_gen();
    nasal_ast nil_gen();
    nasal_ast num_gen();
    nasal_ast str_gen();
    nasal_ast id_gen();
    nasal_ast vec_gen();
    nasal_ast hash_gen();
    nasal_ast hmem_gen();
    nasal_ast func_gen();
    nasal_ast args_gen();
    nasal_ast expr();
    nasal_ast exprs_gen();
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
    nasal_ast var_incurve_def();
    nasal_ast var_outcurve_def();
    nasal_ast multi_id();
    nasal_ast multi_scalar(bool);
    nasal_ast multi_assgin();
    nasal_ast loop();
    nasal_ast while_loop();
    nasal_ast for_loop();
    nasal_ast forei_loop();
    nasal_ast new_iter_gen();
    nasal_ast conditional();
    nasal_ast continue_expr();
    nasal_ast break_expr();
    nasal_ast ret_expr();
public:
    int get_error(){return error;}
    void set_toklist(std::vector<token>& toks){tok_list=toks;}
    void main_process();
    nasal_ast& get_root(){return root;}
};
void nasal_parse::main_process()
{
    ptr=in_function=in_loop=error=0;
    root.clear();
    error_token.clear();

    root.set_line(1);
    root.set_type(ast_root);
    while(tok_list[ptr].type!=tok_eof)
    {
        int err_tok_size=error_token.size();
        root.add_child(expr());
        if(tok_list[ptr].type==tok_semi)
            match(tok_semi);
        // if detect error token, avoid checking semicolon
        else if(error_token.size()>err_tok_size)
            continue;
        // the last expression can be recognized without semi
        else if(need_semi_check(root.get_children().back()) && tok_list[ptr].type!=tok_eof)
            die(error_line,"expected \";\"");
    }
    if(!error_token.size())
        return;
    ++error;
    std::cout<<">> [parse] line";
    int err_line=0;
    for(auto& tok:error_token)
        if(err_line!=tok.line)
        {
            std::cout<<' '<<tok.line;
            err_line=tok.line;
        }
    std::cout
    <<" have fatal syntax errors."
    <<"check \'(\',\'[\',\'{\',\')\',\']\',\'}\' match or not.\n";
    return;
}
void nasal_parse::die(int line,std::string&& info)
{
    ++error;
    std::cout<<">> [parse] line "<<line<<": "<<info<<".\n";
    return;
}
void nasal_parse::match(int type,const char* err_info)
{
    if(tok_list[ptr].type!=type)
    {
        if(err_info[0])
        {
            die(error_line,err_info);
            return;
        }
        switch(type)
        {
            case tok_num:die(error_line,"expected number");    break;
            case tok_str:die(error_line,"expected string");    break;
            case tok_id: die(error_line,"expected identifier");break;
            default:     die(error_line,"expected \'"+std::string(token_table[type-tok_for].str)+"\'"); break;
        }
        return;
    }
    if(tok_list[ptr].type==tok_eof)
        return;
    ++ptr;
    return;
}
bool nasal_parse::check_comma(int* panic_set)
{
    for(int i=0;panic_set[i];++i)
        if(tok_list[ptr].type==panic_set[i])
        {
            die(error_line,"expected \',\' between scalars");
            return true;
        }
    return false;
}
bool nasal_parse::check_multi_def()
{
    return tok_list[ptr+1].type==tok_var;
}
bool nasal_parse::check_multi_scalar()
{
    int check_ptr=ptr,curve_cnt=1,bracket_cnt=0,brace_cnt=0;
    while(tok_list[++check_ptr].type!=tok_eof && curve_cnt)
    {
        switch(tok_list[check_ptr].type)
        {
            case tok_lcurve:   ++curve_cnt;   break;
            case tok_lbracket: ++bracket_cnt; break;
            case tok_lbrace:   ++brace_cnt;   break;
            case tok_rcurve:   --curve_cnt;   break;
            case tok_rbracket: --bracket_cnt; break;
            case tok_rbrace:   --brace_cnt;   break;
        }
        if(curve_cnt==1 && !bracket_cnt && !brace_cnt && tok_list[check_ptr].type==tok_comma)
            return true;
    }
    return false;
}
bool nasal_parse::check_function_end(nasal_ast& node)
{
    int type=node.get_type();
    if(type==ast_func)
        return true;
    else if(type==ast_num || type==ast_id || type==ast_str || type==ast_nil || type==ast_vec || type==ast_hash)
        return false;
    if(
        node.get_children().empty() || 
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
        return check_function_end(node.get_children().back());
    return false;
}
bool nasal_parse::check_special_call()
{
    // special call means like this:
    // function_name(a:1,b:2,c:3);
    int check_ptr=ptr,curve_cnt=1,bracket_cnt=0,brace_cnt=0,ques_cnt=0;
    while(tok_list[++check_ptr].type!=tok_eof && curve_cnt)
    {
        switch(tok_list[check_ptr].type)
        {
            case tok_lcurve:   ++curve_cnt;  break;
            case tok_lbracket: ++bracket_cnt;break;
            case tok_lbrace:   ++brace_cnt;  break;
            case tok_rcurve:   --curve_cnt;  break;
            case tok_rbracket: --bracket_cnt;break;
            case tok_rbrace:   --brace_cnt;  break;
        }
        // m?1:0 will be recognized as normal parameter
        if(curve_cnt==1 && !bracket_cnt && !brace_cnt && tok_list[check_ptr].type==tok_quesmark)
            return false;
        if(curve_cnt==1 && !bracket_cnt && !brace_cnt && tok_list[check_ptr].type==tok_colon)
            return true;
    }
    return false;
}
bool nasal_parse::need_semi_check(nasal_ast& node)
{
    int type=node.get_type();
    if(type==ast_for || type==ast_foreach || type==ast_forindex || type==ast_while || type==ast_conditional)
        return false;
    return !check_function_end(node);
}
void nasal_parse::check_memory_reachable(nasal_ast& node)
{
    if(node.get_type()==ast_call)
    {
        nasal_ast& tmp=node.get_children().back();
        if(tmp.get_type()==ast_callf)
            die(tmp.get_line(),"bad left-value");
        if(tmp.get_type()==ast_callv && (tmp.get_children().size()>1 || tmp.get_children()[0].get_type()==ast_subvec))
            die(tmp.get_line(),"bad left-value");
    }
    else if(node.get_type()!=ast_id)
        die(node.get_line(),"bad left-value");
    return;
}
nasal_ast nasal_parse::null_node_gen()
{
    return nasal_ast(tok_list[ptr].line,ast_null);
}
nasal_ast nasal_parse::nil_gen()
{
    return nasal_ast(tok_list[ptr].line,ast_nil);
}
nasal_ast nasal_parse::num_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_num);
    node.set_num(str2num(tok_list[ptr].str.c_str()));
    return node;
}
nasal_ast nasal_parse::str_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_str);
    node.set_str(tok_list[ptr].str);
    return node;
}
nasal_ast nasal_parse::id_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_id);
    node.set_str(tok_list[ptr].str);
    return node;
}
nasal_ast nasal_parse::vec_gen()
{
    // panic set for this token is not ','
    // this is the FIRST set of calculation
    // array end with tok_null=0
    int panic_set[]={tok_id,tok_str,tok_num,tok_not,tok_sub,tok_nil,tok_func,tok_var,tok_lcurve,tok_lbrace,tok_lbracket,tok_null};
    nasal_ast node(tok_list[ptr].line,ast_vec);
    match(tok_lbracket);
    while(tok_list[ptr].type!=tok_rbracket)
    {
        node.add_child(calc());
        if(tok_list[ptr].type==tok_comma)
            match(tok_comma);
        else if(tok_list[ptr].type==tok_eof)
            break;
        else if(tok_list[ptr].type!=tok_rbracket && !check_comma(panic_set))
            break;
    }
    match(tok_rbracket,"expected \']\' when generating vector");
    return node;
}
nasal_ast nasal_parse::hash_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_hash);
    match(tok_lbrace);
    while(tok_list[ptr].type!=tok_rbrace)
    {
        node.add_child(hmem_gen());
        if(tok_list[ptr].type==tok_comma)
            match(tok_comma);
        else if(tok_list[ptr].type==tok_id || tok_list[ptr].type==tok_str)// first set of hashmember
            die(error_line,"expected \',\' between hash members");
        else
            break;
    }
    match(tok_rbrace,"expected \'}\' when generating hash");
    return node;
}
nasal_ast nasal_parse::hmem_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_hashmember);
    if(tok_list[ptr].type==tok_id)
    {
        node.add_child(id_gen());
        match(tok_id);
    }
    else if(tok_list[ptr].type==tok_str)
    {
        node.add_child(str_gen());
        match(tok_str);
    }
    else
        match(tok_id);
    match(tok_colon);
    node.add_child(calc());
    return node;
}
nasal_ast nasal_parse::func_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_func);
    match(tok_func);
    if(tok_list[ptr].type==tok_lcurve)
        node.add_child(args_gen());
    else
        node.add_child(null_node_gen());
    node.add_child(exprs_gen());
    return node;
}
nasal_ast nasal_parse::args_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_args);
    match(tok_lcurve);
    while(tok_list[ptr].type!=tok_rcurve)
    {
        nasal_ast tmp=id_gen();
        match(tok_id);
        if(tok_list[ptr].type==tok_eq || tok_list[ptr].type==tok_ellipsis)
        {
            nasal_ast special_arg(tok_list[ptr].line,ast_null);
            if(tok_list[ptr].type==tok_eq)
            {
                match(tok_eq);
                special_arg=std::move(tmp);
                special_arg.set_type(ast_default_arg);
                special_arg.add_child(calc());
            }
            else
            {
                match(tok_ellipsis);
                special_arg=std::move(tmp);
                special_arg.set_type(ast_dynamic_id);
            }
            node.add_child(std::move(special_arg));
        }
        else
            node.add_child(std::move(tmp));
        if(tok_list[ptr].type==tok_comma)
            match(tok_comma);
        else if(tok_list[ptr].type==tok_id)// first set of identifier
            die(error_line,"expected \',\' between identifiers");
        else
            break;
    }
    match(tok_rcurve,"expected \')\' after parameter list");

    std::string args_format="func(";
    for(auto& tmp:node.get_children())
    {
        switch(tmp.get_type())
        {
            case ast_id:          args_format+=tmp.get_str();break;
            case ast_default_arg: args_format+=tmp.get_str()+"=val";break;
            case ast_dynamic_id:  args_format+=tmp.get_str()+"...";break;
        }
        args_format+=",)"[&tmp==&node.get_children().back()];
    }
    bool checked_default_val=false,checked_dynamic_ids=false;
    for(auto& tmp:node.get_children())
    {
        if(tmp.get_type()==ast_default_arg)
            checked_default_val=true;
        else if(tmp.get_type()==ast_dynamic_id)
            checked_dynamic_ids=true;
        if(checked_default_val && tmp.get_type()!=ast_default_arg)
            die(tmp.get_line(),"must use default paras after using it once: "+args_format);
        if(checked_dynamic_ids && &tmp!=&node.get_children().back())
            die(tmp.get_line(),"dynamic para must be the end: "+args_format);
    }
    std::unordered_map<std::string,bool> argname_table;
    for(auto& tmp:node.get_children())
    {
        std::string new_name;
        switch(tmp.get_type())
        {
            case ast_dynamic_id:
            case ast_id:         new_name=tmp.get_str();break;
            case ast_default_arg:new_name=tmp.get_str();break;
        }
        if(argname_table.count(new_name))
            die(tmp.get_line(),"parameter's name repeats: "+new_name);
        else
            argname_table[new_name]=true;
    }
    return node;
}
nasal_ast nasal_parse::expr()
{
    nasal_ast node(tok_list[ptr].line,ast_null);
    int tok_type=tok_list[ptr].type;
    if((tok_type==tok_break || tok_type==tok_continue) && !in_loop)
        die(error_line,"use break/continue in the loop");
    if(tok_type==tok_ret && !in_function)
        die(error_line,"use return in the function");
    switch(tok_type)
    {
        case tok_nil:
        case tok_num:
        case tok_str:
        case tok_id:
        case tok_func:
        case tok_lbracket:
        case tok_lbrace:
        case tok_sub:
        case tok_not:        node=calc();          break;
        case tok_var:        node=definition();    break;
        case tok_lcurve:     node=(check_multi_def()?definition():(check_multi_scalar()?multi_assgin():calc()));break;
        case tok_for:
        case tok_forindex:
        case tok_foreach:
        case tok_while:      node=loop();          break;
        case tok_if:         node=conditional();   break;
        case tok_continue:   node=continue_expr(); break;
        case tok_break:      node=break_expr();    break;
        case tok_ret:        node=ret_expr();      break;
        case tok_semi:                             break;
        default:error_token.push_back(tok_list[ptr]);++ptr;break;
    }
    return node;
}
nasal_ast nasal_parse::exprs_gen()
{
    if(tok_list[ptr].type==tok_eof)
    {
        die(error_line,"expected expression block");
        return null_node_gen();
    }
    nasal_ast node(tok_list[ptr].line,ast_block);
    if(tok_list[ptr].type==tok_lbrace)
    {
        match(tok_lbrace);
        while(tok_list[ptr].type!=tok_rbrace && tok_list[ptr].type!=tok_eof)
        {
            int err_tok_size=error_token.size();
            node.add_child(expr());
            if(tok_list[ptr].type==tok_semi)
                match(tok_semi);
            // if detect error token, avoid checking semicolon
            else if(error_token.size()>err_tok_size)
                continue;
            // the last expression can be recognized without semi
            else if(need_semi_check(node.get_children().back()) && tok_list[ptr].type!=tok_rbrace)
                die(error_line,"expected \";\"");
        }
        match(tok_rbrace,"expected \'}\' when generating expressions");
    }
    else
    {
        node.add_child(expr());
        if(tok_list[ptr].type==tok_semi)
            match(tok_semi);
    }
    return node;
}
nasal_ast nasal_parse::calc()
{
    nasal_ast node=or_expr();
    if(tok_list[ptr].type==tok_quesmark)
    {
        // trinocular calculation
        nasal_ast tmp(tok_list[ptr].line,ast_trino);
        match(tok_quesmark);
        tmp.add_child(std::move(node));
        tmp.add_child(calc());
        match(tok_colon);
        tmp.add_child(calc());
        node=std::move(tmp);
    }
    else if(tok_eq<=tok_list[ptr].type && tok_list[ptr].type<=tok_lnkeq)
    {
        check_memory_reachable(node);
        // tok_eq~tok_lnkeq is 37 to 42,ast_equal~ast_lnkeq is 21~26
        nasal_ast tmp(tok_list[ptr].line,tok_list[ptr].type-tok_eq+ast_equal);
        tmp.add_child(std::move(node));
        match(tok_list[ptr].type);
        tmp.add_child(calc());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::or_expr()
{
    nasal_ast node=and_expr();
    while(tok_list[ptr].type==tok_or)
    {
        nasal_ast tmp(tok_list[ptr].line,ast_or);
        tmp.add_child(std::move(node));
        match(tok_or);
        tmp.add_child(and_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::and_expr()
{
    nasal_ast node=cmp_expr();
    while(tok_list[ptr].type==tok_and)
    {
        nasal_ast tmp(tok_list[ptr].line,ast_and);
        tmp.add_child(std::move(node));
        match(tok_and);
        tmp.add_child(cmp_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::cmp_expr()
{
    nasal_ast node=additive_expr();
    while(tok_cmpeq<=tok_list[ptr].type && tok_list[ptr].type<=tok_geq)
    {
        // tok_cmpeq~tok_geq is 43~48,ast_cmpeq~ast_geq is 27~32
        nasal_ast tmp(tok_list[ptr].line,tok_list[ptr].type-tok_cmpeq+ast_cmpeq);
        tmp.add_child(std::move(node));
        match(tok_list[ptr].type);
        tmp.add_child(additive_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::additive_expr()
{
    nasal_ast node=multive_expr();
    while(tok_list[ptr].type==tok_add || tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_link)
    {
        nasal_ast tmp(tok_list[ptr].line,ast_null);
        switch(tok_list[ptr].type)
        {
            case tok_add:  tmp.set_type(ast_add);  break;
            case tok_sub:  tmp.set_type(ast_sub);  break;
            case tok_link: tmp.set_type(ast_link); break;
        }
        tmp.add_child(std::move(node));
        match(tok_list[ptr].type);
        tmp.add_child(multive_expr());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::multive_expr()
{
    nasal_ast node=(tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not)?unary():scalar();
    while(tok_list[ptr].type==tok_mult || tok_list[ptr].type==tok_div)
    {
        nasal_ast tmp(tok_list[ptr].line,tok_list[ptr].type-tok_mult+ast_mult);
        tmp.add_child(std::move(node));
        match(tok_list[ptr].type);
        tmp.add_child((tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not)?unary():scalar());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::unary()
{
    nasal_ast node(tok_list[ptr].line,ast_null);
    switch(tok_list[ptr].type)
    {
        case tok_sub:node.set_type(ast_neg);match(tok_sub);break;
        case tok_not:node.set_type(ast_not);match(tok_not);break;
    }
    node.add_child((tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not)?unary():scalar());
    return node;
}
nasal_ast nasal_parse::scalar()
{
    nasal_ast node(tok_list[ptr].line,ast_null);
    if(tok_list[ptr].type==tok_nil)     {node=nil_gen(); match(tok_nil);}
    else if(tok_list[ptr].type==tok_num){node=num_gen(); match(tok_num);}
    else if(tok_list[ptr].type==tok_str){node=str_gen(); match(tok_str);}
    else if(tok_list[ptr].type==tok_id) {node=id_gen();  match(tok_id); }
    else if(tok_list[ptr].type==tok_func)
    {
        if(tok_list[ptr+1].type==tok_id)
        {
            match(tok_func);
            node=id_gen();
            match(tok_id);
        }
        else
        {
            ++in_function;
            node=func_gen();
            --in_function;
        }
    }
    else if(tok_list[ptr].type==tok_lbracket)
        node=vec_gen();
    else if(tok_list[ptr].type==tok_lbrace)
        node=hash_gen();
    else if(tok_list[ptr].type==tok_lcurve)
    {
        match(tok_lcurve);
        node=calc();
        match(tok_rcurve);
    }
    else if(tok_list[ptr].type==tok_var)
    {
        match(tok_var);
        node.set_type(ast_def);
        node.add_child(id_gen());
        match(tok_id);
        match(tok_eq);
        node.add_child(calc());
    }
    else
    {
        die(error_line,"expected scalar");
        return node;
    }
    if(is_call(tok_list[ptr].type))
    {
        nasal_ast tmp=std::move(node);
        node.set_line(tok_list[ptr].line);
        node.set_type(ast_call);
        node.add_child(std::move(tmp));
    }
    while(is_call(tok_list[ptr].type))
        node.add_child(call_scalar());
    return node;
}
nasal_ast nasal_parse::call_scalar()
{
    switch(tok_list[ptr].type)
    {
        case tok_lcurve:   return callf(); break;
        case tok_lbracket: return callv(); break;
        case tok_dot:      return callh(); break;
    }
    // should never run this expression
    return nasal_ast(tok_list[ptr].line,ast_nil);
}
nasal_ast nasal_parse::callh()
{
    nasal_ast node(tok_list[ptr].line,ast_callh);
    match(tok_dot);
    node.set_str(tok_list[ptr].str);
    match(tok_id);
    return node;
}
nasal_ast nasal_parse::callv()
{
    // panic set for this token is not ','
    // this is the FIRST set of subvec
    // array end with tok_null=0
    int panic_set[]={tok_id,tok_str,tok_num,tok_not,tok_sub,tok_nil,tok_func,tok_var,tok_lcurve,tok_lbrace,tok_lbracket,tok_colon,tok_null};
    nasal_ast node(tok_list[ptr].line,ast_callv);
    match(tok_lbracket);
    while(tok_list[ptr].type!=tok_rbracket)
    {
        node.add_child(subvec());
        if(tok_list[ptr].type==tok_comma)
            match(tok_comma);
        else if(tok_list[ptr].type==tok_eof)
            break;
        else if(tok_list[ptr].type!=tok_rbracket && !check_comma(panic_set))
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
    int panic_set[]={tok_id,tok_str,tok_num,tok_not,tok_sub,tok_nil,tok_func,tok_var,tok_lcurve,tok_lbrace,tok_lbracket,tok_null};
    nasal_ast node(tok_list[ptr].line,ast_callf);
    bool special_call=check_special_call();
    match(tok_lcurve);
    while(tok_list[ptr].type!=tok_rcurve)
    {
        node.add_child(special_call?hmem_gen():calc());
        if(tok_list[ptr].type==tok_comma)
            match(tok_comma);
        else if(tok_list[ptr].type==tok_eof)
            break;
        else if(tok_list[ptr].type!=tok_rcurve && !check_comma(panic_set))
            break;
    }
    match(tok_rcurve,"expected \')\' when calling function");
    return node;
}
nasal_ast nasal_parse::subvec()
{
    nasal_ast node=tok_list[ptr].type==tok_colon?nil_gen():calc();
    if(tok_list[ptr].type==tok_colon)
    {
        nasal_ast tmp(node.get_line(),ast_subvec);
        match(tok_colon);
        tmp.add_child(std::move(node));
        tmp.add_child((tok_list[ptr].type==tok_comma || tok_list[ptr].type==tok_rbracket)?nil_gen():calc());
        node=std::move(tmp);
    }
    return node;
}
nasal_ast nasal_parse::definition()
{
    nasal_ast node(tok_list[ptr].line,ast_def);
    if(tok_list[ptr].type==tok_var)
    {
        match(tok_var);
        switch(tok_list[ptr].type)
        {
            case tok_id:     node.add_child(id_gen());match(tok_id); break;
            case tok_lcurve: node.add_child(var_outcurve_def());     break;
            default:         die(error_line,"expected identifier");  break;
        }
    }
    else if(tok_list[ptr].type==tok_lcurve)
        node.add_child(var_incurve_def());
    match(tok_eq);
    if(tok_list[ptr].type==tok_lcurve)
        node.add_child(check_multi_scalar()?multi_scalar(false):calc());
    else
        node.add_child(calc());
    if(node.get_children()[0].get_type()==ast_id && node.get_children()[1].get_type()==ast_multi_scalar)
        die(node.get_children()[1].get_line(),"one identifier cannot accept too many values");
    else if(node.get_children()[0].get_type()==ast_multi_id && node.get_children()[1].get_type()==ast_multi_scalar)
        if(node.get_children()[0].get_children().size()!=node.get_children()[1].get_children().size())
            die(node.get_children()[0].get_line(),"too much or lack values in multi-definition");
    return node;
}
nasal_ast nasal_parse::var_incurve_def()
{
    match(tok_lcurve);
    match(tok_var);
    nasal_ast node=multi_id();
    match(tok_rcurve);
    return node;
}
nasal_ast nasal_parse::var_outcurve_def()
{
    match(tok_lcurve);
    nasal_ast node=multi_id();
    match(tok_rcurve);
    return node;
}
nasal_ast nasal_parse::multi_id()
{
    nasal_ast node(tok_list[ptr].line,ast_multi_id);
    while(tok_list[ptr].type!=tok_eof)
    {
        node.add_child(id_gen());
        match(tok_id);
        if(is_call(tok_list[ptr].type))
        {
            call_scalar();// recognize calls but this is still a syntax error
            die(error_line,"cannot call identifier in multi-definition");
        }
        if(tok_list[ptr].type==tok_comma)
            match(tok_comma);
        else if(tok_list[ptr].type==tok_id)// first set of identifier
            die(error_line,"expected \',\' between identifiers");
        else
            break;
    }
    return node;
}
nasal_ast nasal_parse::multi_scalar(bool check_call_memory)
{
    // if check_call_memory is true,we will check if value called here can reach a memory space
    int panic_set[]={tok_id,tok_str,tok_num,tok_not,tok_sub,tok_nil,tok_func,tok_var,tok_lcurve,tok_lbrace,tok_lbracket,tok_null};
    nasal_ast node(tok_list[ptr].line,ast_multi_scalar);
    match(tok_lcurve);
    while(tok_list[ptr].type!=tok_rcurve)
    {
        node.add_child(calc());
        if(check_call_memory)
            check_memory_reachable(node.get_children().back());
        if(tok_list[ptr].type==tok_comma)
            match(tok_comma);
        else if(tok_list[ptr].type==tok_eof)
            break;
        else if(tok_list[ptr].type!=tok_rcurve && !check_comma(panic_set))
            break;
    }
    match(tok_rcurve,"expected \')\' after multi-scalar");
    return node;
}
nasal_ast nasal_parse::multi_assgin()
{
    nasal_ast node(tok_list[ptr].line,ast_multi_assign);
    node.add_child(multi_scalar(true));
    match(tok_eq);
    if(tok_list[ptr].type==tok_eof)
    {
        die(error_line,"expected value list");
        return node;
    }
    if(tok_list[ptr].type==tok_lcurve)
        node.add_child(check_multi_scalar()?multi_scalar(false):calc());
    else
        node.add_child(calc());
    if(node.get_children()[1].get_type()==ast_multi_scalar
        && node.get_children()[0].get_children().size()!=node.get_children()[1].get_children().size())
        die(node.get_children()[0].get_line(),"too much or lack values in multi-assignment");
    return node;
}
nasal_ast nasal_parse::loop()
{
    ++in_loop;
    nasal_ast node;
    switch(tok_list[ptr].type)
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
    nasal_ast node(tok_list[ptr].line,ast_while);
    match(tok_while);
    match(tok_lcurve);
    node.add_child(calc());
    match(tok_rcurve);
    node.add_child(exprs_gen());
    return node;
}
nasal_ast nasal_parse::for_loop()
{
    nasal_ast node(tok_list[ptr].line,ast_for);
    match(tok_for);
    match(tok_lcurve);
    // first expression
    if(tok_list[ptr].type==tok_eof)
        die(error_line,"expected definition");
    if(tok_list[ptr].type==tok_semi)
        node.add_child(null_node_gen());
    else if(tok_list[ptr].type==tok_var)
        node.add_child(definition());
    else if(tok_list[ptr].type==tok_lcurve)
        node.add_child(
            check_multi_def()?
                definition():
                (
                    check_multi_scalar()?
                    multi_assgin():
                    calc()
                )
        );
    else
        node.add_child(calc());
    // check first semi
    match(tok_semi,"expected \';\' in for(;;)");
    // conditional expression
    if(tok_list[ptr].type==tok_eof)
        die(error_line,"expected conditional expr");
    if(tok_list[ptr].type==tok_semi)
        node.add_child(null_node_gen());
    else
        node.add_child(calc());
    // check second semi
    match(tok_semi,"expected \';\' in for(;;)");
    //after loop expression
    if(tok_list[ptr].type==tok_eof)
        die(error_line,"expected calculation");
    if(tok_list[ptr].type==tok_rcurve)
        node.add_child(null_node_gen());
    else
        node.add_child(calc());
    match(tok_rcurve);
    node.add_child(exprs_gen());
    return node;
}
nasal_ast nasal_parse::forei_loop()
{
    nasal_ast node(tok_list[ptr].line,ast_null);
    switch(tok_list[ptr].type)
    {
        case tok_forindex: node.set_type(ast_forindex);match(tok_forindex); break;
        case tok_foreach:  node.set_type(ast_foreach); match(tok_foreach);  break;
    }
    match(tok_lcurve);
    // first expression
    // foreach/forindex must have an iterator to loop through
    if(tok_list[ptr].type!=tok_var && tok_list[ptr].type!=tok_id)
        die(error_line,"expected iterator");
    node.add_child(new_iter_gen());
    // check semi
    match(tok_semi,"expected \';\' in foreach/forindex(iter;vector)");
    // check vector
    if(tok_list[ptr].type==tok_eof)
        die(error_line,"expected vector");
    node.add_child(calc());
    match(tok_rcurve);
    node.add_child(exprs_gen());
    return node;
}
nasal_ast nasal_parse::new_iter_gen()
{
    nasal_ast node(tok_list[ptr].line,ast_null);
    if(tok_list[ptr].type==tok_var)
    {
        match(tok_var);
        node.set_type(ast_new_iter);
        node.add_child(id_gen());
        match(tok_id);
    }
    else
    {
        node.set_type(ast_call);
        node.add_child(id_gen());
        match(tok_id);
        while(is_call(tok_list[ptr].type))
            node.add_child(call_scalar());
    }
    return node;
}
nasal_ast nasal_parse::conditional()
{
    nasal_ast node(tok_list[ptr].line,ast_conditional);
    nasal_ast tmp(tok_list[ptr].line,ast_if);
    match(tok_if);
    match(tok_lcurve);
    tmp.add_child(calc());
    match(tok_rcurve);
    tmp.add_child(exprs_gen());
    node.add_child(std::move(tmp));
    // end of if-expression
    while(tok_list[ptr].type==tok_elsif)
    {
        nasal_ast tmp(tok_list[ptr].line,ast_elsif);
        match(tok_elsif);
        match(tok_lcurve);
        tmp.add_child(calc());
        match(tok_rcurve);
        tmp.add_child(exprs_gen());
        node.add_child(std::move(tmp));
    }
    if(tok_list[ptr].type==tok_else)
    {
        nasal_ast tmp(tok_list[ptr].line,ast_else);
        match(tok_else);
        tmp.add_child(exprs_gen());
        node.add_child(std::move(tmp));
    }
    return node;
}
nasal_ast nasal_parse::continue_expr()
{
    nasal_ast node(tok_list[ptr].line,ast_continue);
    match(tok_continue);
    return node;
}
nasal_ast nasal_parse::break_expr()
{
    nasal_ast node(tok_list[ptr].line,ast_break);
    match(tok_break);
    return node;
}
nasal_ast nasal_parse::ret_expr()
{
    nasal_ast node(tok_list[ptr].line,ast_ret);
    match(tok_ret);
    int type=tok_list[ptr].type;
    if(type==tok_nil || type==tok_num || type==tok_str || type==tok_id || type==tok_func ||
    type==tok_sub || type==tok_not || type==tok_lcurve || type==tok_lbracket || type==tok_lbrace)
        node.add_child(calc());
    return node;
}
#endif
