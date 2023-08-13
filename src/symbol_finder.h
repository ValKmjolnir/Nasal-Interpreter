#pragma once

#include "nasal_ast.h"
#include "ast_visitor.h"

#include <cstring>
#include <sstream>
#include <vector>

class symbol_finder:public ast_visitor {
public:
    struct symbol_info {
        std::string name;
        span location;
    };

private:
    std::vector<symbol_info> symbols;

public:
    bool visit_definition_expr(definition_expr*) override;
    bool visit_function(function*) override;
    bool visit_iter_expr(iter_expr*) override;
    const std::vector<symbol_finder::symbol_info>& do_find(code_block*);
};