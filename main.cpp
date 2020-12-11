#include "nasal.h"

nasal_lexer    lexer;
nasal_parse    parse;
nasal_import   import;
std::string    inputfile="null";
nasal_runtime  runtime;
nasal_codegen  code_generator;
nasal_bytecode_vm bytevm;

void help()
{
	std::cout<<">> [\"file\"] input a file name.\n";
	std::cout<<">> [help  ] show help.\n";
	std::cout<<">> [clear ] clear the screen.\n";
	std::cout<<">> [del   ] clear the input filename.\n";
	std::cout<<">> [lex   ] use lexer to turn code into tokens.\n";
	std::cout<<">> [ast   ] do parsing and check the abstract syntax tree.\n";
	std::cout<<">> [run   ] run abstract syntax tree.\n";
	std::cout<<">> [exec  ] generate byte code.\n";
	std::cout<<">> [logo  ] print logo of nasal .\n";
	std::cout<<">> [exit  ] quit nasal interpreter.\n";
	return;
}

void logo()
{
    std::cout<<"       __                _      \n";
	std::cout<<"    /\\ \\ \\__ _ ___  __ _| |  \n";
	std::cout<<"   /  \\/ / _` / __|/ _` | |    \n";
	std::cout<<"  / /\\  / (_| \\__ \\ (_| | |  \n";
	std::cout<<"  \\_\\ \\/ \\__,_|___/\\__,_|_|\n";
    return;
}

void del_func()
{
	lexer.clear();
	parse.clear();
	inputfile="null";
	std::cout<<">> [Delete] complete.\n";
	return;
}

void die(std::string stage,std::string filename)
{
	std::cout<<">> ["<<stage<<"] in <\""<<filename<<"\">: error(s) occurred,stop.\n";
	return;
}

void lex_func()
{
	lexer.openfile(inputfile);
	lexer.scanner();
	if(lexer.get_error())
	{
		die("lexer",inputfile);
		return;
	}
	lexer.print_token();
	return;
}

void ast_print()
{
	lexer.openfile(inputfile);
	lexer.scanner();
	if(lexer.get_error())
	{
		die("lexer",inputfile);
		return;
	}
	parse.set_toklist(lexer.get_token_list());
	parse.main_process();
	if(parse.get_error())
	{
		die("parse",inputfile);
		return;
	}
	parse.get_root().print_ast(0);
	return;
}

void runtime_start()
{
	lexer.openfile(inputfile);
	lexer.scanner();
	if(lexer.get_error())
	{
		die("lexer",inputfile);
		return;
	}
	parse.set_toklist(lexer.get_token_list());
	parse.main_process();
	if(parse.get_error())
	{
		die("parse",inputfile);
		return;
	}
	import.link(parse.get_root());
	if(import.get_error())
	{
		die("import",inputfile);
		return;
	}
	runtime.set_root(import.get_root());
	runtime.run();
	return;
}

void execute()
{
	lexer.openfile(inputfile);
	lexer.scanner();
	if(lexer.get_error())
	{
		die("lexer",inputfile);
		return;
	}
	parse.set_toklist(lexer.get_token_list());
	parse.main_process();
	if(parse.get_error())
	{
		die("parse",inputfile);
		return;
	}
	import.link(parse.get_root());
	if(import.get_error())
	{
		die("import",inputfile);
		return;
	}
	code_generator.main_progress(import.get_root());
	code_generator.print_byte_code();
	bytevm.run(
		code_generator.get_string_table(),
		code_generator.get_number_table(),
		code_generator.get_exec_code()
	);
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
#ifdef _WIN32
	std::cout<<">> [system] Windows system.\n";
#endif
#ifdef _linux_
	std::cout<<">> [system] Linux system.\n";
#endif
#ifdef TARGET_OS_MAC
	std::cout<<">> [system] MacOS system.\n";
#endif
	
	std::cout<<">> Nasal interpreter ver 3.0 .\n";
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
		else if(command=="del")
			del_func();
		else if(command=="lex")
			lex_func();
		else if(command=="ast")
			ast_print();
		else if(command=="run")
			runtime_start();
		else if(command=="exec")
			execute();
		else if(command=="logo")
			logo();
		else if(command=="exit")
			break;
		else
			inputfile=command;
	}
    return 0;
}