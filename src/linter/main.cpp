#include <string>

#include "lexer/lexer.hpp"
#include "parse/parse.hpp"
#include "error/error.hpp"

#include "linter/checker/manager.hpp"
#include "linter/checker/space_infix_ops.hpp"
#include "cli/cli.hpp"

void check_single_file(nasal::linter::checker_manager& cm,
                       const std::string& file) {
    nasal::lexer lex;
    nasal::parse parse;

    if (lex.scan(file).geterr()) {
        return;
    }
    if (parse.compile(lex.result()).geterr()) {
        return;
    }

    cm.check(parse.tree());
}

int main(i32 argc, const char* argv[]) {
    if (argc == 1) {
        nasal::cli::version(std::clog, "nasal-linter");
        return 0;
    }

    if (argc != 2) {
        std::cerr << nasal::cli::nasal_linter_help;
        return -1;
    }

    const auto config = nasal::cli::parse({argv + 1, argv + argc});
    if (config.has(nasal::cli::option::cli_help)) {
        std::cout << nasal::cli::nasal_linter_help;
        return 0;
    } else if (config.has(nasal::cli::option::cli_version)) {
        nasal::cli::version(std::cout, "nasal-linter");
        return 0;
    }

    if (config.input_file_path.empty()) {
        std::cerr << "nasal-linter: no input file\n";
        std::cerr << nasal::cli::nasal_linter_help;
        return -1;
    }

    nasal::error err;
    nasal::linter::checker_manager cm;
    cm.regist_checker<nasal::linter::space_infix_ops>(err, "space-infix-ops");

    check_single_file(cm, config.input_file_path);
    if (err.geterr()) {
        err.err("nasal-linter", std::to_string(err.geterr()) + " error(s) found");
        return -1;
    }
    return 0;
}
