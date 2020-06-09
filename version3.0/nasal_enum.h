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
    tok_add_equal,tok_sub_equal,tok_mult_equal,tok_div_equal,tok_link_equal,
    tok_cmp_equal,tok_cmp_not_equal,tok_less_than,tok_greater_than,tok_less_equal,tok_greater_equal
};

enum ast_node
{
    ast_null=0,
    ast_nil,ast_number,ast_string,ast_identifier,ast_function,ast_hash,ast_vector,
    ast_hashmember,
    ast_args,
    ast_for,ast_forindex,ast_foreach,ast_while,
    ast_definition,ast_assignment,ast_calculation
};

enum parse_error
{
    unknown,
    lack_id,
    lack_left_curve,
    lack_left_bracket,
    lack_left_brace,
    lack_comma,
    lack_colon,
    lack_scalar,
};

void error_info(int line,int error_type)
{
    std::string info=">> [parse] error: line ";
    std::string detail;
    std::cout<<info<<line<<": ";
    switch(error_type)
    {
        case unknown:           detail="unknown error.";     break;
        case lack_id:           detail="lack identifier.";   break;
        case lack_left_curve:   detail="lack left curve.";   break;
        case lack_left_bracket: detail="lack left bracket."; break;
        case lack_left_brace:   detail="lack left brace.";   break;
        case lack_comma:        detail="lack comma.";        break;
        case lack_colon:        detail="lack colon.";        break;
        case lack_scalar:       detail="lack scalar";        break;
    }
    std::cout<<detail<<std::endl;
    return;
}

#endif