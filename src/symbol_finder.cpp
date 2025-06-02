#include "symbol_finder.h"

namespace nasal {

bool symbol_finder::visit_definition_expr(definition_expr* node) {
    if (node->get_variable_name()) {
        // single variable definition
        // example: var a = 1;
        symbols.push_back({
            node->get_variable_name()->get_name(),
            node->get_variable_name()
        });
    } else {
        // multiple variable definition
        // example: var (a, b, c) = (0, 1, 2);
        for (auto i : node->get_variables()->get_variables()) {
            symbols.push_back({i->get_name(), i});
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
    // do not scan variables defined in function inside this code block
    return true;
}

bool symbol_finder::visit_iter_expr(iter_expr* node) {
    if (node->is_definition() && node->get_name()) {
        symbols.push_back({
            node->get_name()->get_name(),
            node->get_name()
        });
    }
    return true;
}

const std::vector<symbol_finder::symbol_info>& symbol_finder::do_find(code_block* root) {
    symbols.clear();
    root->accept(this);
    return symbols;
}

}
