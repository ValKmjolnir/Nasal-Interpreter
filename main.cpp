#include "nasal.h"

void help_interact()
{
	std::cout
	<<">> [     ] input a file name to execute.  \n"
	<<">> [help ] show help.                     \n"
	<<">> [ast  ] view abstract syntax tree.     \n"
	<<">> [code ] view byte code.                \n"
	<<">> [exec ] execute program on bytecode vm.\n"
	<<">> [logo ] print logo of nasal .          \n"
	<<">> [exit ] quit nasal interpreter.        \n";
	return;
}
void help_cmd()
{
	std::cout
#ifdef _WIN32
	<<"use command \'chcp 65001\' if want to use unicode.\n"
#endif
	<<"nasal [option]|[file]\n"
	<<"    input 0 argument to use the interactive interpreter.\n"
	<<"option:\n"
	<<"    -h, --help    | get help.\n"
	<<"    -v, --version | get version of nasal interpreter.\n"
	<<"file:\n"
	<<"    input file name to execute script file.\n";
	return;
}
void info()
{
	std::cout
	<<">> thanks to https://github.com/andyross/nasal\n"
	<<">> code: https://github.com/ValKmjolnir/Nasal-Interpreter\n"
	<<">> code: https://gitee.com/valkmjolnir/Nasal-Interpreter\n"
	<<">> info: http://wiki.flightgear.org/Nasal_scripting_language\n"
	<<">> input \"help\" to get help .\n";
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
	<<"   nasal interpreter ver 7.0    \n";
    return;
}
void die(const char* stage,std::string& filename)
{
	std::cout<<">> ["<<stage<<"] in <\""<<filename<<"\">: error(s) occurred,stop.\n";
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
	parse.main_process(lexer.get_token_list());
	if(parse.get_error())
	{
		die("parse",file);
		return;
	}
	if(command=="ast")
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
	codegen.main_progress(import.get_root());
	if(codegen.get_error())
	{
		die("codegen",file);
		return;
	}
	if(command=="code")
	{
		codegen.print_byte_code();
		return;
	}
	vm.init(
		codegen.get_str_table(),
		codegen.get_num_table(),
		import.get_file()
	);
	vm.run(codegen.get_exec_code());
	vm.clear();
	return;
}
void interact()
{
#ifdef _WIN32
	// use chcp 65001 to use unicode io
	system("chcp 65001");
#endif
	std::string command,file="null";
	logo();
	info();
    while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
			help_interact();
		else if(command=="logo")
			logo();
		else if(command=="exit")
			return;
		else if(command=="ast" || command=="code" || command=="exec")
			execute(file,command);
		else
			file=command;
	}
}
int main(int argc,const char* argv[])
{
	std::string command,file="null";
	if(argc==1)
		interact();
	else if(argc==2 && (!strcmp(argv[1],"-v") || !strcmp(argv[1],"--version")))
		logo();
	else if(argc==2 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))
		help_cmd();
	else if(argc==2 && argv[1][0]!='-')
	{
		file=argv[1];
		command="exec";
		execute(file,command);
		return 0;
	}
	else
	{
		std::cout
		<<"invalid command.\n"
		<<"use nasal -h to get help.\n";
	}
    return 0;
}