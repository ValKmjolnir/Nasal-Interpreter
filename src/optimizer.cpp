#include "optimizer.h"

void optimizer::const_string(
    binary_operator* node,
    string_literal* left_node,
    string_literal* right_node) {
    if (node->get_operator_type()!=binary_operator::binary_type::concat) {
        return;
    }
    const auto& left = left_node->get_content();
    const auto& right = right_node->get_content();
    node->set_optimized_string(
        new string_literal(node->get_location(), left+right));
}

void optimizer::const_number(
    binary_operator* node,
    number_literal* left_node,
    number_literal* right_node) {
    const auto left = left_node->get_number();
    const auto right = right_node->get_number();
    f64 res;
    switch(node->get_operator_type()) {
        case binary_operator::binary_type::add: res = left+right; break;
        case binary_operator::binary_type::sub: res = left-right; break;
        case binary_operator::binary_type::mult: res = left*right; break;
        case binary_operator::binary_type::div: res = left/right; break;
        case binary_operator::binary_type::less: res = left<right; break;
        case binary_operator::binary_type::leq: res = left<=right; break;
        case binary_operator::binary_type::grt: res = left>right; break;
        case binary_operator::binary_type::geq: res = left>=right; break;
        case binary_operator::binary_type::bitwise_or:
            res = i32(left)|i32(right); break;
        case binary_operator::binary_type::bitwise_xor:
            res = i32(left)^i32(right); break;
        case binary_operator::binary_type::bitwise_and:
            res = i32(left)&i32(right); break;
        default: return;
    }
    if (std::isinf(res) || std::isnan(res)) {
        return;
    }
    node->set_optimized_number(
        new number_literal(node->get_location(), res));
}

void optimizer::const_number(
    unary_operator* node,
    number_literal* value_node) {
    auto res = value_node->get_number();
    switch(node->get_operator_type()) {
        case unary_operator::unary_type::negative: res = -res; break;
        case unary_operator::unary_type::bitwise_not: res = ~i32(res); break;
        case unary_operator::unary_type::logical_not: res = !res; break;
    }
    if (std::isinf(res) || std::isnan(res)) {
        return;
    }
    node->set_optimized_number(
        new number_literal(node->get_location(), res));
}

bool optimizer::visit_binary_operator(binary_operator* node) {
    node->get_left()->accept(this);
    node->get_right()->accept(this);
    number_literal* left_num_node = nullptr;
    number_literal* right_num_node = nullptr;
    string_literal* left_str_node = nullptr;
    string_literal* right_str_node = nullptr;
    if (node->get_left()->get_type()==expr_type::ast_num) {
        left_num_node = (number_literal*)node->get_left();
    } else if (node->get_left()->get_type()==expr_type::ast_binary &&
        ((binary_operator*)node->get_left())->get_optimized_number()) {
        left_num_node = ((binary_operator*)node->get_left())->get_optimized_number();
    } else if (node->get_left()->get_type()==expr_type::ast_unary &&
        ((unary_operator*)node->get_left())->get_optimized_number()) {
        left_num_node = ((unary_operator*)node->get_left())->get_optimized_number();
    }
    if (node->get_right()->get_type()==expr_type::ast_num) {
        right_num_node = (number_literal*)node->get_right();
    } else if (node->get_right()->get_type()==expr_type::ast_binary &&
        ((binary_operator*)node->get_right())->get_optimized_number()) {
        right_num_node = ((binary_operator*)node->get_right())->get_optimized_number();
    } else if (node->get_right()->get_type()==expr_type::ast_unary &&
        ((unary_operator*)node->get_right())->get_optimized_number()) {
        right_num_node = ((unary_operator*)node->get_right())->get_optimized_number();
    }

    if (node->get_left()->get_type()==expr_type::ast_str) {
        left_str_node = (string_literal*)node->get_left();
    } else if (node->get_left()->get_type()==expr_type::ast_binary &&
        ((binary_operator*)node->get_left())->get_optimized_string()) {
        left_str_node = ((binary_operator*)node->get_left())->get_optimized_string();
    }
    if (node->get_right()->get_type()==expr_type::ast_str) {
        right_str_node = (string_literal*)node->get_right();
    } else if (node->get_right()->get_type()==expr_type::ast_binary &&
        ((binary_operator*)node->get_right())->get_optimized_string()) {
        right_str_node = ((binary_operator*)node->get_right())->get_optimized_string();
    }
    if (left_num_node && right_num_node) {
        const_number(node, left_num_node, right_num_node);
        return true;
    }
    if (left_str_node && right_str_node) {
        const_string(node, left_str_node, right_str_node);
        return true;
    }
    return true;
}

bool optimizer::visit_unary_operator(unary_operator* node) {
    node->get_value()->accept(this);
    number_literal* value_node = nullptr;
    if (node->get_value()->get_type()==expr_type::ast_num) {
        value_node = (number_literal*)node->get_value();
    } else if (node->get_value()->get_type()==expr_type::ast_binary &&
        ((binary_operator*)node->get_value())->get_optimized_number()) {
        value_node = ((binary_operator*)node->get_value())->get_optimized_number();
    } else if (node->get_value()->get_type()==expr_type::ast_unary &&
        ((unary_operator*)node->get_value())->get_optimized_number()) {
        value_node = ((unary_operator*)node->get_value())->get_optimized_number();
    }
    if (value_node) {
        const_number(node, value_node);
    }
    return true;
}

void optimizer::do_optimization(code_block* root) {
    root->accept(this);
}