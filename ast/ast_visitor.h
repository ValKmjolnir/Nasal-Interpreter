#pragma once

#include "nasal_new_ast.h"

class ast_visitor {
public:
    virtual void visit_expr(expr*);
    virtual void visit_null_expr(null_expr*);
    virtual void visit_nil_expr(nil_expr*);
    virtual void visit_number_literal(number_literal*);
    virtual void visit_string_literal(string_literal*);
};