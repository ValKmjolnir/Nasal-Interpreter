#include "tools/linter/checker/manager.hpp"

namespace nasal::linter {

void checker::report(expr* node, const std::string& msg) {
    err.err(checker_name, node->get_location(), msg);
}

void checker_manager::check(expr* node) {
    for (auto& checker : checkers) {
        node->accept(checker.get());
    }
}

}