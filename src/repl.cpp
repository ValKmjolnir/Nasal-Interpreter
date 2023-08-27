#include "repl.h"
#include "nasal_lexer.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "optimizer.h"
#include "nasal_codegen.h"
#include "nasal_vm.h"

namespace repl {

std::string repl::readline(std::string prompt = ">>> ") {
    auto line = std::string("");
    std::cout << prompt;
    std::getline(std::cin, line,'\n');
    return line;
}

void repl::update_temp_file() {
    auto content = std::string("");
    for(const auto& i : source) {
        content += i + "\n";
    }

    std::ofstream out(".temp.nas", std::ios::binary);
    out << content;
}

bool repl::check_need_more_input() {
    while(true) {
        update_temp_file();
        auto nasal_lexer = std::unique_ptr<lexer>(new lexer);
        if (nasal_lexer->scan(".temp.nas").geterr()) {
            return false;
        }

        i64 in_curve = 0;
        i64 in_bracket = 0;
        i64 in_brace = 0;
        for(const auto& t : nasal_lexer->result()) {
            switch(t.type) {
                case tok::lcurve: ++in_curve; break;
                case tok::rcurve: --in_curve; break;
                case tok::lbracket: ++in_bracket; break;
                case tok::rbracket: --in_bracket; break;
                case tok::lbrace: ++in_brace; break;
                case tok::rbrace: --in_brace; break;
                default: break;
            }
        }
        if (!in_curve && !in_bracket && !in_brace) {
            break;
        }
        auto line = readline("... ");
        source.back() += "\n" + line;
    }
    return true;
}

void repl::help() {
    std::cout << ".h, .help  | show help\n";
    std::cout << ".e, .exit  | quit the REPL\n";
    std::cout << ".q, .quit  | quit the REPL\n";
    std::cout << ".c, .clear | clear the screen\n";
    std::cout << "\n";
}

bool repl::run() {
    update_temp_file();

    auto nasal_lexer = std::unique_ptr<lexer>(new lexer);
    auto nasal_parser = std::unique_ptr<parse>(new parse);
    auto nasal_linker = std::unique_ptr<linker>(new linker);
    auto nasal_opt = std::unique_ptr<optimizer>(new optimizer);
    auto nasal_codegen = std::unique_ptr<codegen>(new codegen);
    auto nasal_runtime = std::unique_ptr<vm>(new vm);

    if (nasal_lexer->scan(".temp.nas").geterr()) {
        return false;
    }

    if (nasal_parser->compile(*nasal_lexer).geterr()) {
        return false;
    }

    if (nasal_linker->link(*nasal_parser, ".temp.nas", true).geterr()) {
        return false;
    }

    nasal_opt->do_optimization(nasal_parser->tree());
    if (nasal_codegen->compile(*nasal_parser, *nasal_linker).geterr()) {
        return false;
    }

    nasal_runtime->run(*nasal_codegen, *nasal_linker, {}, false);

    return true;
}

void repl::execute() {
    source = {};
    std::cout << "Nasal REPL interpreter(experimental).\n";
    help();

    while(true) {
        auto line = readline();
        if (!line.length()) {
            continue;
        }

        if (line == ".e" || line == ".exit" || line == ".q" || line == ".quit") {
            break;
        } else if (line == ".h" || line == ".help") {
            help();
            continue;
        } else if (line == ".c" || line == ".clear") {
            std::cout << "\033c";
            continue;
        } else if (line[0] == "."[0]) {
            std::cout << "no such command \"" << line;
            std::cout << "\", input \".help\" for help\n";
            continue;
        }

        source.push_back(line);
        if (!check_need_more_input()) {
            source.pop_back();
            continue;
        }

        if (!run()) {
            source.pop_back();
        }
        std::cout << "\n";
    }
}

}