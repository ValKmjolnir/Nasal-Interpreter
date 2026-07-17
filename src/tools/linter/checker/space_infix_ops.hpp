#pragma once

#include "ast/ast.hpp"
#include "tools/linter/checker/manager.hpp"

namespace nasal::linter {

class space_infix_ops : public checker {
public:
    space_infix_ops(error& e, const char* n): checker(e, n) {}

public:
    bool visit_binary_operator(binary_operator*) override;
    bool visit_definition_expr(definition_expr*) override;
    bool visit_assignment_expr(assignment_expr*) override;
    bool visit_parameter(parameter*) override;
};

}
