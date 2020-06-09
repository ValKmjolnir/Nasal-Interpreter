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
    nasal_ast scalar_gen();
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

#endif