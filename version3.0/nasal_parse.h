#ifndef __NASAL_PARSE_H__
#define __NAsAL_PARSE_H__

class nasal_parse
{
#ifndef error_line
#define error_line tok_list[ptr>=tok_list_size? tok_list_size-1:ptr].line
#endif

private:
    int tok_list_size;
    int ptr;
    int error;
    nasal_ast root;
    std::vector<token> tok_list;
    void reset();
    bool check_multi_definition();
    bool check_multi_scalar();
    bool check_function_end(nasal_ast&);
    bool check_special_call();
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
    nasal_ast multi_id();
    nasal_ast multi_scalar();
    nasal_ast multi_assgin();
    nasal_ast loop();
    nasal_ast while_loop();
    nasal_ast for_loop();
    nasal_ast forei_loop();
    nasal_ast conditional();
    nasal_ast continue_expr();
    nasal_ast break_expr();
    nasal_ast return_expr();
public:
    int get_error();
    void clear();
    void set_toklist(std::vector<token>&);
    void main_process();
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
        if(ptr<tok_list_size && tok_list[ptr].type==tok_semi) ++ptr;
        else if(ptr<tok_list_size && (root.get_children().empty() || !check_function_end(root.get_children().back())))
        {
            ++error;
            error_info(error_line,lack_semi);
        }
    }
    std::cout<<">> [parse] complete generation. "<<error<<" error(s)."<<std::endl;
    return;
}

void nasal_parse::reset()
{
    this->ptr=0;
    this->error=0;
    this->root.clear();
    return;
}

bool nasal_parse::check_multi_definition()
{
    return ptr+1<tok_list_size && tok_list[ptr+1].type==tok_var;
}

bool nasal_parse::check_multi_scalar()
{
    bool ret=false;
    int check_ptr=ptr,curve_cnt=1,bracket_cnt=0,brace_cnt=0;
    while(curve_cnt)
    {
        ++check_ptr;
        if(check_ptr<tok_list_size)
        {
            switch(tok_list[check_ptr].type)
            {
                case tok_left_curve:++curve_cnt;break;
                case tok_left_bracket:++bracket_cnt;break;
                case tok_left_brace:++brace_cnt;break;
                case tok_right_curve:--curve_cnt;break;
                case tok_right_bracket:--bracket_cnt;break;
                case tok_right_brace:--brace_cnt;break;
            }
        }
        else break;
        if(curve_cnt==1 && !bracket_cnt && !brace_cnt && tok_list[check_ptr].type==tok_comma)
        {
            ret=true;
            break;
        }
    }
    return ret;
}

