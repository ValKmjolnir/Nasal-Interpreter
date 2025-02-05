#include "nasal.h"
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_parse.h"
#include "util/util.h"
#include "cli/cli.h"
#include "ast_format.h"

#include <iostream>
#include <thread>

[[noreturn]]
void err() {
    std::cerr << "invalid argument(s), use <nasal-format -h> to get help.\n";
    std::exit(1);
}

void execute(const nasal::cli::cli_config& config) {
    nasal::lexer lex;
    nasal::parse parse;

    // lexer scans file to get tokens
    lex.scan(config.input_file_path).chkerr();

    // parser gets lexer's token list to compile
    parse.compile(lex).chkerr();

    nasal::ast_format("nasal-format-out.nas").do_format(parse.tree());
}

int main(i32 argc, const char* argv[]) {
    // output version info
    if (argc <= 1) {
        err();
    } else if (argc > 2) {
        err();
    }

    // the first argument is the executable itself, ignore it
    const auto config = nasal::cli::parse({argv+1, argv+argc});

    // run directly or show help
    if (config.has(nasal::cli::option::cli_help)) {
        std::clog << nasal::cli::nasal_format_help;
    } else if (config.has(nasal::cli::option::cli_version)) {
        std::clog << nasal::cli::nasal_format_version;
    } else if (config.input_file_path.size()) {
        execute(config);
    } else {
        err();
    }
    return 0;
}