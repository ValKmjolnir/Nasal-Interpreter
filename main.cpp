#include "nasal.h"

const uint32_t VM_LEXINFO  =0x01;
const uint32_t VM_ASTINFO  =0x02;
const uint32_t VM_CODEINFO =0x04;
const uint32_t VM_EXECTIME =0x08;
const uint32_t VM_OPCALLNUM=0x10;
const uint32_t VM_EXEC     =0x20;
const uint32_t VM_DBGINFO  =0x40;
const uint32_t VM_DEBUG    =0x80;
const uint32_t VM_OPTIMIZE =0x100;

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
    <<"    -h, --help      | get help.\n"
    <<"    -v, --version   | get version of nasal interpreter.\n\n"
    <<"nasal <file>\n"
    <<"file:\n"
    <<"    input file name to execute script file.\n\n"
    <<"nasal [option...] <file> [argv...]\n"
    <<"option:\n"
    <<"    -l,   --lex     | view token info.\n"
    <<"    -a,   --ast     | view abstract syntax tree.\n"
    <<"    -c,   --code    | view bytecode.\n"
    <<"    -e,   --exec    | execute.\n"
    <<"    -t,   --time    | execute and get the running time.\n"
    <<"    -o,   --opcnt   | execute and count used operands.\n"
    <<"    -d,   --detail  | execute and get detail crash info.\n"
    <<"                    | get garbage collector info if didn't crash.\n"
    <<"    -op,  --optimize| use optimizer(beta).\n"
    <<"                    | if want to use -op and run, please use -op -e/-t/-o/-d.\n"
    <<"    -dbg, --debug   | debug mode (this will ignore -t -o -d -e).\n"
    <<"file:\n"
    <<"    input file name to execute script file.\n"
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
    <<"nasal ver : "<<__nasver<<"\n"
    <<"c++ std   : "<<__cplusplus<<"\n"
    <<"thanks to : https://github.com/andyross/nasal\n"
    <<"code repo : https://github.com/ValKmjolnir/Nasal-Interpreter\n"
    <<"code repo : https://gitee.com/valkmjolnir/Nasal-Interpreter\n"
    <<"lang info : http://wiki.flightgear.org/Nasal_scripting_language\n"
    <<"input <nasal -h> to get help .\n";
}

void err()
{
    std::cerr
    <<"invalid argument(s).\n"
    <<"use <nasal -h> to get help.\n";
    std::exit(1);
}

void execute(const std::string& file,const std::vector<std::string>& argv,const uint32_t cmd)
{
    // front end use the same error module
    nasal_err     nerr;
    nasal_lexer   lexer(nerr);
    nasal_parse   parse(nerr);
    nasal_import  linker(nerr);
    nasal_codegen gen(nerr);
    // back end
    nasal_vm      vm;

    // lexer scans file to get tokens
    lexer.scan(file);
    if(cmd&VM_LEXINFO)
        lexer.print();
    
    // parser gets lexer's token list to compile
    parse.compile(lexer);
    // linker gets parser's ast and load import files to this ast
    linker.link(parse,file);
    // optimizer does simple optimization on ast
    if(cmd&VM_OPTIMIZE)
        optimize(parse.ast());
    if(cmd&VM_ASTINFO)
        parse.print();
    
    // code generator gets parser's ast and linker's import file list to generate code
    gen.compile(parse,linker);
    if(cmd&VM_CODEINFO)
        gen.print();
    
    // run bytecode
    if(cmd&VM_DEBUG)
    {
        nasal_dbg debugger;
        debugger.run(gen,linker,argv);
    }
    else if(cmd&VM_EXECTIME)
    {
        auto start=std::chrono::high_resolution_clock::now();
        vm.run(gen,linker,argv,cmd&VM_OPCALLNUM,cmd&VM_DBGINFO);
        auto end=std::chrono::high_resolution_clock::now();
        std::clog<<"process exited after "<<(end-start).count()*1.0/std::chrono::high_resolution_clock::duration::period::den<<"s.\n";
    }
    else if(cmd&VM_EXEC)
        vm.run(gen,linker,argv,cmd&VM_OPCALLNUM,cmd&VM_DBGINFO);
}

int main(int argc,const char* argv[])
{
    if(argc<=1)
    {
        logo();
        return 0;
    }
    if(argc==2)
    {
        std::string s(argv[1]);
        if(s=="-v" || s=="--version")
            logo();
        else if(s=="-h" || s=="--help")
            help();
        else if(s[0]!='-')
            execute(s,{},VM_EXEC);
        else
            err();
        return 0;
    }
    std::unordered_map<std::string,uint32_t> cmdlst={
        {"--lex",VM_LEXINFO},{"-l",VM_LEXINFO},
        {"--ast",VM_ASTINFO},{"-a",VM_ASTINFO},
        {"--code",VM_CODEINFO},{"-c",VM_CODEINFO},
        {"--exec",VM_EXEC},{"-e",VM_EXEC},
        {"--opcnt",VM_OPCALLNUM|VM_EXEC},{"-o",VM_OPCALLNUM|VM_EXEC},
        {"--time",VM_EXECTIME|VM_EXEC},{"-t",VM_EXECTIME|VM_EXEC},
        {"--detail",VM_DBGINFO|VM_EXEC},{"-d",VM_DBGINFO|VM_EXEC},
        {"--optimize",VM_OPTIMIZE},{"-op",VM_OPTIMIZE},
        {"--debug",VM_DEBUG},{"-dbg",VM_DEBUG}
    };
    uint32_t cmd=0;
    std::string filename;
    std::vector<std::string> vm_argv;
    for(int i=1;i<argc;++i)
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
    if(!cmd)
        cmd|=VM_EXEC;
    execute(filename,vm_argv,cmd);
    return 0;
}