#ifndef __NASAL_ENUM_H__
#define __NASAL_ENUM_H__

/* token_type is used in lexer */
enum token_type
{
    tok_null=0,
    tok_number,tok_string,
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

#endif