#include "repl.h"
#include "nasal_lexer.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "optimizer.h"
#include "nasal_codegen.h"
#include "nasal_vm.h"

namespace nasal {
namespace repl {

void repl::add_command_history(const std::string& history) {
    if (command_history.size() && command_history.back()==history) {
        return;
    }
    command_history.push_back(history);
    if (command_history.size()>1000) {
        command_history.pop_front();
    }
}

std::string repl::readline(const std::string& prompt = ">>> ") {
    auto line = std::string("");
    std::cout << prompt;
    std::getline(std::cin, line, '\n');
    return line;
}

void repl::update_temp_file() {
    auto content = std::string("");
    for (const auto& i : source) {
        content += i + "\n";
    }
    info::instance()->repl_file_source = content + " ";
}

void repl::update_temp_file(const std::vector<std::string>& src) {
    auto content = std::string("");
    for (const auto& i : src) {
        content += i + "\n";
    }
    info::instance()->repl_file_source = content + " ";
}

bool repl::check_need_more_input() {
    while(true) {
        update_temp_file();
        auto nasal_lexer = std::make_unique<lexer>();
        if (nasal_lexer->scan("<nasal-repl>").geterr()) {
            return false;
        }

        i64 in_curve = 0;
        i64 in_bracket = 0;
        i64 in_brace = 0;
        for (const auto& t : nasal_lexer->result()) {
            switch(t.type) {
                case tok::tk_lcurve: ++in_curve; break;
                case tok::tk_rcurve: --in_curve; break;
                case tok::tk_lbracket: ++in_bracket; break;
                case tok::tk_rbracket: --in_bracket; break;
                case tok::tk_lbrace: ++in_brace; break;
                case tok::tk_rbrace: --in_brace; break;
                default: break;
            }
        }
        if (in_curve<=0 && in_bracket<=0 && in_brace<=0) {
            break;
        }

        auto line = readline("... ");
        add_command_history(line);
        source.back() += "\n" + line;
    }
    return true;
}

int repl::check_need_more_input(std::vector<std::string>& src) {
    update_temp_file(src);
    auto nasal_lexer = std::make_unique<lexer>();
    if (nasal_lexer->scan("<nasal-repl>").geterr()) {
        return -1;
    }

    i64 in_curve = 0;
    i64 in_bracket = 0;
    i64 in_brace = 0;
    for (const auto& t : nasal_lexer->result()) {
        switch(t.type) {
            case tok::tk_lcurve: ++in_curve; break;
            case tok::tk_rcurve: --in_curve; break;
            case tok::tk_lbracket: ++in_bracket; break;
            case tok::tk_rbracket: --in_bracket; break;
            case tok::tk_lbrace: ++in_brace; break;
            case tok::tk_rbrace: --in_brace; break;
            default: break;
        }
    }
    if (in_curve > 0 || in_bracket > 0 || in_brace > 0) {
        return 1;  // More input needed
    }
    return 0;  // Input is complete
}


void repl::help() {
    std::cout << ".h, .help   | show help\n";
    std::cout << ".e, .exit   | quit the REPL\n";
    std::cout << ".q, .quit   | quit the REPL\n";
    std::cout << ".c, .clear  | clear the screen\n";
    std::cout << ".s, .source | show source code\n";
    std::cout << "\n";
}

bool repl::run() {
    auto nasal_lexer = std::make_unique<lexer>();
    auto nasal_parser = std::make_unique<parse>();
    auto nasal_linker = std::make_unique<linker>();
    auto nasal_opt = std::make_unique<optimizer>();
    auto nasal_codegen = std::make_unique<codegen>();

    update_temp_file();
    if (nasal_lexer->scan("<nasal-repl>").geterr()) {
        return false;
    }

    if (nasal_parser->compile(*nasal_lexer).geterr()) {
        return false;
    }

    if (nasal_linker->link(*nasal_parser, true).geterr()) {
        return false;
    }

    nasal_opt->do_optimization(nasal_parser->tree());
    if (nasal_codegen->compile(*nasal_parser, *nasal_linker, true, false).geterr()) {
        return false;
    }

    runtime.run(*nasal_codegen, *nasal_linker, {});
    return true;
}

void repl::execute() {
    source = {};
    // mark we are in repl mode
    info::instance()->in_repl_mode = true;
    std::cout << "[nasal-repl] Initializating enviroment...\n";
    // run on pass for initializing basic modules, without output
    if (!run()) {
        std::cout << "[nasal-repl] Initialization failed.\n\n";
        std::exit(-1);
    }
    // allow output now
    runtime.set_allow_repl_output_flag(true);
    std::cout << "[nasal-repl] Initialization complete.\n\n";

    // finish initialization, output version info
    std::cout << "Nasal REPL interpreter version " << __nasver__;
    std::cout << " (" << __DATE__ << " " << __TIME__ << ")\n";
    help();

    while(true) {
        auto line = readline();
        if (!line.length()) {
            continue;
        }
        add_command_history(line);

        if (line == ".e" || line == ".exit") {
            break;
        } else if (line == ".q" || line == ".quit") {
            break;
        } else if (line == ".h" || line == ".help") {
            help();
            continue;
        } else if (line == ".c" || line == ".clear") {
            std::cout << "\033c";
            continue;
        } else if (line == ".s" || line == ".source") {
            update_temp_file();
            std::cout << info::instance()->repl_file_source << "\n";
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

        // run program
        if (!run()) {
            source.pop_back();
        }

        std::cout << "\n";
    }
}

}
}
