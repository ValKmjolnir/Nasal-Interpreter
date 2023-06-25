#include "ast_dumper.h"

#include <iostream>

bool ast_dumper::visit_null_expr(null_expr* node) {
    dump_indent();
    std::cout << "null" << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_nil_expr(nil_expr* node) {
    dump_indent();
    std::cout << "nil" << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_number_literal(number_literal* node) {
    dump_indent();
    std::cout << "number " << node->get_number();
    std::cout << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_string_literal(string_literal* node) {
    dump_indent();
    std::cout << "string \"" << rawstr(node->get_content()) << "\"";
    std::cout << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_identifier(identifier* node) {
    dump_indent();
    std::cout << "identifier " << node->get_name();
    std::cout << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_bool_literal(bool_literal* node) {
    dump_indent();
    std::cout << "bool " << node->get_flag();
    std::cout << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_vector_expr(vector_expr* node) {
    dump_indent();
    std::cout << "vector";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_elements()) {
        if (i==node->get_elements().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_hash_expr(hash_expr* node) {
    dump_indent();
    std::cout << "hash";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_members()) {
        if (i==node->get_members().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_hash_pair(hash_pair* node) {
    dump_indent();
    std::cout << "pair " << node->get_name();
    std::cout << format_location(node->get_location());
    if (node->get_value()) {
        push_indent();
        set_last();
        node->get_value()->accept(this);
        pop_indent();
    }
    return true;
}

bool ast_dumper::visit_function(function* node) {
    dump_indent();
    std::cout << "function";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_parameter_list()) {
        i->accept(this);
    }
    set_last();
    node->get_code_block()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_code_block(code_block* node) {
    dump_indent();
    std::cout << "block";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_expressions()) {
        if (i==node->get_expressions().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_parameter(parameter* node) {
    dump_indent();
    std::cout << "parameter";
    std::cout << format_location(node->get_location());
    push_indent();
    if (!node->get_default_value()) {
        set_last();
    }
    node->get_parameter_name()->accept(this);
    if (node->get_default_value()) {
        set_last();
        node->get_default_value()->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_ternary_operator(ternary_operator* node) {
    dump_indent();
    std::cout << "ternary";
    std::cout << format_location(node->get_location());
    push_indent();
    node->get_condition()->accept(this);
    node->get_left()->accept(this);
    set_last();
    node->get_right()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_binary_operator(binary_operator* node) {
    dump_indent();
    std::cout << "binary";
    std::cout << format_location(node->get_location());
    push_indent();
    node->get_left()->accept(this);
    set_last();
    node->get_right()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_unary_operator(unary_operator* node) {
    dump_indent();
    std::cout << "unary";
    std::cout << format_location(node->get_location());
    push_indent();
    set_last();
    node->get_value()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_call_expr(call_expr* node) {
    dump_indent();
    std::cout << "call_expr";
    std::cout << format_location(node->get_location());
    push_indent();
    if (!node->get_calls().size()) {
        set_last();
    }
    node->get_first()->accept(this);
    for(auto i : node->get_calls()) {
        if (i==node->get_calls().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_call_hash(call_hash* node) {
    dump_indent();
    std::cout << "call_hash" << node->get_field();
    std::cout << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_call_vector(call_vector* node) {
    dump_indent();
    std::cout << "call_vector";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_slices()) {
        if (i==node->get_slices().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_call_function(call_function* node) {
    dump_indent();
    std::cout << "call_function";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_argument()) {
        if (i==node->get_argument().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_slice_vector(slice_vector* node) {
    dump_indent();
    std::cout << "slice";
    std::cout << format_location(node->get_location());
    push_indent();
    if (!node->get_end()) {
        set_last();
    }
    node->get_begin()->accept(this);
    if (node->get_end()) {
        set_last();
        node->get_end()->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_definition_expr(definition_expr* node) {
    dump_indent();
    std::cout << "definition";
    std::cout << format_location(node->get_location());
    push_indent();
    if (node->get_variable_name()) {
        node->get_variable_name()->accept(this);
    } else {
        node->get_variables()->accept(this);
    }
    set_last();
    node->get_value()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_assignment_expr(assignment_expr* node) {
    dump_indent();
    std::cout << "assignment";
    std::cout << format_location(node->get_location());
    push_indent();
    node->get_left()->accept(this);
    set_last();
    node->get_right()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_multi_identifier(multi_identifier* node) {
    dump_indent();
    std::cout << "multi_define";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_variables()) {
        if (i==node->get_variables().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_tuple_expr(tuple_expr* node) {
    dump_indent();
    std::cout << "tuple";
    std::cout << format_location(node->get_location());
    push_indent();
    for(auto i : node->get_elements()) {
        if (i==node->get_elements().back()) {
            set_last();
        }
        i->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_multi_assign(multi_assign* node) {
    dump_indent();
    std::cout << "multi_assign";
    std::cout << format_location(node->get_location());
    push_indent();
    node->get_tuple()->accept(this);
    set_last();
    node->get_value()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_while_expr(while_expr* node) {
    dump_indent();
    std::cout << "while";
    std::cout << format_location(node->get_location());
    push_indent();
    node->get_condition()->accept(this);
    set_last();
    node->get_code_block()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_for_expr(for_expr* node) {
    dump_indent();
    std::cout << "for";
    std::cout << format_location(node->get_location());
    push_indent();
    node->get_initial()->accept(this);
    node->get_condition()->accept(this);
    node->get_step()->accept(this);
    set_last();
    node->get_code_block()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_iter_expr(iter_expr* node) {
    dump_indent();
    std::cout << "iter";
    std::cout << format_location(node->get_location());
    push_indent();
    set_last();
    if (node->get_name()) {
        node->get_name()->accept(this);
    } else {
        node->get_call()->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_forei_expr(forei_expr* node) {
    dump_indent();
    if (node->get_type()==forei_expr::forei_loop_type::foreach) {
        std::cout << "foreach";
    } else {
        std::cout << "forindex";
    }
    std::cout << format_location(node->get_location());
    push_indent();
    node->get_iterator()->accept(this);
    node->get_value()->accept(this);
    set_last();
    node->get_code_block()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_condition_expr(condition_expr* node) {
    dump_indent();
    std::cout << "condition";
    std::cout << format_location(node->get_location());
    push_indent();
    if (!node->get_elsif_stataments().size() &&
        !node->get_else_statement()) {
        set_last();
    }
    node->get_if_statement()->accept(this);
    for(auto i : node->get_elsif_stataments()) {
        if (i==node->get_elsif_stataments().back() &&
            !node->get_else_statement()) {
            set_last();
        }
        i->accept(this);
    }
    if (node->get_else_statement()) {
        set_last();
        node->get_else_statement()->accept(this);
    }
    pop_indent();
    return true;
}

bool ast_dumper::visit_if_expr(if_expr* node) {
    dump_indent();
    std::cout << "if";
    std::cout << format_location(node->get_location());
    push_indent();
    if (node->get_condition()) {
        node->get_condition()->accept(this);
    }
    set_last();
    node->get_code_block()->accept(this);
    pop_indent();
    return true;
}

bool ast_dumper::visit_continue_expr(continue_expr* node) {
    dump_indent();
    std::cout << "continue";
    std::cout << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_break_expr(break_expr* node) {
    dump_indent();
    std::cout << "break";
    std::cout << format_location(node->get_location());
    return true;
}

bool ast_dumper::visit_return_expr(return_expr* node) {
    dump_indent();
    std::cout << "return";
    std::cout << format_location(node->get_location());
    if (node->get_value()) {
        push_indent();
        set_last();
        node->get_value()->accept(this);
        pop_indent();
    }
    return true;
}