bool nasal_parse::check_function_end(nasal_ast& node)
{
    if(node.get_type()==ast_function)
        return true;
    if(node.get_children().empty() || (node.get_type()!=ast_definition && node.get_type()!=ast_equal))
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
    int curve_cnt=1;
    bool ret=false;
    while(check_ptr<tok_list_size && curve_cnt && !ret)
    {
        switch(tok_list[check_ptr].type)
        {
            case tok_left_curve:++curve_cnt;break;
            case tok_right_curve:--curve_cnt;break;
        }
        if(curve_cnt==1 && tok_list[check_ptr].type==tok_colon)
        {
            ret=true;
            break;
        }
    }
    return ret;
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
    node.set_str(tok_list[ptr].str);
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
        if(ptr<tok_list_size && tok_list[ptr].type==tok_comma) ++ptr;
        else if(ptr<tok_list_size && tok_list[ptr].type!=tok_comma && tok_list[ptr].type!=tok_right_bracket)
        {
            error_info(error_line,lack_comma);
            ++error;
            break;
        }
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_bracket)
    {
        ++error;
        error_info(error_line,lack_right_bracket);
    }
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
        if(ptr<tok_list_size && tok_list[ptr].type==tok_comma) ++ptr;
        else if(ptr<tok_list_size && tok_list[ptr].type!=tok_comma && tok_list[ptr].type!=tok_right_brace)
        {
            error_info(error_line,lack_comma);
            ++error;
            break;
        }
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_brace)
    {
        ++error;
        error_info(error_line,lack_right_brace);
    }
    return node;
}
nasal_ast nasal_parse::hash_member_gen()
{
    nasal_ast node;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_identifier)
    {
        error_info(error_line,lack_identifier);
        return node;
    }
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_hashmember);
    node.add_child(id_gen());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_colon)
    {
        error_info(error_line,lack_colon);
        ++error;
        return node;
    }
    ++ptr;
    if(ptr<tok_list_size) node.add_child(calculation());
    else
    {
        error_info(error_line,lack_scalar);
        ++error;
    }
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
        error_info(error_line,lack_left_curve);
        ++error;
        return node;
    }
    if(tok_list[ptr].type==tok_left_curve)
    {
        node.add_child(args_list_gen());
        ++ptr;
    }
    if(ptr>=tok_list_size)
    {
        error_info(error_line,lack_left_brace);
        ++error;
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
    while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_curve)
    {
        nasal_ast tmp;
        ++ptr;
        if(tok_list[ptr].type!=tok_identifier)
        {
            ++error;
            error_info(tok_list[ptr].line,lack_identifier);
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
        if(ptr>=tok_list_size || (tok_list[ptr].type!=tok_comma && tok_list[ptr].type!=tok_right_curve))
        {
            ++error;
            error_info(error_line,lack_comma);
            --ptr;
        }
    }
    return node;
}
nasal_ast nasal_parse::expr()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    switch(tok_list[ptr].type)
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
        default: error_info(error_line,error_token,tok_list[ptr].str);++error;break;
    }
    return node;
}
nasal_ast nasal_parse::exprs_gen()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_block);
    if(tok_list[ptr].type==tok_left_brace)
    {
        int left_brace_line=tok_list[ptr].line;
        while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_brace)
        {
            node.add_child(expr());
            ++ptr;
            if(ptr<tok_list_size && tok_list[ptr].type==tok_semi) ++ptr;
            else if(ptr<tok_list_size && (node.get_children().empty() || !check_function_end(node.get_children().back())))
            {
                ++error;
                error_info(error_line,lack_semi);
            }
        }
        if(ptr>=tok_list_size)
        {
            error_info(left_brace_line,exprs_lack_rbrace);
            ++error;
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
    node=or_expr();
    ++ptr;
    if(ptr<tok_list_size && tok_list[ptr].type==tok_quesmark)
    {
        nasal_ast tmp;
        tmp.set_line(tok_list[ptr].line);
        tmp.set_type(ast_trinocular);
        tmp.add_child(node);
        ++ptr;
        if(ptr<tok_list_size) tmp.add_child(calculation());
        else{ ++error; error_info(error_line,lack_calculation);}
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_colon)
        {
            ++error;
            error_info(error_line,lack_colon);
            return node;
        }
        ++ptr;
        if(ptr<tok_list_size) tmp.add_child(calculation());
        else{ ++error; error_info(error_line,lack_calculation);}
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
        if(ptr<tok_list_size) tmp.add_child(calculation());
        else{ ++error; error_info(error_line,lack_calculation);}
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
        if(ptr<tok_list_size) tmp.add_child(and_expr());
        else{ ++error; error_info(error_line,lack_calculation);}
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
        if(ptr<tok_list_size) tmp.add_child(cmp_expr());
        else{ ++error; error_info(error_line,lack_calculation);}
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
        if(ptr<tok_list_size) tmp.add_child(additive_expr());
        else{ ++error; error_info(error_line,lack_calculation);}
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
        if(ptr<tok_list_size) tmp.add_child(multive_expr());
        else{ ++error; error_info(error_line,lack_calculation);}
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
            ++error;
            error_info(error_line,lack_calculation);
            break;
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
    if(ptr<tok_list_size) node.add_child(scalar());
    else{ ++error; error_info(error_line,lack_calculation);}
    return node;
}
nasal_ast nasal_parse::scalar()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    if(tok_list[ptr].type==tok_number)
        node=number_gen();
    else if(tok_list[ptr].type==tok_string)
        node=string_gen();
    else if(tok_list[ptr].type==tok_identifier)
        node=id_gen();
    else if(tok_list[ptr].type==tok_func)
        node=func_gen();
    else if(tok_list[ptr].type==tok_left_bracket)
        node=vector_gen();
    else if(tok_list[ptr].type==tok_left_brace)
        node=hash_gen();
    else if(tok_list[ptr].type==tok_left_curve)
    {
        ++ptr;
        if(ptr<tok_list_size) node=calculation();
        else{ ++error; error_info(node.get_line(),lack_calculation);}
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        {
            ++error;
            error_info(error_line,lack_right_curve);
        }
    }
    else
    {
        ++error;
        error_info(error_line,lack_scalar);
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
        node.add_child(call_scalar());
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
    {
        ++error;
        error_info(error_line,lack_identifier);
    }
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
        if(ptr<tok_list_size && tok_list[ptr].type==tok_comma) ++ptr;
        else if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_bracket)
        {
            ++error;
            error_info(error_line,lack_comma);
            break;
        }
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_bracket)
    {
        ++error;
        error_info(error_line,lack_right_bracket);
    }
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
        if(ptr<tok_list_size && tok_list[ptr].type==tok_comma) ++ptr;
        else if(ptr>=tok_list_size || tok_list[ptr].type!=tok_comma || tok_list[ptr].type!=tok_right_curve)
        {
            ++error;
            error_info(error_line,lack_comma);
        }
    }
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
    {
        ++error;
        error_info(error_line,lack_right_curve);
    }
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
            else tmp.add_child(calculation());
            node=tmp;
        }
    }
    else --ptr;
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
        // unfinished
    }
    else if(tok_list[ptr].type==tok_left_curve)
    {
        ++ptr;
        node.add_child(multi_id());
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        {
            ++error;
            error_info(error_line,lack_right_curve);
            return node;
        }
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_equal)
        {
            ++error;
            error_info(error_line,lack_equal);
            return node;
        }
        ++ptr;
        // unfinished
    }
    return node;
}
nasal_ast nasal_parse::multi_id()
{
    nasal_ast node;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_identifier)
    {
        ++error;
        error_info(error_line,lack_identifier);
        return node;
    }
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_multi_id);
    node.add_child(id_gen());
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type==tok_comma)
    {
        ++ptr;
        if(ptr<tok_list_size && tok_list[ptr].type==tok_identifier)
            node.add_child(id_gen());
        else
        {
            ++error;
            error_info(error_line,lack_identifier);
            break;
        }
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::multi_scalar()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::multi_assgin()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::loop()
{
    nasal_ast node;
    if(ptr>=tok_list_size)
    {
        ++error;
        error_info(error_line,lack_token,"loop");
        return node;
    }
    switch(tok_list[ptr].type)
    {
        case tok_while:   node=while_loop(); break;
        case tok_for:     node=for_loop();   break;
        case tok_forindex:
        case tok_foreach: node=forei_loop(); break;
    }
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
        ++error;
        error_info(error_line,lack_left_curve);
    }
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
    {
        ++error;
        error_info(error_line,lack_right_curve);
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
        ++error;
        error_info(error_line,lack_left_curve);
    }
    ++ptr;
    // unfinished
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
        ++error;
        error_info(error_line,lack_left_curve);
    }
    ++ptr;
    // unfinished
    return node;
}
nasal_ast nasal_parse::conditional()
{
    nasal_ast node;
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_if);
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
    }
    return node;
}
#endif
