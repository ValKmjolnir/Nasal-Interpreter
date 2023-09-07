#pragma once

#include <cmath>

#include "nasal_ast.h"
#include "ast_visitor.h"

namespace nasal {

class optimizer:public ast_visitor {
private:
    void const_string(binary_operator*, string_literal*, string_literal*);
    void const_number(binary_operator*, number_literal*, number_literal*);
    void const_number(unary_operator*, number_literal*);

public:
    bool visit_binary_operator(binary_operator*) override;
    bool visit_unary_operator(unary_operator*) override;

public:
    void do_optimization(code_block*);
};

}
