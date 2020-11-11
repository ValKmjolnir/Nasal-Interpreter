#ifndef __NASAL_ENUM_H__
#define __NASAL_ENUM_H__

/* token_type is used in lexer */
enum token_type
{
    tok_null=0,
    tok_number,tok_string,tok_identifier,
    tok_for,tok_forindex,tok_foreach,tok_while,
    tok_var,tok_func,tok_break,tok_continue,
    tok_return,tok_if,tok_elsif,tok_else,tok_nil,
    tok_left_curve,tok_right_curve,
    tok_left_bracket,tok_right_bracket,
    tok_left_brace,tok_right_brace,
    tok_semi,tok_and,tok_or,tok_comma,tok_dot,tok_ellipsis,tok_quesmark,
    tok_colon,tok_add,tok_sub,tok_mult,tok_div,tok_link,tok_not,
    tok_equal,
    tok_add_equal,tok_sub_equal,tok_mult_equal,tok_div_equal,tok_link_equal,
    tok_cmp_equal,tok_cmp_not_equal,tok_less_than,tok_greater_than,tok_less_equal,tok_greater_equal
};

enum ast_node
{
    ast_null=0,ast_root,ast_block,
    ast_nil,ast_number,ast_string,ast_identifier,ast_function,ast_hash,ast_vector,
    ast_hashmember,ast_call,ast_call_hash,ast_call_vec,ast_call_func,ast_subvec,
    ast_args,ast_default_arg,ast_dynamic_id,
    ast_and,ast_or,
    ast_equal,ast_add_equal,ast_sub_equal,ast_mult_equal,ast_div_equal,ast_link_equal,
    ast_cmp_equal,ast_cmp_not_equal,ast_less_than,ast_less_equal,ast_greater_than,ast_greater_equal,
    ast_add,ast_sub,ast_mult,ast_div,ast_link,
    ast_unary_sub,ast_unary_not,
    ast_trinocular,
    ast_for,ast_forindex,ast_foreach,ast_while,ast_new_iter,
    ast_conditional,ast_if,ast_elsif,ast_else,
    ast_multi_id,ast_multi_scalar,
    ast_definition,ast_multi_assign,
    ast_continue,ast_break,ast_return
};

std::string ast_str(int type)
{
    std::string str;
    switch(type)
    {
        case ast_null:         str="null";break;
        case ast_root:         str="root";break;
        case ast_block:        str="block";break;
        case ast_nil:          str="nil";break;
        case ast_number:       str="number";break;
        case ast_string:       str="string";break;
        case ast_identifier:   str="id";break;
        case ast_function:     str="function";break;
        case ast_hash:         str="hash";break;
        case ast_vector:       str="vector";break;
        case ast_hashmember:   str="hashmember";break;
        case ast_call:         str="call";break;
        case ast_call_hash:    str="call_hash";break;
        case ast_call_vec:     str="call_vector";break;
        case ast_call_func:    str="call_func";break;
        case ast_subvec:       str="subvec";break;
        case ast_args:         str="arguments";break;
        case ast_default_arg:  str="default_arg";break;
        case ast_dynamic_id:   str="dynamic_id";break;
        case ast_and:          str="and";break;
        case ast_or:           str="or";break;
        case ast_equal:        str="=";break;
        case ast_add_equal:    str="+=";break;
        case ast_sub_equal:    str="-=";break;
        case ast_mult_equal:   str="*=";break;
        case ast_div_equal:    str="/=";break;
        case ast_link_equal:   str="~=";break;
        case ast_cmp_equal:    str="==";break;
        case ast_cmp_not_equal:str="!=";break;
        case ast_less_than:    str="<";break;
        case ast_less_equal:   str="<=";break;
        case ast_greater_than: str=">";break;
        case ast_greater_equal:str=">=";break;
        case ast_add:          str="+";break;
        case ast_sub:          str="-";break;
        case ast_mult:         str="*";break;
        case ast_div:          str="/";break;
        case ast_link:         str="~";break;
        case ast_unary_sub:    str="unary-";break;
        case ast_unary_not:    str="unary!";break;
        case ast_trinocular:   str="trinocular";break;
        case ast_for:          str="for";break;
        case ast_forindex:     str="forindex";break;
        case ast_foreach:      str="foreach";break;
        case ast_while:        str="while";break;
        case ast_new_iter:     str="new_iterator";break;
        case ast_conditional:  str="conditional";break;
        case ast_if:           str="if";break;
        case ast_elsif:        str="elsif";break;
        case ast_else:         str="else";break;
        case ast_multi_id:     str="multi_id";break;
        case ast_multi_scalar: str="multi_scalar";break;
        case ast_definition:   str="definition";break;
        case ast_multi_assign: str="multi_assignment";break;
        case ast_continue:     str="continue";break;
        case ast_break:        str="break";break;
        case ast_return:       str="return";break;
    }
    return str;
}

