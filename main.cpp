#include "nasal.h"
#include "nasal_err.h"
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "nasal_opt.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"
#include "nasal_codegen.h"
#include "nasal_vm.h"
#include "nasal_dbg.h"

#include <unordered_map>

const u32 VM_TOKEN =0x01;
const u32 VM_AST   =0x02;
const u32 VM_CODE  =0x04;
const u32 VM_TIME  =0x08;
const u32 VM_EXEC  =0x10;
const u32 VM_DETAIL=0x20;
const u32 VM_DEBUG =0x40;
const u32 VM_OPT   =0x80;

void help()
{
    std::clog
    <<"     ,--#-,\n"
    <<"<3  / \\____\\  <3\n"
    <<"    |_|__A_|\n"
#ifdef _WIN32
    <<"use command <chcp 65001> if want to use unicode.\n"
#endif
    <<"nasal <option>\n"
    <<"option:\n"
    <<"   -h,   --help     | get this help and exit.\n"
    <<"   -v,   --version  | get version of nasal interpreter.\n\n"
    <<"nasal [option...] <file> [argv...]\n"
    <<"option:\n"
    <<"   -l,   --lex      | view token info.\n"
    <<"   -a,   --ast      | view abstract syntax tree.\n"
    <<"   -c,   --code     | view bytecode.\n"
    <<"   -e,   --exec     | execute.\n"
    <<"   -t,   --time     | get the running time.\n"
    <<"   -d,   --detail   | get detail runtime crash info.\n"
    <<"                    | get detail linker path-not-found info.\n"
    <<"                    | get garbage collector info if didn't crash.\n"
    <<"   -o,   --optimize | use optimizer(beta).\n"
    <<"                    | if want to use -op and run, please use -op -e/-t/-d.\n"
    <<"   -dbg, --debug    | debug mode (this will ignore -t -d).\n"
    <<"file:\n"
    <<"    input file name to execute.\n"
    <<"argv:\n"
    <<"    command line arguments used in program.\n";
}

void logo()
{
    std::clog
    <<"       __                _\n"
    <<"    /\\ \\ \\__ _ ___  __ _| |\n"
    <<"   /  \\/ / _` / __|/ _` | |\n"
    <<"  / /\\  / (_| \\__ \\ (_| | |\n"
    <<"  \\_\\ \\/ \\__,_|___/\\__,_|_|\n"
    <<"version   : "<<__nasver<<" ("<<__DATE__<<" "<<__TIME__<<")\n"
    <<"c++ std   : "<<__cplusplus<<"\n"
    <<"thanks to : https://github.com/andyross/nasal\n"
    <<"code repo : https://github.com/ValKmjolnir/Nasal-Interpreter\n"
    <<"code repo : https://gitee.com/valkmjolnir/Nasal-Interpreter\n"
    <<"lang info : http://wiki.flightgear.org/Nasal_scripting_language\n"
    <<"input <nasal -h> to get help .\n";
}

[[noreturn]]
void err()
{
    std::cerr
    <<"invalid argument(s).\n"
    <<"use <nasal -h> to get help.\n";
    std::exit(1);
}

void execute(const string& file,const std::vector<string>& argv,const u32 cmd)
{
    // front end use the same error module
    error   err;
    lexer   lex(err);
    parse   parse(err);
    linker  ld(err);
    codegen gen(err);
    // back end
    vm      rt;

    // lexer scans file to get tokens
    lex.scan(file);
    if(cmd&VM_TOKEN)
        lex.print();
    
    // parser gets lexer's token list to compile
    parse.compile(lex);
    // linker gets parser's ast and load import files to this ast
    ld.link(parse,file,cmd&VM_DETAIL);
    // optimizer does simple optimization on ast
    if(cmd&VM_OPT)
        optimize(parse.tree());
    if(cmd&VM_AST)
        parse.print();
    
    // code generator gets parser's ast and linker's import file list to generate code
    gen.compile(parse,ld);
    if(cmd&VM_CODE)
        gen.print();
    
    // run
    if(cmd&VM_DEBUG)
        debugger(err).run(gen,ld,argv);
    else if(cmd&VM_TIME)
    {
        auto start=std::chrono::high_resolution_clock::now();
        rt.run(gen,ld,argv,cmd&VM_DETAIL);
        auto end=std::chrono::high_resolution_clock::now();
        std::clog<<"process exited after "<<(end-start).count()*1.0/std::chrono::high_resolution_clock::duration::period::den<<"s.\n";
    }
    else if(cmd&VM_EXEC)
        rt.run(gen,ld,argv,cmd&VM_DETAIL);
}

i32 main(i32 argc,const char* argv[])
{
    if(argc<=1)
    {
        logo();
        return 0;
    }
    if(argc==2)
    {
        string s(argv[1]);
        if(s=="-v" || s=="--version")
            std::clog<<"nasal "<<__nasver<<" ("<<__DATE__<<" "<<__TIME__<<")\n";
        else if(s=="-h" || s=="--help")
            help();
        else if(s[0]!='-')
            execute(s,{},VM_EXEC);
        else
            err();
        return 0;
    }
    std::unordered_map<string,u32> cmdlst={
        {"--lex",VM_TOKEN},{"-l",VM_TOKEN},
        {"--ast",VM_AST},{"-a",VM_AST},
        {"--code",VM_CODE},{"-c",VM_CODE},
        {"--exec",VM_EXEC},{"-e",VM_EXEC},
        {"--time",VM_TIME|VM_EXEC},{"-t",VM_TIME|VM_EXEC},
        {"--detail",VM_DETAIL|VM_EXEC},{"-d",VM_DETAIL|VM_EXEC},
        {"--optimize",VM_OPT},{"-o",VM_OPT},
        {"--debug",VM_DEBUG},{"-dbg",VM_DEBUG}
    };
    u32 cmd=0;
    string filename;
    std::vector<string> vm_argv;
    for(i32 i=1;i<argc;++i)
    {
        if(cmdlst.count(argv[i]))
            cmd|=cmdlst[argv[i]];
        else if(!filename.length())
            filename=argv[i];
        else
            vm_argv.push_back(argv[i]);
    }
    if(!filename.length())
        err();
    execute(filename,vm_argv,cmd?cmd:VM_EXEC);
    return 0;
}