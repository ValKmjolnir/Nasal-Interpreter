#include "ast_dumper.h"

#include <iostream>

bool ast_dumper::visit_expr(expr* node) {
    std::cout << "expr\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->accept(this);
    return true;
}

bool ast_dumper::visit_null_expr(null_expr* node) {
    std::cout << "null\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_nil_expr(nil_expr* node) {
    std::cout << "nil\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_number_literal(number_literal* node) {
    std::cout << "number " << node->get_number() << "\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_string_literal(string_literal* node) {
    std::cout << "string " << node->get_content() << "\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_identifier(identifier* node) {
    std::cout << "identifier " << node->get_name() << "\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_bool_literal(bool_literal* node) {
    std::cout << "bool " << node->get_flag() << "\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_vector_expr(vector_expr* node) {
    std::cout << "vector\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_elements()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_hash_expr(hash_expr* node) {
    std::cout << "hash\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_members()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_hash_pair(hash_pair* node) {
    std::cout << "pair\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    if (node->get_value()) {
        node->get_value()->accept(this);
    }
    return true;
}

bool ast_dumper::visit_function(function* node) {
    std::cout << "function\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_parameter_list()) {
        i->accept(this);
    }
    node->get_code_block()->accept(this);
    return true;
}

bool ast_dumper::visit_code_block(code_block* node) {
    std::cout << "block\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_expressions()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_parameter(parameter* node) {
    std::cout << "parameter\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_parameter_name()->accept(this);
    if (node->get_default_value()) {
        node->get_default_value()->accept(this);
    }
    return true;
}

bool ast_dumper::visit_ternary_operator(ternary_operator* node) {
    std::cout << "ternary\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_condition()->accept(this);
    node->get_left()->accept(this);
    node->get_right()->accept(this);
    return true;
}

bool ast_dumper::visit_binary_operator(binary_operator* node) {
    std::cout << "binary\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_left()->accept(this);
    node->get_right()->accept(this);
    return true;
}

bool ast_dumper::visit_unary_operator(unary_operator* node) {
    std::cout << "unary\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_value()->accept(this);
    return true;
}

bool ast_dumper::visit_call_expr(call_expr* node) {
    std::cout << "call_expr\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_first()->accept(this);
    for(auto i : node->get_calls()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_call_hash(call_hash* node) {
    std::cout << "call_hash\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_call_vector(call_vector* node) {
    std::cout << "call_vector\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_slices()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_call_function(call_function* node) {
    std::cout << "call_function\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_argument()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_slice_vector(slice_vector* node) {
    std::cout << "slice\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_begin()->accept(this);
    if (node->get_end()) {
        node->get_end()->accept(this);
    }
    return true;
}

bool ast_dumper::visit_definition_expr(definition_expr* node) {
    std::cout << "definition\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    if (node->get_variable_name()) {
        node->get_variable_name()->accept(this);
    } else {
        node->get_variables()->accept(this);
    }
    node->get_value()->accept(this);
    return true;
}

bool ast_dumper::visit_assignment_expr(assignment_expr* node) {
    std::cout << "assignment\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_left()->accept(this);
    node->get_right()->accept(this);
    return true;
}

bool ast_dumper::visit_multi_identifier(multi_identifier* node) {
    std::cout << "multi_define\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_variables()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_tuple_expr(tuple_expr* node) {
    std::cout << "tuple\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    for(auto i : node->get_elements()) {
        i->accept(this);
    }
    return true;
}

bool ast_dumper::visit_multi_assign(multi_assign* node) {
    std::cout << "multi_assign\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_tuple()->accept(this);
    node->get_value()->accept(this);
    return true;
}

bool ast_dumper::visit_while_expr(while_expr* node) {
    std::cout << "while\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_condition()->accept(this);
    node->get_code_block()->accept(this);
    return true;
}

bool ast_dumper::visit_for_expr(for_expr* node) {
    std::cout << "for\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_initial()->accept(this);
    node->get_condition()->accept(this);
    node->get_step()->accept(this);
    node->get_code_block()->accept(this);
    return true;
}

bool ast_dumper::visit_iter_expr(iter_expr* node) {
    std::cout << "iter\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    if (node->get_name()) {
        node->get_name()->accept(this);
    } else {
        node->get_call()->accept(this);
    }
    return true;
}

bool ast_dumper::visit_forei_expr(forei_expr* node) {
    std::cout << "forei\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_iterator()->accept(this);
    node->get_value()->accept(this);
    node->get_code_block()->accept(this);
    return true;
}

bool ast_dumper::visit_condition_expr(condition_expr* node) {
    std::cout << "condition\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    node->get_if_statement()->accept(this);
    for(auto i : node->get_elsif_stataments()) {
        i->accept(this);
    }
    if (node->get_else_statement()) {
        node->get_else_statement()->accept(this);
    }
    return true;
}

bool ast_dumper::visit_if_expr(if_expr* node) {
    std::cout << "if\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    if (node->get_condition()) {
        node->get_condition()->accept(this);
    }
    node->get_code_block()->accept(this);
    return true;
}

bool ast_dumper::visit_continue_expr(continue_expr* node) {
    std::cout << "continue\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_break_expr(break_expr* node) {
    std::cout << "break\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    return true;
}

bool ast_dumper::visit_return_expr(return_expr* node) {
    std::cout << "return\n";
    std::cout << " -> " << node->get_location().file << ":"
    << node->get_location().begin_line << ":"
    << node->get_location().begin_column << "\n";
    if (node->get_value()) {
        node->get_value()->accept(this);
    }
    return true;
}