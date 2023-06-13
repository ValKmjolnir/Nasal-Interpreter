#include "nasal_new_ast.h"
#include "ast_visitor.h"

bool expr::accept(ast_visitor* visitor) {
    visitor->visit_expr(this);
    return true;
}

bool null_expr::accept(ast_visitor* visitor) {
    visitor->visit_null_expr(this);
    return true;
}

bool nil_expr::accept(ast_visitor* visitor) {
    visitor->visit_nil_expr(this);
    return true;
}

bool number_literal::accept(ast_visitor* visitor) {
    visitor->visit_number_literal(this);
    return true;
}

bool string_literal::accept(ast_visitor* visitor) {
    visitor->visit_string_literal(this);
    return true;
}