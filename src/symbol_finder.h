#pragma once

#include "nasal_ast.h"
#include "ast_visitor.h"

#include <cstring>
#include <sstream>
#include <vector>

class symbol_finder:public ast_visitor {
private:
    std::vector<std::string> symbols;

public:
    bool visit_definition_expr(definition_expr*) override;
    bool visit_function(function*) override;
    bool visit_iter_expr(iter_expr*) override;
    const std::vector<std::string>& do_find(code_block*);
};