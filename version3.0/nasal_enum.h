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
    ast_for,ast_forindex,ast_foreach,ast_while,
    ast_if,ast_elsif,ast_else,
    ast_multi_id,ast_multi_scalar,
    ast_definition,ast_multi_assign,ast_calculation,
    ast_continue,ast_break,ast_return,
};

enum parse_error
{
    unknown,
    error_token,
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
    lack_token,
};

void error_info(int line,int error_type,std::string error_str="")
{
    std::string info=">> [parse] error: line ";
    std::string detail;
    std::cout<<info<<line<<": ";
    switch(error_type)
    {
        case unknown:           detail="unknown error.";                       break;
        case error_token:       detail="error token \'"+error_str+"\'";        break;
        case lack_left_curve:   detail="expected \'(\'.";                      break;
        case lack_right_curve:  detail="expected \')\'.";                      break;
        case lack_left_bracket: detail="expected \'[\'.";                      break;
        case lack_right_bracket:detail="expected \']\'.";                      break;
        case lack_left_brace:   detail="expected \'{\'.";                      break;
        case lack_right_brace:  detail="expected \'}\'.";                      break;
        case exprs_lack_rbrace:  detail="expected \'}\' with this line\'s \'{\'.";break;
        case lack_semi:         detail="expected \';\'.";                      break;
        case lack_comma:        detail="expected \',\'.";                      break;
        case lack_colon:        detail="expected \':\'.";                      break;
        case lack_equal:        detail="expected \'=\'.";                      break;
        case lack_scalar:       detail="expected scalar here.";                break;
        case lack_identifier:   detail="expected identifier here.";            break;
        case lack_calculation:  detail="expected arithmetic-expression here."; break;
        case lack_token:        detail="expected \'"+error_str+"\' here.";     break;
    }
    std::cout<<detail<<std::endl;
    return;
}

#endif