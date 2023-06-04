#include "nasal_new_ast.h"

bool stmt::accept(ast_visitor* visitor) {
    visitor->visit_stmt(this);
    return true;
}

bool expr::accept(ast_visitor* visitor) {
    visitor->visit_expr(this);
    return true;
}

void ast_visitor::visit_stmt(stmt* node) {
    node->accept(this);
}

void ast_visitor::visit_expr(expr* node) {
    node->accept(this);
}