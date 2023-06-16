#pragma once

#include "nasal_new_ast.h"

class ast_visitor {
public:
    virtual bool visit_expr(expr*);
    virtual bool visit_null_expr(null_expr*);
    virtual bool visit_nil_expr(nil_expr*);
    virtual bool visit_number_literal(number_literal*);
    virtual bool visit_string_literal(string_literal*);
    virtual bool visit_identifier(identifier*);
    virtual bool visit_bool_literal(bool_literal*);
};