enum parse_error
{
    error_token,
    error_expr,
    lack_left_curve,
    lack_right_curve,
    lack_left_bracket,
    lack_right_bracket,
    lack_left_brace,
    lack_right_brace,
    exprs_lack_rbrace,
    lack_semi,
    lack_comma,
    lack_colon,
    lack_equal,
    lack_scalar,
    lack_identifier,
    lack_calculation,
    lack_exprs,
    lack_token,
    lack_args,
    default_arg_not_end,
    dynamic_id_not_end,
    name_repetition,
    definition_use_call,
    multi_id_use_call,
    multi_assign_lack_val,
    lack_definition,
    lack_loop_iter,
    lack_func_content
};

void error_info(int line,int error_type,std::string error_str="")
{
    std::string detail;
    std::cout<<">> [parse] line "<<line<<": ";
    switch(error_type)
    {
        case error_token:          std::cout<<"error token \""+error_str+"\".\n";       break;
        case error_expr:           std::cout<<"error expression \""+error_str+"\".\n";  break;
        case lack_left_curve:      std::cout<<"expected \"(\".\n";                      break;
        case lack_right_curve:     std::cout<<"expected \")\".\n";                      break;
        case lack_left_bracket:    std::cout<<"expected \"[\".\n";                      break;
        case lack_right_bracket:   std::cout<<"expected \"]\".\n";                      break;
        case lack_left_brace:      std::cout<<"expected \"{\".\n";                      break;
        case lack_right_brace:     std::cout<<"expected \"}\".\n";                      break;
        case exprs_lack_rbrace:    std::cout<<"expected \"}\" with this line\'s \"{\".\n";break;
        case lack_semi:            std::cout<<"expected \";\".\n";                      break;
        case lack_comma:           std::cout<<"expected \",\".\n";                      break;
        case lack_colon:           std::cout<<"expected \":\".\n";                      break;
        case lack_equal:           std::cout<<"expected \"=\".\n";                      break;
        case lack_scalar:          std::cout<<"expected scalar here.\n";                break;
        case lack_identifier:      std::cout<<"expected identifier here.\n";            break;
        case lack_calculation:     std::cout<<"expected arithmetic-expression here.\n"; break;
        case lack_exprs:           std::cout<<"expected expression block here.\n";      break;
        case lack_token:           std::cout<<"expected \""+error_str+"\" here.\n";     break;
        case lack_args:            std::cout<<"expected arguments here.\n";             break;
        case default_arg_not_end:  std::cout<<"default argument missing for parameter of "+error_str+".\n";break;
        case dynamic_id_not_end:   std::cout<<"dynamic id must be the end of "+error_str+".\n";break;
        case name_repetition:      std::cout<<"this identifier name has existed.\n";break;
        case definition_use_call:  std::cout<<"should not use call_scalar in definition progress.\n";break;
        case multi_id_use_call:    std::cout<<"should not use call_scalar in multi_id progress.\n";break;
        case multi_assign_lack_val:std::cout<<"multi-assignment lacks value list.\n";break;
        case lack_definition:      std::cout<<"expected a definition expression here.\n";break;
        case lack_loop_iter:       std::cout<<"expected an iterator to loop through.\n";break;
        case lack_func_content:    std::cout<<"expected arguments or expression block here.\n";break;
    }
    return;
}

enum runtime_scalar_type
{
    vm_nil=0,
    vm_number,
    vm_string,
    vm_closure,
    vm_function,
    vm_vector,
    vm_hash
};

#endif