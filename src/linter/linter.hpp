#pragma once

#include "ast/ast.hpp"
#include "ast/visitor.hpp"
#include "error/error.hpp"

namespace nasal {

class linter: public visitor {
private:
    error err;

public:
    bool visit_binary_operator(binary_operator*) override;
    bool visit_definition_expr(definition_expr*) override;
    bool visit_assignment_expr(assignment_expr*) override;
};

}
