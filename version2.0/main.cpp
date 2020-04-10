#include "nasal.h"

// source code will be put in resource
resource_file resource;
// source code will be generated to tokens in lexer
nasal_lexer lexer;
// token list will be checked in parser and output the abstract syntax tree
nasal_parse parser;
// libroot stores the ast of lib file
abstract_syntax_tree libroot;
// root stores the ast of source code
abstract_syntax_tree root;
// executable_ast generates libroot and root together
// this ast will be sent into nasal runtime
abstract_syntax_tree executable_ast;
// main process is running here
nasal_runtime runtime;

// command is used in main()
std::string command;
int main()
{
#ifdef _WIN32
	// use chcp 65001 to use unicode io
	system("chcp 65001");
#endif
	// this curve looks really cool
	std::cout<<"       __                _      "<<std::endl;
	std::cout<<"    /\\ \\ \\__ _ ___  __ _| |  "<<std::endl;
	std::cout<<"   /  \\/ / _` / __|/ _` | |    "<<std::endl;
	std::cout<<"  / /\\  / (_| \\__ \\ (_| | |  "<<std::endl;
	std::cout<<"  \\_\\ \\/ \\__,_|___/\\__,_|_|"<<std::endl;
#ifdef _WIN32
	std::cout<<">> [system] Windows system."<<std::endl;
#endif
#ifdef _linux_
	std::cout<<">> [system] Linux system."<<std::endl;
#endif
#ifdef TARGET_OS_MAC
	std::cout<<">> [system] MacOS system."<<std::endl;
#endif
	std::cout<<">> Nasal interpreter ver 2.0 ."<<std::endl;
	std::cout<<">> Code: https://github.com/ValKmjolnir/Nasal-Interpreter"<<std::endl;
	std::cout<<">> More info: http://wiki.flightgear.org/Nasal_scripting_language"<<std::endl;
	std::cout<<">> Input \"help\" to get help ."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
		{
			std::cout<<">> Be careful that this program does not support unicode(unicode will be set to \'?\')"<<std::endl;
			std::cout<<">> [\'file\'] input a file."<<std::endl;
			std::cout<<">> [cls   ] clear the screen."<<std::endl;
			std::cout<<">> [del   ] clear the resource code."<<std::endl;
			std::cout<<">> [lib   ] add lib file."<<std::endl;
			std::cout<<">> [rs    ] print resource code."<<std::endl;
			std::cout<<">> [lex   ] turn code into tokens."<<std::endl;
			std::cout<<">> [par   ] turn tokens into abstract syntax tree."<<std::endl;
			std::cout<<">> [ast   ] check the abstract syntax tree."<<std::endl;
			std::cout<<">> [run   ] run code."<<std::endl;
			std::cout<<">> [logo  ] print logo of nasal ."<<std::endl;
			std::cout<<">> [exit  ] quit nasal interpreter."<<std::endl;
		}
		// clear the window
		else if(command=="cls")
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
		// del all the source codes and asts
		else if(command=="del")
		{
			resource.delete_all_source();
			lexer.delete_all_tokens();
			parser.delete_all_elements();
			root.set_clear();
			executable_ast.set_clear();
			std::cout<<">> [Delete] complete."<<std::endl;
		}
		// add lib
		else if(command=="lib")
		{
			libroot.set_clear();
			resource.load_lib_file();
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
				{
					libroot=parser.get_root();
					std::cout<<">> [Lib] loaded."<<std::endl;
				}
				else
					std::cout<<">> [Lib] lib files have parse error(s),stop."<<std::endl;
			}
			else
				std::cout<<">> [Lib] lib files have lexer error(s),stop."<<std::endl;
			resource.delete_all_source();
			lexer.delete_all_tokens();
			parser.delete_all_elements();
		}
		// print source codes
		else if(command=="rs")
			resource.print_resource();
		// print detail token after scanning source codes
		else if(command=="lex")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
				lexer.print_token_list();
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		// print the parse result of source codes
		else if(command=="par")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.print_detail_token();
				parser.main_generate();
			}
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		// print the ast of source codes
		else if(command=="ast")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
					parser.get_root().print_tree();
				else
					std::cout<<">> [Parse] error occurred,stop."<<std::endl;
			}
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		// running process begins here
		else if(command=="run")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
				{
					executable_ast.set_clear();
					executable_ast=libroot;
					executable_ast.merge_children(parser.get_root());
					runtime.main_proc(executable_ast);
					//root.print_tree();
				}
				else
					std::cout<<">> [Parse] error occurred,stop."<<std::endl;
			}
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		// do you wanna see it again?
		else if(command=="logo")
		{
			std::cout<<"       __                _      "<<std::endl;
			std::cout<<"    /\\ \\ \\__ _ ___  __ _| |  "<<std::endl;
			std::cout<<"   /  \\/ / _` / __|/ _` | |    "<<std::endl;
			std::cout<<"  / /\\  / (_| \\__ \\ (_| | |  "<<std::endl;
			std::cout<<"  \\_\\ \\/ \\__,_|___/\\__,_|_|"<<std::endl;
		}
		// exit interpreter
		else if(command=="exit")
			break;
		else
			resource.input_file(command);
	}
	return 0;
}
