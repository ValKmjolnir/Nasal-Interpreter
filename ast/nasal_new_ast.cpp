#include "nasal_new_ast.h"

bool expr::accept(ast_visitor* visitor) {
    visitor->visit_expr(this);
    return true;
}

void ast_visitor::visit_expr(expr* node) {
    node->accept(this);
}