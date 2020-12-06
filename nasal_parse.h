#ifndef __NASAL_PARSE_H__
#define __NAsAL_PARSE_H__

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
#ifndef error_line
#define error_line (tok_list[ptr>=tok_list_size? tok_list_size-1:ptr].line)
#endif

private:
    int tok_list_size;
    int ptr;
    int error;
    nasal_ast root;
    std::vector<token> tok_list;
    int in_function; // count when generating function block,used to check return-expression
    int in_loop;     // count when generating loop block,used to check break/continue-expression
    void reset();
    void die(int,std::string);
    bool check_multi_definition();
    bool check_multi_scalar();
    bool check_function_end(nasal_ast&);
    bool check_special_call();
    bool need_semi_check(nasal_ast&);
    nasal_ast null_node_gen();
    nasal_ast nil_gen();
    nasal_ast number_gen();
    nasal_ast string_gen();
    nasal_ast id_gen();
    nasal_ast vector_gen();
    nasal_ast hash_gen();
    nasal_ast hash_member_gen();
    nasal_ast func_gen();
    nasal_ast args_list_gen();
    nasal_ast expr();
    nasal_ast exprs_gen();
    nasal_ast calculation();
    nasal_ast or_expr();
    nasal_ast and_expr();
    nasal_ast cmp_expr();
    nasal_ast additive_expr();
    nasal_ast multive_expr();
    nasal_ast unary();
    nasal_ast scalar();
    nasal_ast call_scalar();
    nasal_ast call_hash();
    nasal_ast call_vector();
    nasal_ast call_func();
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
    nasal_ast return_expr();
public:
    int get_error();
    void clear();
    void set_toklist(std::vector<token>&);
    void main_process();
    nasal_ast& get_root();
};

int nasal_parse::get_error()
{
    return this->error;
}

void nasal_parse::clear()
{
    this->tok_list_size=0;
    this->ptr=0;
    this->error=0;
    this->in_function=0;
    this->in_loop=0;
    this->tok_list.clear();
    this->root.clear();
    return;
}

void nasal_parse::set_toklist(std::vector<token>& lex_token)
{
    this->tok_list=lex_token;
    this->tok_list_size=this->tok_list.size();
    return;
}

void nasal_parse::main_process()
{
    this->reset();
    root.set_line(1);
    root.set_type(ast_root);
    while(ptr<tok_list_size)
    {
        root.add_child(expr());
        ++ptr;
        if(ptr>=tok_list_size)
            break;
        if(tok_list[ptr].type==tok_semi)
            ++ptr;
        else if(need_semi_check(root.get_children().back()))
        {
            // the last expression can be recognized without semi
            if(ptr<tok_list_size)
                die(error_line,"expected \";\"");
        }
        if(root.get_children().size())
        {
            int type=root.get_children().back().get_type();
            if(type==ast_continue || type==ast_break || type==ast_return)
            {
                ++error;
                die(root.get_children().back().get_line(),ast_str(type)+" is not allowed in main scope");
            }
        }
    }
    return;
}

nasal_ast& nasal_parse::get_root()
{
    return root;
}

void nasal_parse::reset()
{
    this->ptr=0;
    this->in_function=0;
    this->in_loop=0;
    this->error=0;
    this->root.clear();
    return;
}

void nasal_parse::die(int line,std::string info)
{
    ++error;
    std::cout<<">> [parse] line "<<line<<": "<<info<<".\n";
    return;
}

bool nasal_parse::check_multi_definition()
{
    return ptr+1<tok_list_size && tok_list[ptr+1].type==tok_var;
}

bool nasal_parse::check_multi_scalar()
{
    bool ret=false;
    int check_ptr=ptr+1,curve_cnt=1,bracket_cnt=0,brace_cnt=0;
    while(check_ptr<tok_list_size && curve_cnt)
    {
        switch(tok_list[check_ptr].type)
        {
            case tok_left_curve:   ++curve_cnt;  break;
            case tok_left_bracket: ++bracket_cnt;break;
            case tok_left_brace:   ++brace_cnt;  break;
            case tok_right_curve:  --curve_cnt;  break;
            case tok_right_bracket:--bracket_cnt;break;
            case tok_right_brace:  --brace_cnt;  break;
        }
        if(curve_cnt==1 && !bracket_cnt && !brace_cnt && tok_list[check_ptr].type==tok_comma)
        {
            ret=true;
            break;
        }
        ++check_ptr;
    }
    return ret;
}

bool nasal_parse::check_function_end(nasal_ast& node)
{
    int type=node.get_type();
    if(type==ast_function)
        return true;
    else if(type==ast_number || type==ast_identifier || type==ast_string || type==ast_nil || type==ast_vector || type==ast_hash)
        return false;
    if(
        node.get_children().empty() || 
        (
            type!=ast_definition &&
            type!=ast_equal &&
            type!=ast_add_equal &&
            type!=ast_sub_equal &&
            type!=ast_mult_equal &&
            type!=ast_div_equal &&
            type!=ast_link_equal
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
    int check_ptr=ptr+1;
    int curve_cnt=1,bracket_cnt=0,brace_cnt=0,ques_cnt=0;
    bool ret=false;
    while(check_ptr<tok_list_size && curve_cnt)
    {
        switch(tok_list[check_ptr].type)
        {
            case tok_left_curve:   ++curve_cnt;  break;
            case tok_left_bracket: ++bracket_cnt;break;
            case tok_left_brace:   ++brace_cnt;  break;
            case tok_right_curve:  --curve_cnt;  break;
            case tok_right_bracket:--bracket_cnt;break;
            case tok_right_brace:  --brace_cnt;  break;
        }
        if(curve_cnt==1 && !bracket_cnt && !brace_cnt && tok_list[check_ptr].type==tok_quesmark)
        {
            // m?1:0 will be recognized as normal parameter
            ret=false;
            break;
        }
        if(curve_cnt==1 && !bracket_cnt && !brace_cnt && tok_list[check_ptr].type==tok_colon)
        {
            ret=true;
            break;
        }
        ++check_ptr;
    }
    return ret;
}

bool nasal_parse::need_semi_check(nasal_ast& node)
{
    int type=node.get_type();
    if(type==ast_for || type==ast_foreach || type==ast_forindex || type==ast_while || type==ast_conditional)
        return false;
    return !check_function_end(node);
}

nasal_ast nasal_parse::null_node_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_null);
    return node;
}

