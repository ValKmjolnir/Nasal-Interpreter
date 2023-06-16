#include "nasal_new_ast.h"
#include "ast_visitor.h"

void expr::accept(ast_visitor* visitor) {
    visitor->visit_expr(this);
}

void null_expr::accept(ast_visitor* visitor) {
    visitor->visit_null_expr(this);
}

void nil_expr::accept(ast_visitor* visitor) {
    visitor->visit_nil_expr(this);
}

void number_literal::accept(ast_visitor* visitor) {
    visitor->visit_number_literal(this);
}

void string_literal::accept(ast_visitor* visitor) {
    visitor->visit_string_literal(this);
}

void identifier::accept(ast_visitor* visitor) {
    visitor->visit_identifier(this);
}

void bool_literal::accept(ast_visitor* visitor) {
    visitor->visit_bool_literal(this);
}

vector_expr::~vector_expr() {
    for(auto i : elements) {
        delete i;
    }
}

void vector_expr::accept(ast_visitor* visitor) {
    visitor->visit_vector_expr(this);
}

hash_expr::~hash_expr() {
    for(auto i : members) {
        delete i;
    }
}

void hash_expr::accept(ast_visitor* visitor) {
    visitor->visit_hash_expr(this);
}

hash_pair::~hash_pair() {
    if (element) {
        delete element;
    }
}

void hash_pair::accept(ast_visitor* visitor) {
    visitor->visit_hash_pair(this);
}