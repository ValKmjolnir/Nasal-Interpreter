#include <iostream>

#include "nasal.hpp"
#include "linter/checker/space_infix_ops.hpp"


namespace nasal::linter {

bool space_infix_ops::visit_binary_operator(binary_operator* node) {
    auto lhs = node->get_left();
    auto rhs = node->get_right();

    lhs->accept(this);
    rhs->accept(this);

    const auto& lhs_loc = lhs->get_location();
    const auto& rhs_loc = rhs->get_location();

    if (lhs_loc.end_line == rhs_loc.begin_line &&
        rhs_loc.begin_column - lhs_loc.end_column <= 2) {
        report(node, "no space between expressions and operator");
    }
    return true;
}

bool space_infix_ops::visit_definition_expr(definition_expr* node) {
    auto lhs = node->get_variable_name()
            ? reinterpret_cast<expr*>(node->get_variable_name())
            : reinterpret_cast<expr*>(node->get_variables());
    auto rhs = node->get_tuple()
            ? reinterpret_cast<expr*>(node->get_tuple())
            : reinterpret_cast<expr*>(node->get_value());
    
    lhs->accept(this);
    rhs->accept(this);

    const auto& lhs_loc = lhs->get_location();
    const auto& rhs_loc = rhs->get_location();

    if (lhs_loc.end_line == rhs_loc.begin_line &&
        rhs_loc.begin_column - lhs_loc.end_column <= 2) {
        report(node, "no space between expressions and `=`");
    }
    return true;
}

bool space_infix_ops::visit_assignment_expr(assignment_expr* node) {
    auto lhs = node->get_left();
    auto rhs = node->get_right();

    lhs->accept(this);
    rhs->accept(this);

    const auto& lhs_loc = lhs->get_location();
    const auto& rhs_loc = rhs->get_location();

    if (node->get_assignment_type() == assignment_expr::kind::equal) {
        if (lhs_loc.end_line == rhs_loc.begin_line &&
            rhs_loc.begin_column - lhs_loc.end_column <= 2) {
            report(node, "no space between expressions and `=`");
        }
    } else {
        if (lhs_loc.end_line == rhs_loc.begin_line &&
            rhs_loc.begin_column - lhs_loc.end_column <= 3) {
            report(node, "no space between expressions and operator");
        }
    }
    return true;
}

bool space_infix_ops::visit_parameter(parameter* node) {
    if (node->get_default_value()) {
        node->get_default_value()->accept(this);
    }

    if (!node->get_default_value()) {
        return true;
    }

    auto name_len = node->get_parameter_name().length();
    auto node_loc = node->get_location();
    auto dv_loc = node->get_default_value()->get_location();
    if (node_loc.begin_line == dv_loc.begin_line &&
        dv_loc.begin_column - (node_loc.begin_column + name_len) <= 2) {
        report(node, "expect space between parameter name, '=', and default value");
    }

    return true;
}

}
