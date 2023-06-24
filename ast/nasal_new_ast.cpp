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

function::~function() {
    for(auto i : parameter_list) {
        delete i;
    }
    if (block) {
        delete block;
    }
}

void function::accept(ast_visitor* visitor) {
    // TODO
}

code_block::~code_block() {
    for(auto i : expressions) {
        delete i;
    }
}

void code_block::accept(ast_visitor* visitor) {
    // TODO
}

parameter::~parameter() {
    if (name) {
        delete name;
    }
    if (default_value) {
        delete default_value;
    }
}

void parameter::accept(ast_visitor* visitor) {
    // TODO
}

ternary_operator::~ternary_operator() {
    if (condition) {
        delete condition;
    }
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

void ternary_operator::accept(ast_visitor* visitor) {
    // TODO
}

binary_operator::~binary_operator() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

void binary_operator::accept(ast_visitor* visitor) {
    // TODO
}

unary_operator::~unary_operator() {
    if (value) {
        delete value;
    }
}

void unary_operator::accept(ast_visitor* visitor) {
    // TODO
}

call_expr::~call_expr() {
    if(first) {
        delete first;
    }
    for(auto i : calls) {
        delete i;
    }
}

void call_expr::accept(ast_visitor* visitor) {
    // TODO
}

void call_hash::accept(ast_visitor* visitor) {
    // TODO
}

call_vector::~call_vector() {
    for(auto i : calls) {
        delete i;
    }
}

void call_vector::accept(ast_visitor* visitor) {
    // TODO
}

call_function::~call_function() {
    for(auto i : args) {
        delete i;
    }
}

void call_function::accept(ast_visitor* visitor) {
    // TODO
}

slice_vector::~slice_vector() {
    if (begin) {
        delete begin;
    }
    if (end) {
        delete end;
    }
}

void slice_vector::accept(ast_visitor* visitor) {
    // TODO
}

definition_expr::~definition_expr() {
    if (variable_name) {
        delete variable_name;
    }
    if (variables) {
        delete variables;
    }
    if (value) {
        delete value;
    }
}

void definition_expr::accept(ast_visitor* visitor) {
    // TODO
}

assignment_expr::~assignment_expr() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

void assignment_expr::accept(ast_visitor* visitor) {
    // TODO
}

multi_define::~multi_define() {
    for(auto i : variables) {
        delete i;
    }
    if (value) {
        delete value;
    }
}

void multi_define::accept(ast_visitor* visitor) {
    // TODO
}

tuple_expr::~tuple_expr() {
    for(auto i : elements) {
        delete i;
    }
}

void tuple_expr::accept(ast_visitor* visitor) {
    // TODO
}

multi_assign::~multi_assign() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

void multi_assign::accept(ast_visitor* visitor) {
    // TODO
}

while_expr::~while_expr() {
    if (condition) {
        delete condition;
    }
    if (block) {
        delete block;
    }
}

void while_expr::accept(ast_visitor* visitor) {
    // TODO
}

for_expr::~for_expr() {
    if (initializing) {
        delete initializing;
    }
    if (condition) {
        delete condition;
    }
    if (step) {
        delete step;
    }
    if (block) {
        delete block;
    }
}

void for_expr::accept(ast_visitor* visitor) {
    // TODO
}

iter_expr::~iter_expr() {
    if (name) {
        delete name;
    }
    if (call) {
        delete call;
    }
}

void iter_expr::accept(ast_visitor* visitor) {
    // TODO
}

forei_expr::~forei_expr() {
    if (iterator) {
        delete iterator;
    }
    if (vector_node) {
        delete vector_node;
    }
    if (block) {
        delete block;
    }
}

void forei_expr::accept(ast_visitor* visitor) {
    // TODO
}

condition_expr::~condition_expr() {
    if (if_stmt) {
        delete if_stmt;
    }
    for(auto i : elsif_stmt) {
        delete i;
    }
    if (else_stmt) {
        delete else_stmt;
    }
}

void condition_expr::accept(ast_visitor* visitor) {
    // TODO
}

if_expr::~if_expr() {
    if (condition) {
        delete condition;
    }
    if (block) {
        delete block;
    }
}

void if_expr::accept(ast_visitor* visitor) {
    // TODO
}

void continue_expr::accept(ast_visitor* visitor) {
    // TODO
}

void break_expr::accept(ast_visitor* visitor) {
    // TODO
}

return_expr::~return_expr() {
    if (value) {
        delete value;
    }
}

void return_expr::accept(ast_visitor* visitor) {
    // TODO
}