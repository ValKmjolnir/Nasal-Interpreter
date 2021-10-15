#include "nasal.h"
#define VM_LEXINFO   1
#define VM_ASTINFO   2
#define VM_CODEINFO  4
#define VM_EXECTIME  8
#define VM_OPCALLNUM 16
#define VM_EXEC      32
void help_cmd()
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
    <<"file:\n"
    <<"    input file name to execute script file.\n";
    return;
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
    <<"input \"nasal -h\" to get help .\n";
    return;
}

void die(const char* stage,const std::string& filename)
{
    std::cout<<"["<<stage<<"] in <"<<filename<<">: error(s) occurred,stop.\n";
    std::exit(1);
}

void cmderr()
{
    std::cout
    <<"invalid argument(s).\n"
    <<"use nasal -h to get help.\n";
    std::exit(1);
}

void execute(const std::string& file,const uint16_t cmd)
{
    nasal_lexer   lexer;
    nasal_parse   parse;
    nasal_import  import;
    nasal_codegen codegen;
    nasal_vm      vm;
    lexer.open(file);
    lexer.scan();
    if(lexer.err())
        die("lexer",file);
    if(cmd&VM_LEXINFO)
        lexer.print();
    parse.compile(lexer.get_tokens());
    if(parse.err())
        die("parse",file);
    if(cmd&VM_ASTINFO)
        parse.ast().print(0);
    // first used file is itself
    import.link(parse.ast(),file);
    if(import.err())
        die("import",file);
    codegen.compile(import.ast(),import.get_file());
    if(codegen.err())
        die("code",file);
    if(cmd&VM_CODEINFO)
        codegen.print();
    vm.init(
        codegen.get_strs(),
        codegen.get_nums(),
        import.get_file()
    );
    if(cmd&VM_EXECTIME)
    {
        clock_t t=clock();
        vm.run(codegen.get_code(),cmd&VM_OPCALLNUM);
        std::cout<<"process exited after "<<((double)(clock()-t))/CLOCKS_PER_SEC<<"s.\n";
    }
    else if(cmd&VM_EXEC)
        vm.run(codegen.get_code(),cmd&VM_OPCALLNUM);
    vm.clear();
    return;
}

int main(int argc,const char* argv[])
{
    if(argc==2 && (!strcmp(argv[1],"-v") || !strcmp(argv[1],"--version")))
        logo();
    else if(argc==2 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))
        help_cmd();
    else if(argc==2 && argv[1][0]!='-')
        execute(argv[1],VM_EXEC);
    else if(argc>=3)
    {
        uint16_t cmd=0;
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
            else
                cmderr();
        }
        execute(argv[argc-1],cmd);
    }
    else
        cmderr();
    return 0;
}