nasal_ast nasal_parse::nil_gen()
{
    nasal_ast node;
    if(ptr>=tok_list_size) return node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_nil);
    return node;
}

nasal_ast nasal_parse::number_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_number);
    node.set_num(trans_string_to_number(tok_list[ptr].str));
    return node;
}

nasal_ast nasal_parse::string_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_string);
    node.set_str(tok_list[ptr].str);
    return node;
}

nasal_ast nasal_parse::id_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_identifier);
    node.set_str(tok_list[ptr].str);
    return node;
}

nasal_ast nasal_parse::vector_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_vector);
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_bracket)
    {
        node.add_child(calculation());
        ++ptr;
        if(ptr>=tok_list_size)
            break;
        if(tok_list[ptr].type==tok_comma)
            ++ptr;
        else if(tok_list[ptr].type!=tok_comma && tok_list[ptr].type!=tok_right_bracket)
            break;
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_bracket)
        die(error_line,"expected \"]\"");
    return node;
}
nasal_ast nasal_parse::hash_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_hash);
    ++ptr;
    while (ptr<tok_list_size && tok_list[ptr].type!=tok_right_brace)
    {
        node.add_child(hash_member_gen());
        ++ptr;
        if(ptr>=tok_list_size)
            break;
        if(tok_list[ptr].type==tok_comma)
            ++ptr;
        else if(tok_list[ptr].type!=tok_comma && tok_list[ptr].type!=tok_right_brace)
            break;
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_brace)
        die(error_line,"expected \"}\"");
    return node;
}
nasal_ast nasal_parse::hash_member_gen()
{
    nasal_ast node;
    if(ptr>=tok_list_size || (tok_list[ptr].type!=tok_identifier && tok_list[ptr].type!=tok_string))
    {
        die(error_line,"expected identifier/string");
        return node;
    }
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_hashmember);
    node.add_child(tok_list[ptr].type==tok_identifier?id_gen():string_gen());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_colon)
    {
        die(error_line,"expected \":\"");
        return node;
    }
    ++ptr;
    if(ptr<tok_list_size)
        node.add_child(calculation());
    else
        die(error_line,"expected scalar");
    return node;
}
nasal_ast nasal_parse::func_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_function);
    ++ptr;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected \"(\"");
        return node;
    }
    if(tok_list[ptr].type!=tok_left_curve && tok_list[ptr].type!=tok_left_brace)
    {
        die(error_line,"expected argument(s)/expression block");
        return node;
    }
    if(tok_list[ptr].type==tok_left_curve)
    {
        node.add_child(args_list_gen());
        ++ptr;
    }
    else
    {
        nasal_ast null_argument_list;
        node.add_child(null_argument_list);
    }
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected expression block");
        return node;
    }
    node.add_child(exprs_gen());
    return node;
}
nasal_ast nasal_parse::args_list_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_args);
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_curve)
    {
        nasal_ast tmp;
        if(tok_list[ptr].type!=tok_identifier)
        {
            die(error_line,"expected identifier");
            return node;
        }
        tmp=id_gen();
        ++ptr;
        if(ptr<tok_list_size && (tok_list[ptr].type==tok_equal || tok_list[ptr].type==tok_ellipsis))
        {
            nasal_ast special_arg;
            special_arg.set_line(tok_list[ptr].line);
            if(tok_list[ptr].type==tok_equal)
            {
                special_arg.add_child(tmp);
                ++ptr;
                special_arg.add_child(scalar());
                special_arg.set_type(ast_default_arg);
            }
            else
            {
                special_arg=tmp;
                special_arg.set_type(ast_dynamic_id);
            }
            node.add_child(special_arg);
        }
        else
        {
            --ptr;
            node.add_child(tmp);
        }
        ++ptr;
        if(ptr>=tok_list_size)
            break;
        if(tok_list[ptr].type==tok_comma)
            ++ptr;
        else if(tok_list[ptr].type!=tok_right_curve)
            break;
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
    {
        die(error_line,"expected \")\"");
        return node;
    }

    std::string args_format="func(";
    int node_child_size=node.get_children().size();
    for(int i=0;i<node_child_size;++i)
    {
        switch(node.get_children()[i].get_type())
        {
            case ast_identifier:  args_format+="val";break;
            case ast_default_arg: args_format+="val=scalar";break;
            case ast_dynamic_id:  args_format+="val...";break;
        }
        if(i!=node_child_size-1)
            args_format+=",";
    }
    args_format+=")";
    bool checked_default_val=false;
    bool checked_dynamic_ids=false;
    for(int i=0;i<node_child_size;++i)
    {
        if(node.get_children()[i].get_type()==ast_default_arg)
            checked_default_val=true;
        else if(node.get_children()[i].get_type()==ast_dynamic_id)
            checked_dynamic_ids=true;
        if(checked_default_val && node.get_children()[i].get_type()!=ast_default_arg)
            die(error_line,"default argument must be the end of argument list: "+args_format);
        if(checked_dynamic_ids && i!=node_child_size-1)
            die(error_line,"dynamic identifier must be the end of argument list: "+args_format);
    }
    std::map<std::string,bool> argname_table;
    for(int i=0;i<node_child_size;++i)
    {
        int tmp_type=node.get_children()[i].get_type();
        std::string new_name;
        switch(tmp_type)
        {
            case ast_dynamic_id:
            case ast_identifier:new_name=node.get_children()[i].get_str();break;
            case ast_default_arg:new_name=node.get_children()[i].get_children()[0].get_str();break;
        }
        if(argname_table.find(new_name)!=argname_table.end())
            die(node.get_children()[i].get_line(),"argument name should not repeat");
        else
            argname_table[new_name]=true;
    }
    return node;
}
nasal_ast nasal_parse::expr()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    int tok_type=tok_list[ptr].type;
    if((tok_type==tok_break || tok_type==tok_continue) && !in_loop)
    {
        die(error_line,"cannot use break/continue outside loop");
        return node;
    }
    if(tok_type==tok_return && !in_function)
    {
        die(error_line,"cannot use return outside function");
        return node;
    }
    switch(tok_type)
    {   
        case tok_nil:
        case tok_number:
        case tok_string:
        case tok_identifier:
        case tok_func:
        case tok_left_bracket:
        case tok_left_brace:
        case tok_sub:
        case tok_not:      node=calculation();   break;
        case tok_var:      node=definition();    break;
        case tok_left_curve:
            if(check_multi_definition())
                node=definition();
            else if(check_multi_scalar())
                node=multi_assgin();
            else
                node=calculation();
            break;
        case tok_for:
        case tok_forindex:
        case tok_foreach:
        case tok_while:    node=loop();          break;
        case tok_if:       node=conditional();   break;
        case tok_continue: node=continue_expr(); break;
        case tok_break:    node=break_expr();    break;
        case tok_return:   node=return_expr();   break;
        case tok_semi:     --ptr;                break;
        default: die(error_line,"error token \""+tok_list[ptr].str+"\"");break;
    }
    return node;
}
nasal_ast nasal_parse::exprs_gen()
{
    nasal_ast node;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected expression block");
        return node;
    }
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_block);
    if(tok_list[ptr].type==tok_left_brace)
    {
        int left_brace_line=tok_list[ptr].line;
        ++ptr;
        while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_brace)
        {
            node.add_child(expr());
            ++ptr;
            if(ptr>=tok_list_size)
                break;
            else if(tok_list[ptr].type==tok_semi)
                ++ptr;
            else if(need_semi_check(node.get_children().back()))
            {
                // the last expression can be recognized without semi
                if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_brace)
                {
                    die(error_line,"expected \";\"");
                    break;
                }
            }
        }
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_brace)
        {
            std::string lb_line="";
            while(left_brace_line>1)
            {
                lb_line=(char)('0'+(left_brace_line%10))+lb_line;
                left_brace_line/=10;
            }
            die(error_line,"expected \"}\" to match \"{\" in line "+lb_line);
        }
    }
    else
    {
        node.add_child(expr());
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_semi)
            --ptr;
    }
    return node;
}
nasal_ast nasal_parse::calculation()
{
    nasal_ast node;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected calculation");
        return node;
    }
    node=or_expr();
    ++ptr;
    if(ptr<tok_list_size && tok_list[ptr].type==tok_quesmark)
    {
        // trinocular calculation
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        tmp.set_type(ast_trinocular);
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size) tmp.add_child(calculation());
        else
        {
            die(error_line,"expected calculation");
            return node;
        }
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_colon)
        {
            die(error_line,"expected \":\"");
            return node;
        }
        ++ptr;
        if(ptr<tok_list_size)
            tmp.add_child(calculation());
        else
            die(error_line,"expected calculation");
        node=tmp;
    }
    else if(
        ptr<tok_list_size &&
        (
            tok_list[ptr].type==tok_equal      ||
            tok_list[ptr].type==tok_add_equal  ||
            tok_list[ptr].type==tok_sub_equal  ||
            tok_list[ptr].type==tok_mult_equal ||
            tok_list[ptr].type==tok_div_equal  ||
            tok_list[ptr].type==tok_link_equal
        )
    )
    {
        // check the left expression to confirm it is available to get memory
        if(node.get_type()!=ast_call && node.get_type()!=ast_identifier)
            die(node.get_line(),"cannot use calculation as the memory of scalar");
        if(node.get_type()==ast_call)
        {
            if(node.get_children()[0].get_type()!=ast_identifier)
                die(node.get_children()[0].get_line(),"cannot get the memory of temporary value");
            int size=node.get_children().size();
            for(int i=0;i<size;++i)
            {
                nasal_ast& tmp_node=node.get_children()[i];
                if(tmp_node.get_type()==ast_call_func)
                    die(tmp_node.get_line(),"cannot get the memory of function-returned value");
                if(tmp_node.get_type()==ast_call_vec)
                {
                    if(tmp_node.get_children().size()>1)
                        die(tmp_node.get_line(),"cannot get the memory in temporary sliced vector");
                    else if(tmp_node.get_children()[0].get_type()==ast_subvec)
                        die(tmp_node.get_children()[0].get_line(),"cannot get the memory in temporary sliced vector");
                }
            } 
        }
        // assignment
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        switch(tok_list[ptr].type)
        {
            case tok_equal:      tmp.set_type(ast_equal);      break;
            case tok_add_equal:  tmp.set_type(ast_add_equal);  break;
            case tok_sub_equal:  tmp.set_type(ast_sub_equal);  break;
            case tok_mult_equal: tmp.set_type(ast_mult_equal); break;
            case tok_div_equal:  tmp.set_type(ast_div_equal);  break;
            case tok_link_equal: tmp.set_type(ast_link_equal); break;
        }
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size)
            tmp.add_child(calculation());
        else
            die(error_line,"expected calculation");
        node=tmp;
    }
    else --ptr;
    return node;
}
nasal_ast nasal_parse::or_expr()
{
    nasal_ast node;
    node=and_expr();
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type==tok_or)
    {
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        tmp.set_type(ast_or);
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size)
            tmp.add_child(and_expr());
        else
            die(error_line,"expected calculation");
        // pre-calculation
        node=tmp;
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::and_expr()
{
    nasal_ast node;
    node=cmp_expr();
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type==tok_and)
    {
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        tmp.set_type(ast_and);
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size)
            tmp.add_child(cmp_expr());
        else
            die(error_line,"expected calculation");
        // pre-calculation
        int type1=tmp.get_children()[0].get_type();
        int type2=tmp.get_children()[1].get_type();
        if(type1==ast_nil || type2==ast_nil)
        {
            tmp.set_type(ast_number);
            tmp.set_num(0);
            tmp.get_children().clear();
        }
        else if((type1==ast_number && tmp.get_children()[0].get_num()==0)||(type2==ast_number && tmp.get_children()[1].get_num()==0))
        {
            tmp.set_type(ast_number);
            tmp.set_num(0);
            tmp.get_children().clear();
        }
        if(type1==ast_string && tmp.get_type()==ast_and)
        {
            std::string str=tmp.get_children()[0].get_str();
            double num=trans_string_to_number(str);
            if(num==0 || !str.length())
            {
                tmp.set_type(ast_number);
                tmp.set_num(0);
                tmp.get_children().clear();
            }
        }
        if(type2==ast_string && tmp.get_type()==ast_and)
        {
            std::string str=tmp.get_children()[1].get_str();
            double num=trans_string_to_number(str);
            if(num==0 || !str.length())
            {
                tmp.set_type(ast_number);
                tmp.set_num(0);
                tmp.get_children().clear();
            }
        }
        node=tmp;
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::cmp_expr()
{
    nasal_ast node;
    node=additive_expr();
    ++ptr;
    while(
        ptr<tok_list_size &&
        (
            tok_list[ptr].type==tok_cmp_equal     ||
            tok_list[ptr].type==tok_cmp_not_equal ||
            tok_list[ptr].type==tok_less_than     ||
            tok_list[ptr].type==tok_less_equal    ||
            tok_list[ptr].type==tok_greater_than  ||
            tok_list[ptr].type==tok_greater_equal
        )
    )
    {
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        switch(tok_list[ptr].type)
        {
            case tok_cmp_equal:     tmp.set_type(ast_cmp_equal);     break;
            case tok_cmp_not_equal: tmp.set_type(ast_cmp_not_equal); break;
            case tok_less_than:     tmp.set_type(ast_less_than);     break;
            case tok_less_equal:    tmp.set_type(ast_less_equal);    break;
            case tok_greater_than:  tmp.set_type(ast_greater_than);  break;
            case tok_greater_equal: tmp.set_type(ast_greater_equal); break;
        }
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size)
            tmp.add_child(additive_expr());
        else
            die(error_line,"expected calculation");
        // pre-calculation
        int type1=tmp.get_children()[0].get_type();
        int type2=tmp.get_children()[1].get_type();
        if(type1==ast_nil && type2==ast_nil)
        {
            double num;
            switch(tmp.get_type())
            {
                case ast_cmp_equal:num=1;break;
                case ast_cmp_not_equal:num=0;break;
                case ast_less_than:die(tmp.get_line(),"nil cannot take part in \"<\"");break;
                case ast_less_equal:num=1;break;
                case ast_greater_than:die(tmp.get_line(),"nil cannot take part in \">\"");break;
                case ast_greater_equal:num=1;break;
            }
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(
            (type1==ast_nil && type2==ast_string) || 
            (type1==ast_string && type2==ast_nil) ||
            (type1==ast_nil && type2==ast_number) ||
            (type1==ast_number && type2==ast_nil)
        )
        {
            double num=(tmp.get_type()==ast_cmp_not_equal);
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(type1==ast_number && type2==ast_number)
        {
            double num;
            double num1=tmp.get_children()[0].get_num();
            double num2=tmp.get_children()[1].get_num();
            switch(tmp.get_type())
            {
                case ast_cmp_equal:num=(num1==num2);break;
                case ast_cmp_not_equal:num=(num1!=num2);break;
                case ast_less_than:num=(num1<num2);break;
                case ast_less_equal:num=(num1<=num2);break;
                case ast_greater_than:num=(num1>num2);break;
                case ast_greater_equal:num=(num1>=num2);break;
            }
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(type1==ast_number && type2==ast_string)
        {
            double num;
            double num1=tmp.get_children()[0].get_num();
            double num2=trans_string_to_number(tmp.get_children()[1].get_str());
            if(std::isnan(num2))
                die(tmp.get_children()[1].get_line(),"cannot compare number and string");
            switch(tmp.get_type())
            {
                case ast_cmp_equal:num=(num1==num2);break;
                case ast_cmp_not_equal:num=(num1!=num2);break;
                case ast_less_than:num=(num1<num2);break;
                case ast_less_equal:num=(num1<=num2);break;
                case ast_greater_than:num=(num1>num2);break;
                case ast_greater_equal:num=(num1>=num2);break;
            }
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(type1==ast_string && type2==ast_number)
        {
            double num;
            double num1=trans_string_to_number(tmp.get_children()[0].get_str());
            double num2=tmp.get_children()[1].get_num();
            if(std::isnan(num2))
                die(tmp.get_children()[0].get_line(),"cannot compare number and string");
            switch(tmp.get_type())
            {
                case ast_cmp_equal:num=(num1==num2);break;
                case ast_cmp_not_equal:num=(num1!=num2);break;
                case ast_less_than:num=(num1<num2);break;
                case ast_less_equal:num=(num1<=num2);break;
                case ast_greater_than:num=(num1>num2);break;
                case ast_greater_equal:num=(num1>=num2);break;
            }
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(type1==ast_string && type2==ast_string)
        {
            double num;
            std::string str1=tmp.get_children()[0].get_str();
            std::string str2=tmp.get_children()[1].get_str();
            switch(tmp.get_type())
            {
                case ast_cmp_equal:num=(str1==str2);break;
                case ast_cmp_not_equal:num=(str1!=str2);break;
                case ast_less_than:num=(str1<str2);break;
                case ast_less_equal:num=(str1<=str2);break;
                case ast_greater_than:num=(str1>str2);break;
                case ast_greater_equal:num=(str1>=str2);break;
            }
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        node=tmp;
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::additive_expr()
{
    nasal_ast node;
    node=multive_expr();
    ++ptr;
    while(ptr<tok_list_size && (tok_list[ptr].type==tok_add || tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_link))
    {
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        switch(tok_list[ptr].type)
        {
            case tok_add:  tmp.set_type(ast_add);  break;
            case tok_sub:  tmp.set_type(ast_sub);  break;
            case tok_link: tmp.set_type(ast_link); break;
        }
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size)
            tmp.add_child(multive_expr());
        else
            die(error_line,"expected calculation");
        // pre-calculation
        if(tmp.get_type()!=ast_link)
        {
            int type1=tmp.get_children()[0].get_type();
            int type2=tmp.get_children()[1].get_type();
            double num1,num2,num;
            if(type1==ast_number && type2==ast_number)
            {
                num1=tmp.get_children()[0].get_num();
                num2=tmp.get_children()[1].get_num();
                num=(tmp.get_type()==ast_add? num1+num2:num1-num2);
                tmp.set_type(ast_number);
                tmp.set_num(num);
                tmp.get_children().clear();
            }
            else if(type1==ast_string && type2==ast_number)
            {
                std::string str=tmp.get_children()[0].get_str();
                num1=trans_string_to_number(str);
                num2=tmp.get_children()[1].get_num();
                if(std::isnan(num1))
                    die(tmp.get_children()[0].get_line(),"plus/minus a string: "+str);
                num=(tmp.get_type()==ast_add? num1+num2:num1-num2);
                tmp.set_type(ast_number);
                tmp.set_num(num);
                tmp.get_children().clear();
            }
            else if(type1==ast_number && type2==ast_string)
            {
                std::string str=tmp.get_children()[1].get_str();
                num1=tmp.get_children()[0].get_num();
                num2=trans_string_to_number(str);
                if(std::isnan(num2))
                    die(tmp.get_children()[1].get_line(),"plus/minus a string: "+str);
                num=(tmp.get_type()==ast_add? num1+num2:num1-num2);
                tmp.set_type(ast_number);
                tmp.set_num(num);
                tmp.get_children().clear();
            }
            else if(type1==ast_string && type2==ast_string)
            {
                std::string str1=tmp.get_children()[0].get_str();
                std::string str2=tmp.get_children()[1].get_str();
                num1=trans_string_to_number(str1);
                num2=trans_string_to_number(str2);
                if(std::isnan(num1))
                    die(tmp.get_children()[0].get_line(),"plus/minus a string: "+str1);
                if(std::isnan(num2))
                    die(tmp.get_children()[1].get_line(),"plus/minus a string: "+str2);
                num=(tmp.get_type()==ast_add? num1+num2:num1-num2);
                tmp.set_type(ast_number);
                tmp.set_num(num);
                tmp.get_children().clear();
            }
        }
        else
        {
            int type1=tmp.get_children()[0].get_type();
            int type2=tmp.get_children()[1].get_type();
            std::string s1,s2;
            if((type1==ast_number || type1==ast_string) && (type2==ast_number || type2==ast_string))
            {
                if(type1==ast_number)
                    s1=trans_number_to_string(tmp.get_children()[0].get_num());
                else
                    s1=tmp.get_children()[0].get_str();
                if(type2==ast_number)
                    s2=trans_number_to_string(tmp.get_children()[1].get_num());
                else
                    s2=tmp.get_children()[1].get_str();
                s1+=s2;
                tmp.set_type(ast_string);
                tmp.set_str(s1);
                tmp.get_children().clear();
            }
            else if(type1==ast_string && type2==ast_string)
            {
                s1=tmp.get_children()[0].get_str();
                s2=tmp.get_children()[1].get_str();
                s1+=s2;
                tmp.set_type(ast_string);
                tmp.set_str(s1);
                tmp.get_children().clear();
            }
        }
        node=tmp;
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::multive_expr()
{
    nasal_ast node;
    node=(tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not)?unary():scalar();
    ++ptr;
    while(ptr<tok_list_size && (tok_list[ptr].type==tok_mult || tok_list[ptr].type==tok_div))
    {
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        switch(tok_list[ptr].type)
        {
            case tok_mult:tmp.set_type(ast_mult);break;
            case tok_div: tmp.set_type(ast_div); break;
        }
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size)
            tmp.add_child((tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not)?unary():scalar());
        else
        {
            die(error_line,"expected calculation");
            break;
        }
        // pre-calculation
        int type1=tmp.get_children()[0].get_type();
        int type2=tmp.get_children()[1].get_type();
        double num1,num2,num;
        if(type1==ast_number && type2==ast_number)
        {
            num1=tmp.get_children()[0].get_num();
            num2=tmp.get_children()[1].get_num();
            num=(tmp.get_type()==ast_mult? num1*num2:num1/num2);
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(type1==ast_string && type2==ast_number)
        {
            std::string str=tmp.get_children()[0].get_str();
            num1=trans_string_to_number(str);
            num2=tmp.get_children()[1].get_num();
            if(std::isnan(num1))
                die(tmp.get_children()[0].get_line(),"multiply a string: "+str);
            num=(tmp.get_type()==ast_mult? num1*num2:num1/num2);
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(type1==ast_number && type2==ast_string)
        {
            std::string str=tmp.get_children()[1].get_str();
            num1=tmp.get_children()[0].get_num();
            num2=trans_string_to_number(str);
            if(std::isnan(num2))
                die(tmp.get_children()[1].get_line(),"multiply a string: "+str);
            num=(tmp.get_type()==ast_mult? num1*num2:num1/num2);
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        else if(type1==ast_string && type2==ast_string)
        {
            std::string str1=tmp.get_children()[0].get_str();
            std::string str2=tmp.get_children()[1].get_str();
            num1=trans_string_to_number(str1);
            num2=trans_string_to_number(str2);
            if(std::isnan(num1))
                die(tmp.get_children()[0].get_line(),"multiply a string: "+str1);
            if(std::isnan(num2))
                die(tmp.get_children()[1].get_line(),"multiply a string: "+str2);
            num=(tmp.get_type()==ast_mult? num1*num2:num1/num2);
            tmp.set_type(ast_number);
            tmp.set_num(num);
            tmp.get_children().clear();
        }
        node=tmp;
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::unary()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    switch(tok_list[ptr].type)
    {
        case tok_sub:node.set_type(ast_unary_sub);break;
        case tok_not:node.set_type(ast_unary_not);break;
    }
    ++ptr;
    if(ptr<tok_list_size)
    {
        if(tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not)
            node.add_child(unary());
        else
            node.add_child(scalar());
    }
    else
        die(error_line,"expected calculation");
    // pre-calculation
    if(node.get_children()[0].get_type()==ast_number)
    {
        double num=node.get_children()[0].get_num();
        if(node.get_type()==ast_unary_not)
            num=(!num);
        else
            num=-num;
        node.set_type(ast_number);
        node.set_num(num);
        node.get_children().clear();
    }
    else if(node.get_children()[0].get_type()==ast_string)
    {
        std::string str=node.get_children()[0].get_str();
        double num=trans_string_to_number(str);
        if(node.get_type()==ast_unary_sub)
        {
            if(std::isnan(num))
                die(node.get_line(),"cannot get negative value of this string: "+str);
            else
            {
                node.set_type(ast_number);
                node.set_num(-num);
                node.get_children().clear();
            }
        }
        else if(node.get_type()==ast_unary_not)
        {
            if(std::isnan(num))
            {
                node.set_type(ast_number);
                node.set_num(!str.length());
                node.get_children().clear();
            }
            else
            {
                node.set_type(ast_number);
                node.set_num(num==0);
                node.get_children().clear();
            }
        }
    }
    return node;
}
nasal_ast nasal_parse::scalar()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    if(tok_list[ptr].type==tok_nil)
        node=nil_gen();
    else if(tok_list[ptr].type==tok_number)
        node=number_gen();
    else if(tok_list[ptr].type==tok_string)
        node=string_gen();
    else if(tok_list[ptr].type==tok_identifier)
        node=id_gen();
    else if(tok_list[ptr].type==tok_func)
    {
        if(ptr+1<tok_list_size && tok_list[ptr+1].type==tok_identifier)
        {
            ++ptr;
            node=id_gen();
        }
        else
        {
            ++in_function;
            node=func_gen();
            --in_function;
        }
    }
    else if(tok_list[ptr].type==tok_left_bracket)
        node=vector_gen();
    else if(tok_list[ptr].type==tok_left_brace)
        node=hash_gen();
    else if(tok_list[ptr].type==tok_left_curve)
    {
        ++ptr;
        if(ptr<tok_list_size)
            node=calculation();
        else
            die(error_line,"expected calculation");
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
            die(error_line,"expected \")\"");
    }
    else
    {
        die(error_line,"expected scalar");
        return node;
    }
    ++ptr;
    if(ptr<tok_list_size && (tok_list[ptr].type==tok_left_curve || tok_list[ptr].type==tok_left_bracket || tok_list[ptr].type==tok_dot))
    {
        nasal_ast tmp=node;
        node.clear();
        node.set_line(tok_list[ptr].line);
        node.set_type(ast_call);
        node.add_child(tmp);
    }
    while(ptr<tok_list_size && (tok_list[ptr].type==tok_left_curve || tok_list[ptr].type==tok_left_bracket || tok_list[ptr].type==tok_dot))
    {
        node.add_child(call_scalar());
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::call_scalar()
{
    nasal_ast node;
    switch(tok_list[ptr].type)
    {
        case tok_left_curve:   node=call_func();   break;
        case tok_left_bracket: node=call_vector(); break;
        case tok_dot:          node=call_hash();   break;
    }
    return node;
}
nasal_ast nasal_parse::call_hash()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_call_hash);
    ++ptr;
    if(ptr<tok_list_size && tok_list[ptr].type==tok_identifier)
        node.set_str(tok_list[ptr].str);
    else
        die(error_line,"expected identifier");
    return node;
}
nasal_ast nasal_parse::call_vector()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_call_vec);
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_bracket)
    {
        node.add_child(subvec());
        ++ptr;
        if(ptr>=tok_list_size)
            break;
        else if(tok_list[ptr].type==tok_comma)
            ++ptr;
        else if(tok_list[ptr].type!=tok_right_bracket)
            break;
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_bracket)
        die(error_line,"expected \"]\"");
    return node;
}
nasal_ast nasal_parse::call_func()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_call_func);
    bool special_call=check_special_call();
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_curve)
    {
        node.add_child(special_call?hash_member_gen():calculation());
        ++ptr;
        if(ptr>=tok_list_size)
            break;
        else if(tok_list[ptr].type==tok_comma)
            ++ptr;
        else if(tok_list[ptr].type!=tok_right_curve)
            break;
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        die(error_line,"expected \")\"");
    return node;
}
nasal_ast nasal_parse::subvec()
{
    nasal_ast node;
    if(tok_list[ptr].type==tok_colon)
    {
        --ptr;
        node=nil_gen();
    }
    else node=calculation();
    ++ptr;
    if(ptr<tok_list_size && tok_list[ptr].type==tok_colon)
    {
        nasal_ast tmp;
        ++ptr;
        if(ptr<tok_list_size)
        {
            tmp.set_line(node.get_line());
            tmp.set_type(ast_subvec);
            tmp.add_child(node);
            if(tok_list[ptr].type==tok_comma || tok_list[ptr].type==tok_right_bracket)
            {
                --ptr;
                tmp.add_child(nil_gen());
            }
            else
                tmp.add_child(calculation());
            node=tmp;
        }
    }
    else
        --ptr;
    return node;
}
nasal_ast nasal_parse::definition()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_definition);
    if(tok_list[ptr].type==tok_var)
    {
        ++ptr;
        switch(tok_list[ptr].type)
        {
            case tok_identifier:node.add_child(id_gen());           break;
            case tok_left_curve:node.add_child(var_outcurve_def()); break;
            default:
                die(error_line,"expected identifier");
                return node;
        }
    }
    else if(tok_list[ptr].type==tok_left_curve)
        node.add_child(var_incurve_def());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_equal)
    {
        die(error_line,"expected \"=\" / don\'t call identifier in definition");
        return node;
    }
    ++ptr;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected scalar");
        return node;
    }
    if(tok_list[ptr].type==tok_left_curve)
        node.add_child(check_multi_scalar()?multi_scalar(false):calculation());
    else
        node.add_child(calculation());
    if(node.get_children()[0].get_type()==ast_identifier && node.get_children()[1].get_type()==ast_multi_scalar)
        die(node.get_children()[1].get_line(),"one identifier cannot accept too many values");
    else if(node.get_children()[0].get_type()==ast_multi_id && node.get_children()[1].get_type()==ast_multi_scalar)
    {
        if(node.get_children()[0].get_children().size()!=node.get_children()[1].get_children().size())
            die(node.get_children()[0].get_line(),"too much or lack values in multi-definition");
    }
    return node;
}
nasal_ast nasal_parse::var_incurve_def()
{
    nasal_ast node;
    ++ptr;// check_multi_definition will check the 'var'
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_identifier)
    {
        die(error_line,"expected identifier");
        return node;
    }
    node=multi_id();
    ++ptr;
    if(ptr<tok_list_size && (tok_list[ptr].type==tok_dot || tok_list[ptr].type==tok_left_bracket || tok_list[ptr].type==tok_left_curve))
        die(error_line,"don\'t call identifier in multi-definition");
    else if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        die(error_line,"expected \")\"");
    return node;
}
nasal_ast nasal_parse::var_outcurve_def()
{
    nasal_ast node;
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_identifier)
    {
        die(error_line,"expected identifier");
        return node;
    }
    node=multi_id();
    ++ptr;
    if(ptr<tok_list_size && (tok_list[ptr].type==tok_dot || tok_list[ptr].type==tok_left_bracket || tok_list[ptr].type==tok_left_curve))
        die(error_line,"don\'t call identifier in multi-definition");
    else if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        die(error_line,"expected \")\"");
    return node;
}
nasal_ast nasal_parse::multi_id()
{
    nasal_ast node;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_identifier)
    {
        die(error_line,"expected identifier");
        return node;
    }
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_multi_id);
    while(ptr<tok_list_size)
    {
        node.add_child(id_gen());
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_comma)
        {
            --ptr;
            break;
        }
        ++ptr;
    }
    return node;
}
nasal_ast nasal_parse::multi_scalar(bool check_call_memory)
{
    // if check_call_memory is true,we will check if value called here can reach a memory space
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_multi_scalar);
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_curve)
    {
        node.add_child(calculation());
        int type=node.get_children().back().get_type();
        if(check_call_memory && type!=ast_call && type!=ast_identifier)
            die(node.get_children().back().get_line(),"cannot use calculation as the memory of scalar");
        ++ptr;
        if(ptr>=tok_list_size)
            break;
        if(tok_list[ptr].type==tok_comma)
            ++ptr;
        else if(tok_list[ptr].type!=tok_right_curve)
            break;
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        die(error_line,"expected \")\"");
    return node;
}
nasal_ast nasal_parse::multi_assgin()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_multi_assign);
    node.add_child(multi_scalar(true));
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_equal)
    {
        die(error_line,"expected \"=\"");
        return node;
    }
    ++ptr;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected value list");
        return node;
    }
    if(tok_list[ptr].type==tok_left_curve)
        node.add_child(check_multi_scalar()?multi_scalar(false):calculation());
    else
        node.add_child(calculation());
    if(node.get_children()[1].get_type()==ast_multi_scalar)
    {
        if(node.get_children()[0].get_children().size()!=node.get_children()[1].get_children().size())
            die(node.get_children()[0].get_line(),"too much or lack values in multi-assignment");
    }
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
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_while);
    ++ptr;
    if(ptr<tok_list_size && tok_list[ptr].type==tok_left_curve)
    {
        ++ptr;
        node.add_child(calculation());
    }
    else
    {
        die(error_line,"expected \"(\"");
        return node;
    }
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
    {
        die(error_line,"expected \")\"");
        return node;
    }
    ++ptr;
    node.add_child(exprs_gen());
    return node;
}
nasal_ast nasal_parse::for_loop()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_for);
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_left_curve)
    {
        die(error_line,"expected \"(\"");
        return node;
    }
    // first expression
    ++ptr;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected definition");
        return node;
    }
    if(tok_list[ptr].type==tok_semi)
    {
        node.add_child(null_node_gen());
        --ptr;
    }
    else if(tok_list[ptr].type==tok_var)
        node.add_child(definition());
    else if(tok_list[ptr].type==tok_left_curve)
    {
        if(check_multi_definition())
            node.add_child(definition());
        else if(check_multi_scalar())
            node.add_child(multi_assgin());
        else
            node.add_child(calculation());
    }
    else
        node.add_child(calculation());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_semi)
    {
        die(error_line,"expected \";\"");
        return node;
    }
    // conditional expression
    ++ptr;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected conditional expression");
        return node;
    }
    if(tok_list[ptr].type==tok_semi)
    {
        node.add_child(null_node_gen());
        --ptr;
    }
    else
        node.add_child(calculation());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_semi)
    {
        die(error_line,"expected \";\"");
        return node;
    }
    //after loop expression
    ++ptr;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected calculation");
        return node;
    }
    if(tok_list[ptr].type==tok_right_curve)
    {
        node.add_child(null_node_gen());
        --ptr;
    }
    else
        node.add_child(calculation());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
    {
        die(error_line,"expected \")\"");
        return node;
    }
    ++ptr;
    node.add_child(exprs_gen());
    return node;
}
nasal_ast nasal_parse::forei_loop()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    switch(tok_list[ptr].type)
    {
        case tok_forindex: node.set_type(ast_forindex);break;
        case tok_foreach:  node.set_type(ast_foreach); break;
    }
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_left_curve)
    {
        die(error_line,"expected \"(\"");
        return node;
    }
    // first expression
    // foreach/forindex must have an iterator to loop through
    ++ptr;
    if(ptr>=tok_list_size || (tok_list[ptr].type!=tok_var && tok_list[ptr].type!=tok_identifier))
    {
        die(error_line,"expected iterable value");
        return node;
    }
    node.add_child(new_iter_gen());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_semi)
    {
        die(error_line,"expected \";\"");
        return node;
    }
    ++ptr;
    if(ptr>=tok_list_size)
    {
        die(error_line,"expected vector");
        return node;
    }
    node.add_child(calculation());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
    {
        die(error_line,"expected \")\"");
        return node;
    }
    ++ptr;
    node.add_child(exprs_gen());
    return node;
}

