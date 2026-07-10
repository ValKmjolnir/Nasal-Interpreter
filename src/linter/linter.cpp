#include <iostream>

#include "nasal.hpp"
#include "linter/linter.hpp"
#include "lexer/lexer.hpp"
#include "parse/parse.hpp"

namespace nasal {

bool linter::visit_binary_operator(binary_operator* node) {
    auto lhs = node->get_left();
    auto rhs = node->get_right();

    lhs->accept(this);
    rhs->accept(this);

    const auto& lhs_loc = lhs->get_location();
    const auto& rhs_loc = rhs->get_location();

    if (lhs_loc.end_line == rhs_loc.begin_line &&
        rhs_loc.begin_column - lhs_loc.end_column <= 2) {
        err.err("space-check",
                node->get_location(),
                "no space between expressions and operator");
    }
    return true;
}

bool linter::visit_definition_expr(definition_expr* node) {
    auto lhs = node->get_variable_name()
            ? reinterpret_cast<expr*>(node->get_variable_name())
            : reinterpret_cast<expr*>(node->get_variables());
    auto rhs = node->get_tuple()
            ? reinterpret_cast<expr*>(node->get_tuple())
            : reinterpret_cast<expr*>(node->get_value());
    
    lhs->accept(this);
    rhs->accept(this);

    const auto& lhs_loc = lhs->get_location();
    const auto& rhs_loc = rhs->get_location();

    if (lhs_loc.end_line == rhs_loc.begin_line &&
        rhs_loc.begin_column - lhs_loc.end_column <= 2) {
        err.err("space-check",
                node->get_location(),
                "no space between expressions and `=`");
    }
    return true;
}

bool linter::visit_assignment_expr(assignment_expr* node) {
    auto lhs = node->get_left();
    auto rhs = node->get_right();

    lhs->accept(this);
    rhs->accept(this);

    const auto& lhs_loc = lhs->get_location();
    const auto& rhs_loc = rhs->get_location();

    if (node->get_assignment_type() == assignment_expr::kind::equal) {
        if (lhs_loc.end_line == rhs_loc.begin_line &&
            rhs_loc.begin_column - lhs_loc.end_column <= 2) {
            err.err("space-check",
                    node->get_location(),
                    "no space between expressions and `=`");
        }
    } else {
        if (lhs_loc.end_line == rhs_loc.begin_line &&
            rhs_loc.begin_column - lhs_loc.end_column <= 3) {
            err.err("space-check",
                    node->get_location(),
                    "no space between expressions and operator");
        }
    }
    return true;
}

}

int main(i32 argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: nasal-linter <file>" << std::endl;
        return -1;
    }

    nasal::lexer lex;
    nasal::parse parse;

    lex.scan(argv[1]).chkerr();
    parse.compile(lex).chkerr();

    nasal::linter lint;
    parse.tree()->accept(&lint);

    return 0;
}
