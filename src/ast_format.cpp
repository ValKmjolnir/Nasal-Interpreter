#include "ast_format.h"
#include "util/util.h"

#include <iostream>

namespace nasal {

bool ast_format::visit_use_stmt(use_stmt* node) {
    dump_formating_node_info(node, "use statement");
    out << "use ";
    for(auto i : node->get_path()) {
        i->accept(this);
        if (i != node->get_path().back()) {
            out << ".";
        }
    }
    return true;
}

bool ast_format::visit_null_expr(null_expr* node) {
    dump_formating_node_info(node, "null expression");
    return true;
}

bool ast_format::visit_nil_expr(nil_expr* node) {
    dump_formating_node_info(node, "nil expression");
    out << "nil";
    return true;
}

bool ast_format::visit_number_literal(number_literal* node) {
    dump_formating_node_info(node, "number expression");
    out << node->get_raw_text();
    return true;
}

bool ast_format::visit_string_literal(string_literal* node) {
    dump_formating_node_info(node, "string expression");
    out << "\"" << util::rawstr(node->get_content()) << "\"";
    return true;
}

bool ast_format::visit_identifier(identifier* node) {
    dump_formating_node_info(node, "identifier");
    out << node->get_name();
    return true;
}

bool ast_format::visit_bool_literal(bool_literal* node) {
    dump_formating_node_info(node, "bool expression");
    out << (node->get_flag()? "true" : "false");
    return true;
}

bool ast_format::visit_vector_expr(vector_expr* node) {
    dump_formating_node_info(node, "vector expression");
    out << "[";
    for(auto i : node->get_elements()) {
        i->accept(this);
        if (i != node->get_elements().back()) {
            out << ", ";
        }
    }
    out << "]";
    return true;
}

bool ast_format::visit_hash_expr(hash_expr* node) {
    dump_formating_node_info(node, "hash expression");
    out << "{";
    for(auto i : node->get_members()) {
        i->accept(this);
        if (i != node->get_members().back()) {
            out << ", ";
        }
    }
    out << "}";
    return true;
}

bool ast_format::visit_hash_pair(hash_pair* node) {
    dump_formating_node_info(node, "hash pair");
    bool contain_not_identifier = false;
    for (auto c : node->get_name()) {
        if (!(std::isdigit(c) || std::isalpha(c) || c == '_')) {
            contain_not_identifier = true;
        }
    }
    if (contain_not_identifier) {
        out << "\"" << node->get_name() << "\"";
    } else {
        out << node->get_name();
    }
    if (node->get_value()) {
        out << " : ";
        node->get_value()->accept(this);
    }
    return true;
}

bool ast_format::visit_function(function* node) {
    dump_formating_node_info(node, "function");
    out << "func(";
    for(auto i : node->get_parameter_list()) {
        i->accept(this);
        if (i != node->get_parameter_list().back()) {
            out << ", ";
        }
    }
    out << ") ";
    if (node->get_code_block()->get_expressions().empty()) {
        out << "{}";
        return true;
    }
    node->get_code_block()->accept(this);
    return true;
}

bool ast_format::visit_code_block(code_block* node) {
    dump_formating_node_info(node, "code block");
    out << "{\n";
    push_indent();
    for(auto i : node->get_expressions()) {
        dump_indent();
        i->accept(this);
        if (need_dump_semi(i)) {
            out << ";\n";
        } else {
            out << "\n";
        }
    }
    pop_indent();
    dump_indent();
    out << "}";
    return true;
}

bool ast_format::visit_parameter(parameter* node) {
    dump_formating_node_info(node, "parameter");
    out << node->get_parameter_name();
    switch (node->get_parameter_type()) {
        case parameter::kind::normal_parameter: break;
        case parameter::kind::dynamic_parameter: out << "..."; break;
        case parameter::kind::default_parameter: out << " = "; break;
    }
    if (node->get_default_value()) {
        node->get_default_value()->accept(this);
    }
    return true;
}

bool ast_format::visit_ternary_operator(ternary_operator* node) {
    dump_formating_node_info(node, "ternary operator");
    out << "(";
    node->get_condition()->accept(this);
    out << " ? ";
    node->get_left()->accept(this);
    out << " : ";
    node->get_right()->accept(this);
    out << ")";
    return true;
}

bool ast_format::visit_binary_operator(binary_operator* node) {
    dump_formating_node_info(node, "binary operator");
    out << "(";
    node->get_left()->accept(this);
    switch(node->get_operator_type()) {
        case binary_operator::kind::add: out << " + "; break;
        case binary_operator::kind::sub: out << " - "; break;
        case binary_operator::kind::mult: out << " * "; break;
        case binary_operator::kind::div: out << " / "; break;
        case binary_operator::kind::concat: out << " ~ "; break;
        case binary_operator::kind::bitwise_and: out << " & "; break;
        case binary_operator::kind::bitwise_or: out << " | "; break;
        case binary_operator::kind::bitwise_xor: out << " ^ "; break;
        case binary_operator::kind::cmpeq: out << " == "; break;
        case binary_operator::kind::cmpneq: out << " != "; break;
        case binary_operator::kind::grt: out << " > "; break;
        case binary_operator::kind::geq: out << " >= "; break;
        case binary_operator::kind::less: out << " < "; break;
        case binary_operator::kind::leq: out << " <= "; break;
        case binary_operator::kind::condition_and: out << " and "; break;
        case binary_operator::kind::condition_or: out << " or "; break;
        case binary_operator::kind::null_chain: out << " ?? "; break;
    }
    node->get_right()->accept(this);
    out << ")";
    return true;
}

bool ast_format::visit_unary_operator(unary_operator* node) {
    dump_formating_node_info(node, "unary operator");
    switch(node->get_operator_type()) {
        case unary_operator::kind::negative: out << "-"; break;
        case unary_operator::kind::logical_not: out << "!"; break;
        case unary_operator::kind::bitwise_not: out << "~"; break;
    }
    node->get_value()->accept(this);
    return true;
}

bool ast_format::visit_call_expr(call_expr* node) {
    dump_formating_node_info(node, "call expression");
    node->get_first()->accept(this);
    for(auto i : node->get_calls()) {
        i->accept(this);
    }
    return true;
}

bool ast_format::visit_call_hash(call_hash* node) {
    dump_formating_node_info(node, "call hash");
    out << "." << node->get_field();
    return true;
}

bool ast_format::visit_null_access(null_access* node) {
    dump_formating_node_info(node, "null access operator(?.)");
    out << "?." << node->get_field();
    return true;
}

bool ast_format::visit_call_vector(call_vector* node) {
    dump_formating_node_info(node, "call vector");
    out << "[";
    for(auto i : node->get_slices()) {
        i->accept(this);
        if (i != node->get_slices().back()) {
            out << ", ";
        }
    }
    out << "]";
    return true;
}

bool ast_format::visit_call_function(call_function* node) {
    dump_formating_node_info(node, "call function");
    out << "(";
    for(auto i : node->get_argument()) {
        i->accept(this);
        if (i != node->get_argument().back()) {
            out << ", ";
        }
    }
    out << ")";
    return true;
}

bool ast_format::visit_slice_vector(slice_vector* node) {
    dump_formating_node_info(node, "slice vector");
    node->get_begin()->accept(this);
    if (node->get_end()) {
        out << " : ";
        node->get_end()->accept(this);
    }
    return true;
}

bool ast_format::visit_definition_expr(definition_expr* node) {
    dump_formating_node_info(node, "definition");
    out << "var ";
    if (node->get_variable_name()) {
        node->get_variable_name()->accept(this);
    } else {
        node->get_variables()->accept(this);
    }
    out << " = ";
    if (node->get_tuple()) {
        node->get_tuple()->accept(this);
    } else {
        node->get_value()->accept(this);
    }
    return true;
}

bool ast_format::visit_assignment_expr(assignment_expr* node) {
    dump_formating_node_info(node, "assignment");
    node->get_left()->accept(this);
    switch(node->get_assignment_type()) {
        case assignment_expr::kind::add_equal: out << " += "; break;
        case assignment_expr::kind::sub_equal: out << " -= "; break;
        case assignment_expr::kind::mult_equal: out << " *= "; break;
        case assignment_expr::kind::div_equal: out << " /= "; break;
        case assignment_expr::kind::concat_equal: out << " ~= "; break;
        case assignment_expr::kind::equal: out << " = "; break;
        case assignment_expr::kind::bitwise_and_equal: out << " &= "; break;
        case assignment_expr::kind::bitwise_or_equal: out << " |= "; break;
        case assignment_expr::kind::bitwise_xor_equal: out << " ^= "; break;
    }
    node->get_right()->accept(this);
    return true;
}

bool ast_format::visit_multi_identifier(multi_identifier* node) {
    dump_formating_node_info(node, "multi identifier");
    out << "(";
    for(auto i : node->get_variables()) {
        i->accept(this);
        if (i != node->get_variables().back()) {
            out << ", ";
        }
    }
    out << ")";
    return true;
}

bool ast_format::visit_tuple_expr(tuple_expr* node) {
    dump_formating_node_info(node, "tuple expression");
    out << "(";
    for(auto i : node->get_elements()) {
        i->accept(this);
        if (i != node->get_elements().back()) {
            out << ", ";
        }
    }
    out << ")";
    return true;
}

bool ast_format::visit_multi_assign(multi_assign* node) {
    dump_formating_node_info(node, "multi assign");
    node->get_tuple()->accept(this);
    out << " = ";
    node->get_value()->accept(this);
    return true;
}

bool ast_format::visit_while_expr(while_expr* node) {
    dump_formating_node_info(node, "while statement");
    out << "while (";
    node->get_condition()->accept(this);
    out << ") ";
    node->get_code_block()->accept(this);
    return true;
}

bool ast_format::visit_for_expr(for_expr* node) {
    dump_formating_node_info(node, "for statement");
    out << "for (";
    node->get_initial()->accept(this);
    out << "; ";
    node->get_condition()->accept(this);
    out << "; ";
    node->get_step()->accept(this);
    out << ") ";
    node->get_code_block()->accept(this);
    return true;
}

bool ast_format::visit_iter_expr(iter_expr* node) {
    dump_formating_node_info(node, "iteration expression");
    if (node->is_definition()) {
        out << "var ";
    }
    if (node->get_name()) {
        node->get_name()->accept(this);
    } else {
        node->get_call()->accept(this);
    }
    return true;
}

bool ast_format::visit_forei_expr(forei_expr* node) {
    dump_formating_node_info(node, "forindex/foreach statement");
    if (node->get_loop_type()==forei_expr::kind::foreach) {
        out << "foreach ";
    } else {
        out << "forindex ";
    }
    out << "(";
    node->get_iterator()->accept(this);
    out << "; ";
    node->get_value()->accept(this);
    out << ") ";
    node->get_code_block()->accept(this);
    return true;
}

bool ast_format::visit_condition_expr(condition_expr* node) {
    dump_formating_node_info(node, "condition statement");
    out << "if ";
    node->get_if_statement()->accept(this);
    for (auto i : node->get_elsif_stataments()) {
        out << " elsif ";
        i->accept(this);
    }
    if (node->get_else_statement()) {
        out << " else ";
        node->get_else_statement()->accept(this);
    }
    return true;
}

bool ast_format::visit_if_expr(if_expr* node) {
    dump_formating_node_info(node, "if statement");
    if (node->get_condition()) {
        out << "(";
        node->get_condition()->accept(this);
        out << ") ";
    }
    node->get_code_block()->accept(this);
    return true;
}

bool ast_format::visit_continue_expr(continue_expr* node) {
    dump_formating_node_info(node, "continue statement");
    out << "continue";
    return true;
}

bool ast_format::visit_break_expr(break_expr* node) {
    dump_formating_node_info(node, "break statement");
    out << "break";
    return true;
}

bool ast_format::visit_return_expr(return_expr* node) {
    dump_formating_node_info(node, "return statement");
    out << "return ";
    if (node->get_value()) {
        node->get_value()->accept(this);
    }
    return true;
}

}
