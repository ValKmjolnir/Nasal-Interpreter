#include "nasal.h"

void help_cmd()
{
    std::cout
#ifdef _WIN32
    <<"use command \'chcp 65001\' if want to use unicode.\n"
#endif
    <<"nasal [option]\n"
    <<"option:\n"
    <<"    -h, --help    | get help.\n"
    <<"    -v, --version | get version of nasal interpreter.\n\n"
    <<"nasal [file]\n"
    <<"file:\n"
    <<"    input file name to execute script file.\n\n"
    <<"nasal [option] [file]\n"
    <<"option:\n"
    <<"    -l, --lex     | view token info.\n"
    <<"    -a, --ast     | view abstract syntax tree.\n"
    <<"    -c, --code    | view bytecode.\n"
    <<"    -e, --exec    | execute script file.\n"
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
    <<"nasal interpreter ver 7.0\n"
    <<"thanks to : https://github.com/andyross/nasal\n"
    <<"code repo : https://github.com/ValKmjolnir/Nasal-Interpreter\n"
    <<"code repo : https://gitee.com/valkmjolnir/Nasal-Interpreter\n"
    <<"lang info : http://wiki.flightgear.org/Nasal_scripting_language\n"
    <<"input \"nasal -h\" to get help .\n";
    return;
}

void die(const char* stage,std::string& filename)
{
    std::cout<<"["<<stage<<"] in <"<<filename<<">: error(s) occurred,stop.\n";
    return;
}

void execute(std::string& file,std::string& command)
{
    nasal_lexer   lexer;
    nasal_parse   parse;
    nasal_import  import;
    nasal_codegen codegen;
    nasal_vm      vm;
    lexer.openfile(file);
    lexer.scanner();
    if(lexer.get_error())
    {
        die("lexer",file);
        return;
    }
    if(command=="--lex" || command=="-l")
    {
        lexer.print_token();
        return;
    }
    parse.main_process(lexer.get_token_list());
    if(parse.get_error())
    {
        die("parse",file);
        return;
    }
    if(command=="--ast" || command=="-a")
    {
        parse.get_root().print_ast(0);
        return;
    }
    // first used file is itself
    import.link(parse.get_root(),file);
    if(import.get_error())
    {
        die("import",file);
        return;
    }
    codegen.main_progress(import.get_root(),import.get_file());
    if(codegen.get_error())
    {
        die("code",file);
        return;
    }
    if(command=="--code" || command=="-c")
    {
        codegen.print_byte_code();
        return;
    }
    vm.init(
        codegen.get_str_table(),
        codegen.get_num_table(),
        import.get_file()
    );
    if(command=="--exec" || command=="-e" || command=="--opcnt" || command=="-o")
        vm.run(codegen.get_exec_code(),command=="--opcnt" || command=="-o");
    else if(command=="--time" || command=="-t")
    {
        clock_t begin=clock();
        vm.run(codegen.get_exec_code(),false);
        std::cout<<"process exited after "<<((double)(clock()-begin))/CLOCKS_PER_SEC<<"s.\n";
    }
    vm.clear();
    return;
}
int main(int argc,const char* argv[])
{
    std::string command,file;
    if(argc==2 && (!strcmp(argv[1],"-v") || !strcmp(argv[1],"--version")))
        logo();
    else if(argc==2 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))
        help_cmd();
    else if(argc==2 && argv[1][0]!='-')
    {
        file=argv[1];
        command="-e";
        execute(file,command);
    }
    else if(argc==3 &&
        (!strcmp(argv[1],"--lex") ||
        !strcmp(argv[1],"-l")     ||
        !strcmp(argv[1],"--ast")  ||
        !strcmp(argv[1],"-a")     ||
        !strcmp(argv[1],"--code") ||
        !strcmp(argv[1],"-c")     ||
        !strcmp(argv[1],"--exec") ||
        !strcmp(argv[1],"-e")     ||
        !strcmp(argv[1],"--opcnt")||
        !strcmp(argv[1],"-o")     ||
        !strcmp(argv[1],"--time") ||
        !strcmp(argv[1],"-t")))
    {
        file=argv[2];
        command=argv[1];
        execute(file,command);
    }
    else
    {
        std::cout
        <<"invalid argument(s).\n"
        <<"use nasal -h to get help.\n";
    }
    return 0;
}