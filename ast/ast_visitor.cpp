#include "ast_visitor.h"

bool ast_visitor::visit_expr(expr* node) {
    node->accept(this);
    return true;
}

bool ast_visitor::visit_null_expr(null_expr* node) {
    return true;
}

bool ast_visitor::visit_nil_expr(nil_expr* node) {
    return true;
}

bool ast_visitor::visit_number_literal(number_literal* node) {
    return true;
}

bool ast_visitor::visit_string_literal(string_literal* node) {
    return true;
}

bool ast_visitor::visit_identifier(identifier* node) {
    return true;
}

bool ast_visitor::visit_bool_literal(bool_literal* node) {
    return true;
}

bool ast_visitor::visit_vector_expr(vector_expr* node) {
    for(auto i : node->get_elements()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_hash_expr(hash_expr* node) {
    for(auto i : node->get_members()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_hash_pair(hash_pair* node) {
    if (node->get_value()) {
        node->get_value()->accept(this);
    }
    return true;
}

bool ast_visitor::visit_function(function* node) {
    for(auto i : node->get_parameter_list()) {
        i->accept(this);
    }
    node->get_code_block()->accept(this);
    return true;
}

bool ast_visitor::visit_code_block(code_block* node) {
    for(auto i : node->get_expressions()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_parameter(parameter* node) {
    node->get_parameter_name()->accept(this);
    if (node->get_default_value()) {
        node->get_default_value()->accept(this);
    }
    return true;
}

bool ast_visitor::visit_ternary_operator(ternary_operator* node) {
    node->get_condition()->accept(this);
    node->get_left()->accept(this);
    node->get_right()->accept(this);
    return true;
}

bool ast_visitor::visit_binary_operator(binary_operator* node) {
    node->get_left()->accept(this);
    node->get_right()->accept(this);
    return true;
}

bool ast_visitor::visit_unary_operator(unary_operator* node) {
    node->get_value()->accept(this);
    return true;
}

bool ast_visitor::visit_call_expr(call_expr* node) {
    node->get_first()->accept(this);
    for(auto i : node->get_calls()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_call_hash(call_hash* node) {
    return true;
}

bool ast_visitor::visit_call_vector(call_vector* node) {
    for(auto i : node->get_slices()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_call_function(call_function* node) {
    for(auto i : node->get_argument()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_slice_vector(slice_vector* node) {
    node->get_begin()->accept(this);
    if (node->get_end()) {
        node->get_end()->accept(this);
    }
    return true;
}

bool ast_visitor::visit_definition_expr(definition_expr* node) {
    if (node->get_variable_name()) {
        node->get_variable_name()->accept(this);
    } else {
        node->get_variables()->accept(this);
    }
    node->get_value()->accept(this);
    return true;
}

bool ast_visitor::visit_assignment_expr(assignment_expr* node) {
    node->get_left()->accept(this);
    node->get_right()->accept(this);
    return true;
}

bool ast_visitor::visit_multi_identifier(multi_identifier* node) {
    for(auto i : node->get_variables()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_tuple_expr(tuple_expr* node) {
    for(auto i : node->get_elements()) {
        i->accept(this);
    }
    return true;
}

bool ast_visitor::visit_multi_assign(multi_assign* node) {
    node->get_tuple()->accept(this);
    node->get_value()->accept(this);
    return true;
}

bool ast_visitor::visit_while_expr(while_expr* node) {
    node->get_condition()->accept(this);
    node->get_code_block()->accept(this);
    return true;
}

bool ast_visitor::visit_for_expr(for_expr* node) {
    node->get_initial()->accept(this);
    node->get_condition()->accept(this);
    node->get_step()->accept(this);
    node->get_code_block()->accept(this);
    return true;
}

bool ast_visitor::visit_iter_expr(iter_expr* node) {
    if (node->get_name()) {
        node->get_name()->accept(this);
    } else {
        node->get_call()->accept(this);
    }
    return true;
}

bool ast_visitor::visit_forei_expr(forei_expr* node) {
    node->get_iterator()->accept(this);
    node->get_value()->accept(this);
    node->get_code_block()->accept(this);
    return true;
}

bool ast_visitor::visit_condition_expr(condition_expr* node) {
    node->get_if_statement()->accept(this);
    for(auto i : node->get_elsif_stataments()) {
        i->accept(this);
    }
    if (node->get_else_statement()) {
        node->get_else_statement()->accept(this);
    }
    return true;
}

bool ast_visitor::visit_if_expr(if_expr* node) {
    if (node->get_condition()) {
        node->get_condition()->accept(this);
    }
    node->get_code_block()->accept(this);
    return true;
}

bool ast_visitor::visit_continue_expr(continue_expr* node) {
    return true;
}

bool ast_visitor::visit_break_expr(break_expr* node) {
    return true;
}

bool ast_visitor::visit_return_expr(return_expr* node) {
    if (node->get_value()) {
        node->get_value()->accept(this);
    }
    return true;
}