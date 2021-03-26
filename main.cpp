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
	<<">> [\"file\"] input a file name.                          \n"
	<<">> [help  ] show help.                                    \n"
	<<">> [clear ] clear the screen.                             \n"
	<<">> [lex   ] use lexer to turn code into tokens.           \n"
	<<">> [ast   ] do parsing and check the abstract syntax tree.\n"
	<<">> [code  ] show byte code.                               \n"
	<<">> [exec  ] execute program on bytecode vm.               \n"
	<<">> [logo  ] print logo of nasal .                         \n"
	<<">> [exit  ] quit nasal interpreter.                       \n";
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

void clear()
{
	// this will clear the data in lexer/parser/import modules
	// to reduce memory footprint
	lexer.get_token_list().clear();
	parse.get_root().clear();
	import.get_root().clear();
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
		clear();
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
		clear();
		return;
	}
	import.link(parse.get_root());
	if(import.get_error())
	{
		die("import",file);
		return;
	}
	codegen.main_progress(import.get_root());
	clear();
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
#endif
	logo();
	std::cout<<">> Nasal interpreter ver 6.0 .\n";
	std::cout<<">> Code: https://github.com/ValKmjolnir/Nasal-Interpreter\n";
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
#endif
#ifdef _linux_
			system("clear");
#endif
#ifdef TARGET_OS_MAC
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