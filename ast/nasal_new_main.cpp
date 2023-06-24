#include "nasal_new_header.h"
#include "nasal_new_err.h"
#include "nasal_new_lexer.h"
#include "nasal_new_ast.h"
#include "nasal_new_parse.h"
#include "ast_visitor.h"
#include "ast_dumper.h"

#include <unordered_map>
#include <thread>

const u32 VM_AST   =0x01;
const u32 VM_CODE  =0x02;
const u32 VM_TIME  =0x04;
const u32 VM_EXEC  =0x08;
const u32 VM_DETAIL=0x10;
const u32 VM_DEBUG =0x20;

std::ostream& help(std::ostream& out) {
    out
    <<"     ,--#-,\n"
    <<"<3  / \\____\\  <3\n"
    <<"    |_|__A_|\n"
#ifdef _WIN32
    <<"use command <chcp 65001> to use unicode.\n"
#endif
    <<"\nnasal <option>\n"
    <<"option:\n"
    <<"   -h,   --help   | get help.\n"
    <<"\nnasal [option] <file> [argv]\n"
    <<"option:\n"
    <<"   -a,   --ast    | view abstract syntax tree.\n"
    <<"   -c,   --code   | view bytecode.\n"
    <<"   -e,   --exec   | execute.\n"
    <<"   -t,   --time   | show execute time.\n"
    <<"   -d,   --detail | get detail info.\n"
    <<"   -dbg, --debug  | debug mode.\n"
    <<"file:\n"
    <<"   <filename>     | execute file.\n"
    <<"argv:\n"
    <<"   <args>         | cmd arguments used in program.\n";
    return out;
}

std::ostream& logo(std::ostream& out) {
    out
    <<"       __                _\n"
    <<"    /\\ \\ \\__ _ ___  __ _| |\n"
    <<"   /  \\/ / _` / __|/ _` | |\n"
    <<"  / /\\  / (_| \\__ \\ (_| | |\n"
    <<"  \\_\\ \\/ \\__,_|___/\\__,_|_|\n"
    <<"ver  : "<<__nasver<<" ("<<__DATE__<<" "<<__TIME__<<")\n"
    <<"std  : c++ "<<__cplusplus<<"\n"
    <<"core : "<<std::thread::hardware_concurrency()<<" cores\n"
    <<"repo : https://github.com/ValKmjolnir/Nasal-Interpreter\n"
    <<"repo : https://gitee.com/valkmjolnir/Nasal-Interpreter\n"
    <<"wiki : https://wiki.flightgear.org/Nasal_scripting_language\n"
    <<"input <nasal -h> to get help .\n";
    return out;
}

[[noreturn]]
void err() {
    std::cerr
    <<"invalid argument(s).\n"
    <<"use <nasal -h> to get help.\n";
    std::exit(1);
}

void execute(
    const std::string& file,
    const std::vector<std::string>& argv,
    const u32 cmd
) {
    using clk=std::chrono::high_resolution_clock;
    const auto den=clk::duration::period::den;

    error   err;
    lexer   lex(err);
    parse   parse(err);
    // linker  ld(err);
    // codegen gen(err);
    // vm      ctx;

    // lexer scans file to get tokens
    lex.scan(file).chkerr();

    // parser gets lexer's token list to compile
    parse.compile(lex).chkerr();
    if (cmd&VM_AST) {
        auto dumper = new ast_dumper();
        dumper->visit_expr(parse.tree());
    }

    // linker gets parser's ast and load import files to this ast
    // ld.link(parse, file, cmd&VM_DETAIL).chkerr();

    // optimizer does simple optimization on ast
    // optimize(parse.tree());

    // code generator gets parser's ast and import file list to generate code
    // gen.compile(parse, ld).chkerr();
    // if (cmd&VM_CODE) {
    //     gen.print();
    // }

    // run
    auto start=clk::now();
    // if (cmd&VM_DEBUG) {
    //     dbg(err).run(gen, ld, argv);
    // } else if (cmd&VM_TIME || cmd&VM_EXEC) {
    //     ctx.run(gen, ld, argv, cmd&VM_DETAIL);
    // }

    // get running time
    if (cmd&VM_TIME) {
        f64 tm=(clk::now()-start).count()*1.0/den;
        std::clog<<"process exited after "<<tm<<"s.\n\n";
    }
}

i32 main(i32 argc, const char* argv[]) {
    // output version info
    if (argc<=1) {
        std::clog<<logo;
        return 0;
    }

    // run directly or show help
    if (argc==2) {
        std::string s(argv[1]);
        if (s=="-h" || s=="--help") {
            std::clog<<help;
        } else if (s[0]!='-') {
            execute(s, {}, VM_EXEC);
        } else {
            err();
        }
        return 0;
    }

    // execute with arguments
    const std::unordered_map<std::string,u32> cmdlst = {
        {"--ast", VM_AST},
        {"-a", VM_AST},
        {"--code", VM_CODE},
        {"-c", VM_CODE},
        {"--exec", VM_EXEC},
        {"-e", VM_EXEC},
        {"--time", VM_TIME|VM_EXEC},
        {"-t", VM_TIME|VM_EXEC},
        {"--detail", VM_DETAIL|VM_EXEC},
        {"-d", VM_DETAIL|VM_EXEC},
        {"--debug", VM_DEBUG},
        {"-dbg", VM_DEBUG}
    };
    u32 cmd=0;
    std::string filename="";
    std::vector<std::string> vm_argv;
    for(i32 i=1; i<argc; ++i) {
        if (cmdlst.count(argv[i])) {
            cmd|=cmdlst.at(argv[i]);
        } else if (!filename.length()) {
            filename=argv[i];
        } else {
            vm_argv.push_back(argv[i]);
        }
    }
    if (!filename.length()) {
        err();
    }
    execute(filename, vm_argv, cmd?cmd:VM_EXEC);
    return 0;
}