nasal_ast nasal_parse::new_iter_gen()
{
    nasal_ast node;
    if(tok_list[ptr].type==tok_var)
    {
        node.set_line(tok_list[ptr].line);
        node.set_type(ast_new_iter);
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_identifier)
        {
            die(error_line,"expected identifier");
            return node;
        }
        node.add_child(id_gen());
    }
    else
    {
        node.set_line(tok_list[ptr].line);
        node.set_type(ast_call);
        node.add_child(id_gen());
        ++ptr;
        while(ptr<tok_list_size && (tok_list[ptr].type==tok_left_curve || tok_list[ptr].type==tok_left_bracket || tok_list[ptr].type==tok_dot))
        {
            node.add_child(call_scalar());
            ++ptr;
        }
        --ptr;
    }
    return node;
}

nasal_ast nasal_parse::conditional()
{
    nasal_ast node;
    nasal_ast tmp;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_conditional);
    tmp.set_line(tok_list[ptr].line);
    tmp.set_type(ast_if);
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_left_curve)
    {
        die(error_line,"expected \"(\"");
        return node;
    }
    ++ptr;
    tmp.add_child(calculation());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
    {
        die(error_line,"expected \")\"");
        return node;
    }
    ++ptr;
    tmp.add_child(exprs_gen());
    node.add_child(tmp);
    // end of if-expression
    ++ptr;
    while(ptr<tok_list_size && (tok_list[ptr].type==tok_elsif || (tok_list[ptr].type==tok_else && ptr+1<tok_list_size && tok_list[ptr+1].type==tok_if)))
    {
        if(tok_list[ptr].type==tok_else)
            ++ptr;
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_left_curve)
        {
            die(error_line,"expected \"(\"");
            return node;
        }
        tmp.set_line(tok_list[ptr].line);
        tmp.set_type(ast_elsif);
        tmp.get_children().clear();
        ++ptr;
        tmp.add_child(calculation());
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        {
            die(error_line,"expected \")\"");
            return node;
        }
        ++ptr;
        tmp.add_child(exprs_gen());
        node.add_child(tmp);
        ++ptr;
    }
    // end of elsif-expression
    // after this process,ptr will point to the next token of exprs_gen()'s last token
    // for example
    // else if(scalar){} else {}
    //                  ptr^
    if(ptr<tok_list_size && tok_list[ptr].type==tok_else)
    {
        tmp.set_line(tok_list[ptr].line);
        tmp.set_type(ast_else);
        tmp.get_children().clear();
        ++ptr;
        tmp.add_child(exprs_gen());
        node.add_child(tmp);
    }
    else
        --ptr;
    return node;
}
nasal_ast nasal_parse::continue_expr()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_continue);
    return node;
}
nasal_ast nasal_parse::break_expr()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_break);
    return node;
}
nasal_ast nasal_parse::return_expr()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_return);
    ++ptr;
    if(ptr<tok_list_size)
    {
        int type=tok_list[ptr].type;
        if(type==tok_nil || type==tok_number || type==tok_string || type==tok_identifier || type==tok_func ||
        type==tok_sub || type==tok_not || type==tok_left_curve || type==tok_left_bracket || type==tok_left_brace)
            node.add_child(calculation());
        else
            --ptr;
    }
    return node;
}
#endif
