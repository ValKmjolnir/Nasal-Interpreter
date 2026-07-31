#include "nasal.hpp"
#include "error/error.hpp"
#include "lexer/lexer.hpp"
#include "parse/parse.hpp"
#include "cli/cli.hpp"
#include "tools/format/format.hpp"

#include <iostream>

[[noreturn]]
void err() {
    std::cerr << "invalid argument(s), use <nasal-format -h> to get help.\n";
    std::exit(1);
}

void execute(const nasal::cli::cli_config& config) {
    nasal::error err;
    nasal::lexer lex(err);
    nasal::parse parse(err);

    // lexer scans file to get tokens
    lex.scan(config.input_file_path).chkerr();

    // parser gets lexer's token list to compile
    parse.compile(lex.result()).chkerr();

    nasal::ast_format("nasal-format-out.nas").do_format(parse.tree());
}

int main(i32 argc, const char* argv[]) {
    // output version info
    if (argc <= 1) {
        nasal::cli::version(std::clog, "nasal-format(beta)");
        return 0;
    } else if (argc > 2) {
        err();
    }

    // the first argument is the executable itself, ignore it
    const auto config = nasal::cli::parse({argv + 1, argv + argc});

    // run directly or show help
    if (config.has(nasal::cli::option::cli_help)) {
        std::clog << nasal::cli::nasal_format_help;
    } else if (config.has(nasal::cli::option::cli_version)) {
        nasal::cli::version(std::clog, "nasal-format(beta)");
    } else if (config.input_file_path.size()) {
        execute(config);
    } else {
        err();
    }
    return 0;
}