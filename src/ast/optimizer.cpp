#include "ast/optimizer.hpp"

namespace nasal {

void optimizer::const_string(binary_operator* node,
                             string_literal* lhs,
                             string_literal* rhs) {
    if (node->get_operator_type() != binary_operator::kind::concat) {
        return;
    }
    const auto& left = lhs->get_content();
    const auto& right = rhs->get_content();
    node->set_optimized_string(
        new string_literal(node->get_location(), left + right)
    );
}

void optimizer::const_number(binary_operator* node,
                             number_literal* lhs,
                             number_literal* rhs) {
    const auto left = lhs->get_number();
    const auto right = rhs->get_number();
    f64 res;
    switch (node->get_operator_type()) {
        case binary_operator::kind::add: res = left + right; break;
        case binary_operator::kind::sub: res = left - right; break;
        case binary_operator::kind::mult: res = left * right; break;
        case binary_operator::kind::div: res = left / right; break;
        case binary_operator::kind::less: res = left < right; break;
        case binary_operator::kind::leq: res = left <= right; break;
        case binary_operator::kind::grt: res = left > right; break;
        case binary_operator::kind::geq: res = left >= right; break;
        case binary_operator::kind::bitwise_or:
            res = static_cast<i32>(left) | static_cast<i32>(right); break;
        case binary_operator::kind::bitwise_xor:
            res = static_cast<i32>(left) ^ static_cast<i32>(right); break;
        case binary_operator::kind::bitwise_and:
            res = static_cast<i32>(left) & static_cast<i32>(right); break;
        default: return;
    }
    if (std::isinf(res) || std::isnan(res)) {
        return;
    }
    node->set_optimized_number(
        new number_literal(node->get_location(), res, "")
    );
}

void optimizer::const_number(unary_operator* node,
                             number_literal* value_node) {
    auto res = value_node->get_number();
    switch (node->get_operator_type()) {
        case unary_operator::kind::negative:
            res = -res; break;
        case unary_operator::kind::bitwise_not:
            res = ~static_cast<i32>(res); break;
        case unary_operator::kind::logical_not:
            res = !res; break;
    }
    if (std::isinf(res) || std::isnan(res)) {
        return;
    }
    node->set_optimized_number(
        new number_literal(node->get_location(), res, "")
    );
}

bool optimizer::visit_binary_operator(binary_operator* node) {
    auto lhs = node->get_left();
    auto rhs = node->get_right();
    lhs->accept(this);
    rhs->accept(this);

    number_literal* left_num_node = nullptr;
    number_literal* right_num_node = nullptr;
    string_literal* left_str_node = nullptr;
    string_literal* right_str_node = nullptr;
    if (lhs->get_type() == expr_type::ast_num) {
        left_num_node = reinterpret_cast<number_literal*>(lhs);
    } else if (lhs->get_type() == expr_type::ast_binary &&
               reinterpret_cast<binary_operator*>(lhs)->get_optimized_number()) {
        auto optimized = reinterpret_cast<binary_operator*>(lhs);
        left_num_node = optimized->get_optimized_number();
    } else if (lhs->get_type() == expr_type::ast_unary &&
               reinterpret_cast<unary_operator*>(lhs)->get_optimized_number()) {
        auto optimized = reinterpret_cast<unary_operator*>(lhs);
        left_num_node = optimized->get_optimized_number();
    }
    if (rhs->get_type() == expr_type::ast_num) {
        right_num_node = reinterpret_cast<number_literal*>(rhs);
    } else if (rhs->get_type() == expr_type::ast_binary &&
               reinterpret_cast<binary_operator*>(rhs)->get_optimized_number()) {
        auto optimized = reinterpret_cast<binary_operator*>(rhs);
        right_num_node = optimized->get_optimized_number();
    } else if (rhs->get_type() == expr_type::ast_unary &&
               reinterpret_cast<unary_operator*>(rhs)->get_optimized_number()) {
        auto optimized = reinterpret_cast<unary_operator*>(rhs);
        right_num_node = optimized->get_optimized_number();
    }

    if (lhs->get_type() == expr_type::ast_str) {
        left_str_node = reinterpret_cast<string_literal*>(lhs);
    } else if (lhs->get_type() == expr_type::ast_binary &&
               reinterpret_cast<binary_operator*>(lhs)->get_optimized_string()) {
        auto optimized = reinterpret_cast<binary_operator*>(lhs);
        left_str_node = optimized->get_optimized_string();
    }
    if (rhs->get_type() == expr_type::ast_str) {
        right_str_node = reinterpret_cast<string_literal*>(rhs);
    } else if (rhs->get_type() == expr_type::ast_binary &&
               reinterpret_cast<binary_operator*>(rhs)->get_optimized_string()) {
        auto optimized = reinterpret_cast<binary_operator*>(rhs);
        right_str_node = optimized->get_optimized_string();
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
    auto value = node->get_value();
    value->accept(this);

    number_literal* num_node = nullptr;
    if (value->get_type() == expr_type::ast_num) {
        num_node = reinterpret_cast<number_literal*>(value);
    } else if (value->get_type() == expr_type::ast_binary &&
               reinterpret_cast<binary_operator*>(value)->get_optimized_number()) {
        auto optimized = reinterpret_cast<binary_operator*>(value);
        num_node = optimized->get_optimized_number();
    } else if (value->get_type() == expr_type::ast_unary &&
               reinterpret_cast<unary_operator*>(value)->get_optimized_number()) {
        auto optimized = reinterpret_cast<unary_operator*>(value);
        num_node = optimized->get_optimized_number();
    }
    if (num_node) {
        const_number(node, num_node);
    }
    return true;
}

void optimizer::do_optimization(code_block* root) {
    root->accept(this);
}

}
