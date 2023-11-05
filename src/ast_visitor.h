#pragma once

#include "nasal_ast.h"

namespace nasal {

class ast_visitor {
public:
    virtual bool visit_expr(expr*);
    virtual bool visit_use_stmt(use_stmt*);
    virtual bool visit_call(call*);
    virtual bool visit_null_expr(null_expr*);
    virtual bool visit_nil_expr(nil_expr*);
    virtual bool visit_number_literal(number_literal*);
    virtual bool visit_string_literal(string_literal*);
    virtual bool visit_identifier(identifier*);
    virtual bool visit_bool_literal(bool_literal*);
    virtual bool visit_vector_expr(vector_expr*);
    virtual bool visit_hash_expr(hash_expr*);
    virtual bool visit_hash_pair(hash_pair*);
    virtual bool visit_function(function*);
    virtual bool visit_code_block(code_block*);
    virtual bool visit_parameter(parameter*);
    virtual bool visit_ternary_operator(ternary_operator*);
    virtual bool visit_binary_operator(binary_operator*);
    virtual bool visit_unary_operator(unary_operator*);
    virtual bool visit_call_expr(call_expr*);
    virtual bool visit_call_hash(call_hash*);
    virtual bool visit_call_vector(call_vector*);
    virtual bool visit_call_function(call_function*);
    virtual bool visit_slice_vector(slice_vector*);
    virtual bool visit_definition_expr(definition_expr*);
    virtual bool visit_assignment_expr(assignment_expr*);
    virtual bool visit_multi_identifier(multi_identifier*);
    virtual bool visit_tuple_expr(tuple_expr*);
    virtual bool visit_multi_assign(multi_assign*);
    virtual bool visit_while_expr(while_expr*);
    virtual bool visit_for_expr(for_expr*);
    virtual bool visit_iter_expr(iter_expr*);
    virtual bool visit_forei_expr(forei_expr*);
    virtual bool visit_condition_expr(condition_expr*);
    virtual bool visit_if_expr(if_expr*);
    virtual bool visit_continue_expr(continue_expr*);
    virtual bool visit_break_expr(break_expr*);
    virtual bool visit_return_expr(return_expr*);
};

}
