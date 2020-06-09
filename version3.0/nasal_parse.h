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
    nasal_ast nil_gen();
    nasal_ast number_gen();
    nasal_ast string_gen();
    nasal_ast id_gen();
    nasal_ast vector_gen();
    nasal_ast hash_gen();
    nasal_ast hash_member_gen();
    nasal_ast func_gen();
    nasal_ast args_list_gen();
    nasal_ast exprs_gen();
    nasal_ast calculation();
    nasal_ast trinocular();
    nasal_ast and_expr();
    nasal_ast or_expr();
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
    while(ptr<tok_list_size)
    {
        if(tok_list[ptr].type);
        ++ptr;
    }
    return;
}

void nasal_parse::reset()
{
    this->ptr=0;
    this->error=0;
    this->root.clear();
    return;
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
        return node;
    }
    ++ptr;
    if(ptr<tok_list_size) node.add_child(calculation());
    else error_info(node.get_line(),lack_scalar);
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
        return node;
    }
    if(tok_list[ptr].type==tok_left_curve)
    {
        node.add_child(args_list_gen());
        ++ptr;
    }
    if(ptr>=tok_list_size)
    {
        error_info(node.get_line(),lack_left_brace);
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
nasal_ast nasal_parse::exprs_gen()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::calculation()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::trinocular()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::and_expr()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::or_expr()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::cmp_expr()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::additive_expr()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::multive_expr()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::unary()
{
    nasal_ast node;
    return node;
}
nasal_ast nasal_parse::scalar()
{
    nasal_ast node;
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

#endif