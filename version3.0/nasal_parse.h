#ifndef __NASAL_PARSE_H__
#define __NAsAL_PARSE_H__

class nasal_parse
{
private:
    int tok_list_size;
    int ptr;
    int error;
    nasal_ast root;
    std::vector<token> tok_list;
    void reset();
    bool check_function_end(nasal_ast&);
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
    nasal_ast trinocular();
    nasal_ast or_expr();
    nasal_ast and_expr();
    nasal_ast cmp_expr();
    nasal_ast additive_expr();
    nasal_ast multive_expr();
    nasal_ast unary();
    nasal_ast scalar();
    nasal_ast func_call();
    nasal_ast id_call();
    nasal_ast vector_call();
    nasal_ast hash_call();
    nasal_ast call_scalar();
    nasal_ast subvec();
    nasal_ast definition();
    nasal_ast multi_id();
    nasal_ast multi_scalar();
    nasal_ast multi_assgin();
    nasal_ast loop();
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
    tok_list_size=this->tok_list.size();
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

nasal_ast nasal_parse::nil_gen()
{
    nasal_ast node;
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
        if(ptr>=tok_list_size) break;
        if(ptr<tok_list_size && tok_list[ptr].type==tok_comma) ++ptr;
        if(ptr<tok_list_size && tok_list[ptr].type!=tok_comma && tok_list[ptr].type!=tok_right_bracket)
        {
            error_info(tok_list[ptr].line,lack_comma);
            ++error;
            break;
        }
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
        if(ptr>=tok_list_size) break;
        if(ptr<tok_list_size && tok_list[ptr].type==tok_comma) ++ptr;
        if(ptr<tok_list_size && tok_list[ptr].type!=tok_comma && tok_list[ptr].type!=tok_right_brace)
        {
            error_info(tok_list[ptr].line,lack_comma);
            ++error;
            break;
        }
    }
    return node;
}
nasal_ast nasal_parse::hash_member_gen()
{
    nasal_ast node;
    if(tok_list[ptr].type!=tok_identifier)
    {
        error_info(tok_list[ptr].line,lack_id);
        return node;
    }
    node.set_line(tok_list[ptr].line);
    node.set_type(ast_hashmember);
    node.add_child(id_gen());
    ++ptr;
    if(ptr>=tok_list_size || tok_list[ptr].type!=tok_colon)
    {
        error_info(ptr>=tok_list_size?node.get_line():tok_list[ptr].line,lack_colon);
        ++error;
        return node;
    }
    ++ptr;
    if(ptr<tok_list_size) node.add_child(calculation());
    else
    {
        error_info(node.get_line(),lack_scalar);
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
        error_info(node.get_line(),lack_left_curve);
        ++error;
        return node;
    }
    if(tok_list[ptr].type==tok_left_curve)
    {
        node.add_child(args_list_gen());
        ++error;
        ++ptr;
    }
    if(ptr>=tok_list_size)
    {
        error_info(node.get_line(),lack_left_brace);
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
    ++ptr;
    while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_curve)
    {
        ;
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
        case tok_for:
        case tok_forindex:
        case tok_foreach:
        case tok_while:    node=loop();          break;
        case tok_if:       node=conditional();   break;
        case tok_continue: node=continue_expr(); break;
        case tok_break:    node=break_expr();    break;
        case tok_return:   node=return_expr();   break;
        case tok_semi:     --ptr;                break;
        default: error_info(tok_list[ptr].line,error_token);++error;break;
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
        while(ptr<tok_list_size && tok_list[ptr].type!=tok_right_brace)
        {
            node.add_child(expr());
            ++ptr;
            if(ptr<tok_list_size && tok_list[ptr].type==tok_semi) ++ptr;
            else if(ptr<tok_list_size && (node.get_children().empty() || !check_function_end(node.get_children().back())))
            {
                ++error;
                error_info(tok_list[ptr].line,lack_semi);
            }
        }
        if(ptr>=tok_list_size)
        {
            error_info(node.get_line(),lack_right_brace);
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
    node.set_line(tok_list[ptr].line);
    int tok_type=tok_list[ptr].type;
    return node;
}
nasal_ast nasal_parse::trinocular()
{
    nasal_ast node;
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
        tmp.add_child(and_expr());
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
        tmp.add_child(cmp_expr());
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
        tmp.add_child(additive_expr());
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
        tmp.add_child(multive_expr());
        node=tmp;
        ++ptr;
    }
    --ptr;
    return node;
}
nasal_ast nasal_parse::multive_expr()
{
    nasal_ast node;
    if(tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not)
        node=unary();
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
        if(ptr<tok_list_size && (tok_list[ptr].type==tok_sub || tok_list[ptr].type==tok_not))
            tmp.add_child(unary());
        else if(ptr<tok_list_size && tok_list[ptr].type!=tok_sub && tok_list[ptr].type!=tok_not)
            tmp.add_child(scalar());
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
    node.add_child(scalar());
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
    else if(
        tok_list[ptr].type==tok_func         ||
        tok_list[ptr].type==tok_identifier   ||
        tok_list[ptr].type==tok_left_bracket ||
        tok_list[ptr].type==tok_left_brace
    )
        node=call_scalar();
    else if(tok_list[ptr].type==tok_left_curve)
    {
        int curve_line=tok_list[ptr].line;
        ++ptr;
        node=calculation();
        ++ptr;
        if(ptr>=tok_list_size || tok_list[ptr].type!=tok_right_curve)
        {
            ++error;
            error_info(curve_line,lack_right_curve);
        }
    }
    return node;
}
nasal_ast nasal_parse::func_call()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::id_call()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::vector_call()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::hash_call()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::call_scalar()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::subvec()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::definition()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::multi_id()
{
    nasal_ast node;
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
    return node;
}
nasal_ast nasal_parse::conditional()
{
    nasal_ast node;
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