#include "ast_visitor.h"

bool ast_visitor::visit_expr(expr* node) {
    node->accept(this);
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