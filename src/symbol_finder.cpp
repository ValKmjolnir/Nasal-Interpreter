#include "symbol_finder.h"

bool symbol_finder::visit_definition_expr(definition_expr* node) {
    if (node->get_variable_name()) {
        symbols.push_back(node->get_variable_name()->get_name());
    } else {
        for(auto i : node->get_variables()->get_variables()) {
            symbols.push_back(i->get_name());
        }
    }
    if (node->get_tuple()) {
        node->get_tuple()->accept(this);
    } else {
        node->get_value()->accept(this);
    }
    return true;
}

bool symbol_finder::visit_function(function* node) {
    return true;
}

bool symbol_finder::visit_iter_expr(iter_expr* node) {
    if (node->get_name()) {
        symbols.push_back(node->get_name()->get_name());
    }
    return true;
}

const std::vector<std::string>& symbol_finder::do_find(code_block* root) {
    symbols.clear();
    root->accept(this);
    return symbols;
}