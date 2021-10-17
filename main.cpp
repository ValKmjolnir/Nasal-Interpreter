#include "nasal.h"
constexpr uint32_t VM_LEXINFO  =1;
constexpr uint32_t VM_ASTINFO  =2;
constexpr uint32_t VM_CODEINFO =4;
constexpr uint32_t VM_EXECTIME =8;
constexpr uint32_t VM_OPCALLNUM=16;
constexpr uint32_t VM_EXEC     =32;
constexpr uint32_t VM_DBGINFO  =64;
void help()
{
    std::cout
#ifdef _WIN32
    <<"use command \'chcp 65001\' if want to use unicode.\n"
#endif
    <<"nasal <option>\n"
    <<"option:\n"
    <<"    -h, --help    | get help.\n"
    <<"    -v, --version | get version of nasal interpreter.\n\n"
    <<"nasal <file>\n"
    <<"file:\n"
    <<"    input file name to execute script file.\n\n"
    <<"nasal [options] <file>\n"
    <<"option:\n"
    <<"    -l, --lex     | view token info.\n"
    <<"    -a, --ast     | view abstract syntax tree.\n"
    <<"    -c, --code    | view bytecode.\n"
    <<"    -t, --time    | execute and get the running time.\n"
    <<"    -o, --opcnt   | count operands while running.\n"
    <<"    -d, --detail  | get detail crash info.\n"
    <<"file:\n"
    <<"    input file name to execute script file.\n";
}

void logo()
{
    std::cout
    <<"       __                _      \n"
    <<"    /\\ \\ \\__ _ ___  __ _| |  \n"
    <<"   /  \\/ / _` / __|/ _` | |    \n"
    <<"  / /\\  / (_| \\__ \\ (_| | |  \n"
    <<"  \\_\\ \\/ \\__,_|___/\\__,_|_|\n"
    <<"nasal interpreter ver 8.0\n"
    <<"thanks to : https://github.com/andyross/nasal\n"
    <<"code repo : https://github.com/ValKmjolnir/Nasal-Interpreter\n"
    <<"code repo : https://gitee.com/valkmjolnir/Nasal-Interpreter\n"
    <<"lang info : http://wiki.flightgear.org/Nasal_scripting_language\n"
    <<"input <nasal -h> to get help .\n";
}

void die(const char* stage,const std::string& filename)
{
    std::cout<<"["<<stage<<"] in <"<<filename<<">: error(s) occurred,stop.\n";
    std::exit(1);
}

void err()
{
    std::cout
    <<"invalid argument(s).\n"
    <<"use <nasal -h> to get help.\n";
    std::exit(1);
}

void execute(const std::string& file,const uint32_t cmd)
{
    nasal_lexer   lexer;
    nasal_parse   parse;
    nasal_import  linker;
    nasal_codegen gen;
    nasal_vm      vm;

    // lexer scans file to get tokens
    lexer.scan(file);
    if(lexer.err())
        die("lexer",file);
    
    // parser gets lexer's token list to compile
    parse.compile(lexer);
    if(parse.err())
        die("parse",file);

    // linker gets parser's ast and load import files to this ast
    linker.link(parse,file);
    if(linker.err())
        die("import",file);
    
    // code generator gets parser's ast and linker's import file list to generate code
    gen.compile(parse,linker);
    if(gen.err())
        die("code",file);

    if(cmd&VM_LEXINFO)
        lexer.print();
    if(cmd&VM_ASTINFO)
        parse.print();
    if(cmd&VM_CODEINFO)
        gen.print();
    if(cmd&VM_EXECTIME)
    {
        clock_t t=clock();
        vm.run(gen,linker,cmd&VM_OPCALLNUM,cmd&VM_DBGINFO);
        std::cout<<"process exited after "<<((double)(clock()-t))/CLOCKS_PER_SEC<<"s.\n";
    }
    else if(cmd&VM_EXEC)
        vm.run(gen,linker,cmd&VM_OPCALLNUM,cmd&VM_DBGINFO);
}

int main(int argc,const char* argv[])
{
    if(argc==2 && (!strcmp(argv[1],"-v") || !strcmp(argv[1],"--version")))
        logo();
    else if(argc==2 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))
        help();
    else if(argc==2 && argv[1][0]!='-')
        execute(argv[1],VM_EXEC);
    else if(argc>=3)
    {
        uint32_t cmd=0;
        for(int i=1;i<argc-1;++i)
        {
            std::string s(argv[i]);
            if(s=="--lex" || s=="-l")
                cmd|=VM_LEXINFO;
            else if(s=="--ast" || s=="-a")
                cmd|=VM_ASTINFO;
            else if(s=="--code" || s=="-c")
                cmd|=VM_CODEINFO;
            else if(s=="--opcnt" || s=="-o")
                cmd|=VM_OPCALLNUM|VM_EXEC;
            else if(s=="--time" || s=="-t")
                cmd|=VM_EXECTIME;
            else if(s=="--detail" || s=="-d")
                cmd|=VM_DBGINFO|VM_EXEC;
            else
                err();
        }
        execute(argv[argc-1],cmd);
    }
    else
        err();
    return 0;
}