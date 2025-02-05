#include "nasal.h"
#include "nasal_type.h"
#include "nasal_gc.h"
#include "nasal_err.h"
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "ast_visitor.h"
#include "ast_dumper.h"
#include "symbol_finder.h"
#include "optimizer.h"
#include "nasal_codegen.h"
#include "nasal_vm.h"
#include "nasal_dbg.h"

#include "util/util.h"
#include "repl/repl.h"
#include "cli/cli.h"

#include <cstdlib>

[[noreturn]]
void err() {
    std::cerr << "invalid argument(s), use <nasal -h> to get help.\n";
    std::exit(1);
}

void execute(const nasal::cli::cli_config& config) {
    using option = nasal::cli::option;
    using clk = std::chrono::high_resolution_clock;
    const auto den = clk::duration::period::den;

    nasal::lexer   lex;
    nasal::parse   parse;
    nasal::linker  ld;
    nasal::codegen gen;

    // lexer scans file to get tokens
    lex.scan(config.input_file_path).chkerr();

    // parser gets lexer's token list to compile
    parse.compile(lex).chkerr();
    if (config.has(option::cli_view_raw_ast)) {
        nasal::ast_dumper().dump(parse.tree());
    }

    // linker gets parser's ast and load import files to this ast
    ld.link(parse, config.has(option::cli_detail_info)).chkerr();
    if (config.has(option::cli_show_referenced_file)) {
        if (ld.get_file_list().size()) {
            std::cout << "referenced file(s):\n";
        }
        for(const auto& file: ld.get_file_list()) {
            std::cout << "  " << file << "\n";
        }
    }
    
    // optimizer does simple optimization on ast
    auto opt = std::make_unique<nasal::optimizer>();
    opt->do_optimization(parse.tree());
    if (config.has(option::cli_view_ast)) {
        nasal::ast_dumper().dump(parse.tree());
    }

    // code generator gets parser's ast and import file list to generate code
    gen.compile(parse, ld, false, config.has(option::cli_limit_mode)).chkerr();
    if (config.has(option::cli_view_code)) {
        gen.print(std::cout);
    }
    if (config.has(option::cli_show_symbol)) {
        gen.symbol_dump(std::cout);
    }

    // run
    const auto start = clk::now();
    double gc_time_ms = 0.0;
    double gc_total_memory = 0.0;
    if (config.has(option::cli_debug_mode)) {
        auto debugger = std::make_unique<nasal::dbg>();
        debugger->run(
            gen,
            ld,
            config.nasal_vm_args,
            config.has(option::cli_profile),
            config.has(option::cli_profile_all)
        );
        gc_time_ms = debugger->get_gc_time_ms();
        gc_total_memory = debugger->get_total_memory();
    } else if (config.has(option::cli_show_execute_time) ||
               config.has(option::cli_detail_info) ||
               config.has(option::cli_limit_mode) ||
               config.has(option::cli_execute)) {
        auto runtime = std::make_unique<nasal::vm>();
        runtime->set_detail_report_info(config.has(option::cli_detail_info));
        runtime->set_limit_mode_flag(config.has(option::cli_limit_mode));
        runtime->run(gen, ld, config.nasal_vm_args);
        gc_time_ms = runtime->get_gc_time_ms();
        gc_total_memory = runtime->get_total_memory();
    }

    // get running time
    const auto end = clk::now();
    if (config.has(option::cli_show_execute_time)) {
        double execute_time_sec = static_cast<f64>((end - start).count())/den;
        double gc_time_sec = gc_time_ms / 1000.0;
        std::clog << "process exited after ";
        std::clog << execute_time_sec << "s, gc time: ";
        std::clog << gc_time_sec << "s (";
        std::clog << gc_time_sec / execute_time_sec * 100.0 << "%), ";
        std::clog << "memory usage: " << gc_total_memory << "MB\n\n";
    }
}

i32 main(i32 argc, const char* argv[]) {
    // output version info
    if (argc <= 1) {
        std::clog << nasal::cli::logo;
        return 0;
    }

    // the first argument is the executable itself, ignore it
    const auto config = nasal::cli::parse({argv+1, argv+argc});

    // run directly or show help
    if (argc == 2) {
        if (config.has(nasal::cli::option::cli_help)) {
            std::clog << nasal::cli::help;
        } else if (config.has(nasal::cli::option::cli_version)) {
            std::clog << nasal::cli::version;
        } else if (config.has(nasal::cli::option::cli_repl_mode)) {
            auto repl = std::make_unique<nasal::repl::repl>();
            repl->execute();
        } else if (config.input_file_path.size()) {
            execute(config);
        } else {
            err();
        }
        return 0;
    }

    // execute with arguments
    execute(config);
    return 0;
}