#include "nasal_ast.h"
#include "ast_visitor.h"

void expr::accept(ast_visitor* visitor) {
    visitor->visit_expr(this);
}

void call::accept(ast_visitor* visitor) {
    visitor->visit_call(this);
}

void file_info::accept(ast_visitor* visitor) {
    visitor->visit_file_info(this);
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
    if (value) {
        delete value;
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
    visitor->visit_function(this);
}

code_block::~code_block() {
    for(auto i : expressions) {
        delete i;
    }
}

void code_block::accept(ast_visitor* visitor) {
    visitor->visit_code_block(this);
}

parameter::~parameter() {
    if (default_value) {
        delete default_value;
    }
}

void parameter::accept(ast_visitor* visitor) {
    visitor->visit_parameter(this);
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
    visitor->visit_ternary_operator(this);
}

binary_operator::~binary_operator() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
    if (optimized_const_number) {
        delete optimized_const_number;
    }
    if (optimized_const_string) {
        delete optimized_const_string;
    }
}

void binary_operator::accept(ast_visitor* visitor) {
    visitor->visit_binary_operator(this);
}

unary_operator::~unary_operator() {
    if (value) {
        delete value;
    }
    if (optimized_number) {
        delete optimized_number;
    }
}

void unary_operator::accept(ast_visitor* visitor) {
    visitor->visit_unary_operator(this);
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
    visitor->visit_call_expr(this);
}

void call_hash::accept(ast_visitor* visitor) {
    visitor->visit_call_hash(this);
}

call_vector::~call_vector() {
    for(auto i : calls) {
        delete i;
    }
}

void call_vector::accept(ast_visitor* visitor) {
    visitor->visit_call_vector(this);
}

call_function::~call_function() {
    for(auto i : args) {
        delete i;
    }
}

void call_function::accept(ast_visitor* visitor) {
    visitor->visit_call_function(this);
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
    visitor->visit_slice_vector(this);
}

definition_expr::~definition_expr() {
    if (variable_name) {
        delete variable_name;
    }
    if (variables) {
        delete variables;
    }
    if (tuple) {
        delete tuple;
    }
    if (value) {
        delete value;
    }
}

void definition_expr::accept(ast_visitor* visitor) {
    visitor->visit_definition_expr(this);
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
    visitor->visit_assignment_expr(this);
}

multi_identifier::~multi_identifier() {
    for(auto i : variables) {
        delete i;
    }
}

void multi_identifier::accept(ast_visitor* visitor) {
    visitor->visit_multi_identifier(this);
}

tuple_expr::~tuple_expr() {
    for(auto i : elements) {
        delete i;
    }
}

void tuple_expr::accept(ast_visitor* visitor) {
    visitor->visit_tuple_expr(this);
}

multi_assign::~multi_assign() {
    if (tuple) {
        delete tuple;
    }
    if (value) {
        delete value;
    }
}

void multi_assign::accept(ast_visitor* visitor) {
    visitor->visit_multi_assign(this);
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
    visitor->visit_while_expr(this);
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
    visitor->visit_for_expr(this);
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
    visitor->visit_iter_expr(this);
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
    visitor->visit_forei_expr(this);
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
    visitor->visit_condition_expr(this);
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
    visitor->visit_if_expr(this);
}

void continue_expr::accept(ast_visitor* visitor) {
    visitor->visit_continue_expr(this);
}

void break_expr::accept(ast_visitor* visitor) {
    visitor->visit_break_expr(this);
}

return_expr::~return_expr() {
    if (value) {
        delete value;
    }
}

void return_expr::accept(ast_visitor* visitor) {
    visitor->visit_return_expr(this);
}