#include "nasal.h"

nasal_lexer    lexer;
nasal_parse    parse;
nasal_import   import;
std::string    file="null";
nasal_codegen  codegen;
nasal_vm       vm;

void help()
{
	std::cout
	<<">> [\"file\"] input file name.              \n"
	<<">> [help  ] show help.                      \n"
	<<">> [clear ] clear the screen.               \n"
	<<">> [lex   ] view tokens.                    \n"
	<<">> [ast   ] view abstract syntax tree.      \n"
	<<">> [code  ] view byte code.                 \n"
	<<">> [exec  ] execute program on bytecode vm. \n"
	<<">> [logo  ] print logo of nasal .           \n"
	<<">> [exit  ] quit nasal interpreter.         \n";
	return;
}

void logo()
{
    std::cout
	<<"       __                _      \n"
	<<"    /\\ \\ \\__ _ ___  __ _| |  \n"
	<<"   /  \\/ / _` / __|/ _` | |    \n"
	<<"  / /\\  / (_| \\__ \\ (_| | |  \n"
	<<"  \\_\\ \\/ \\__,_|___/\\__,_|_|\n";
    return;
}

void die(std::string stage,std::string filename)
{
	std::cout<<">> ["<<stage<<"] in <\""<<filename<<"\">: error(s) occurred,stop.\n";
	return;
}

void execute(std::string& command)
{
	lexer.openfile(file);
	lexer.scanner();
	if(lexer.get_error())
	{
		die("lexer",file);
		return;
	}
	if(command=="lex")
	{
		lexer.print_token();
		return;
	}
	parse.set_toklist(lexer.get_token_list());
	parse.main_process();
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
	import.link(parse.get_root());
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
		codegen.get_exec_code()
	);
	vm.run();
	vm.clear();
	return;
}

int main()
{
	std::string command;
#ifdef _WIN32
	// use chcp 65001 to use unicode io
	system("chcp 65001");
	system("cls");
#else
	system("clear");
#endif
	logo();
	std::cout<<">> Nasal interpreter ver 6.5 efficient gc test .\n";
	std::cout<<">> Thanks to https://github.com/andyross/nasal\n";
	std::cout<<">> Code: https://github.com/ValKmjolnir/Nasal-Interpreter\n";
	std::cout<<">> Code: https://gitee.com/valkmjolnir/Nasal-Interpreter\n";
	std::cout<<">> Info: http://wiki.flightgear.org/Nasal_scripting_language\n";
	std::cout<<">> Input \"help\" to get help .\n";
    while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
			help();
		else if(command=="clear")
		{
#ifdef _WIN32
			system("cls");
#else
			system("clear");
#endif
		}
		else if(command=="logo")
			logo();
		else if(command=="exit")
			break;
		else if(command=="lex" || command=="ast" || command=="code" || command=="exec")
			execute(command);
		else
			file=command;
	}
    return 0;
}