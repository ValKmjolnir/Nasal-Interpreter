#include "ast_visitor.h"

void ast_visitor::visit_expr(expr* node) {
    node->accept(this);
}

void ast_visitor::visit_null_expr(null_expr* node) {
    node->accept(this);
}

void ast_visitor::visit_nil_expr(nil_expr* node) {
    node->accept(this);
}

void ast_visitor::visit_number_literal(number_literal* node) {
    node->accept(this);
}

void ast_visitor::visit_string_literal(string_literal* node) {
    node->accept(this);
}