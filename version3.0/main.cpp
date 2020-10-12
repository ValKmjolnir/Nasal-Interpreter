#include "nasal.h"

nasal_resource resource;
nasal_lexer    lexer;
nasal_parse    parse;
nasal_import   preprocessor;
nasal_codegen  code_generator;
std::string    command;
std::string    inputfile="null";
nasal_runtime  runtime;

void help()
{
	std::cout<<">> [\"file\"] input a file."<<std::endl;
	std::cout<<">> [clear ] clear the screen."<<std::endl;
	std::cout<<">> [del   ] clear the input filename."<<std::endl;
	std::cout<<">> [rs    ] print source code."<<std::endl;
	std::cout<<">> [lex   ] use lexer to turn code into tokens."<<std::endl;
	std::cout<<">> [ast   ] do parsing and check the abstract syntax tree."<<std::endl;
	std::cout<<">> [run   ] run abstract syntax tree."<<std::endl;
	std::cout<<">> [exec  ] generate byte code."<<std::endl;
	std::cout<<">> [erun  ] run byte code."<<std::endl;
	std::cout<<">> [logo  ] print logo of nasal ."<<std::endl;
	std::cout<<">> [exit  ] quit nasal interpreter."<<std::endl;
	return;
}

void logo()
{
    std::cout<<"       __                _      "<<std::endl;
	std::cout<<"    /\\ \\ \\__ _ ___  __ _| |  "<<std::endl;
	std::cout<<"   /  \\/ / _` / __|/ _` | |    "<<std::endl;
	std::cout<<"  / /\\  / (_| \\__ \\ (_| | |  "<<std::endl;
	std::cout<<"  \\_\\ \\/ \\__,_|___/\\__,_|_|"<<std::endl;
    return;
}

void del_func()
{
	resource.clear();
	lexer.clear();
	parse.clear();
	inputfile="null";
	std::cout<<">> [Delete] complete."<<std::endl;
	return;
}

void die(std::string stage,std::string filename)
{
	std::cout<<">> ["<<stage<<"] in <\""<<filename<<"\">: error(s) occurred,stop."<<std::endl;
	return;
}

void lex_func()
{
	if(!resource.input_file(inputfile))
	{
		die("resource",inputfile);
		return;
	}
	lexer.scanner(resource.get_file());
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
	if(!resource.input_file(inputfile))
	{
		die("resource",inputfile);
		return;
	}
	lexer.scanner(resource.get_file());
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
	if(!resource.input_file(inputfile))
	{
		die("resource",inputfile);
		return;
	}
	lexer.scanner(resource.get_file());
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
	preprocessor.preprocessing(parse.get_root());
	if(preprocessor.get_error())
	{
		die("import",inputfile);
		return;
	}
	runtime.set_root(preprocessor.get_root());
	runtime.run();
	return;
}

void codegen_start()
{
	if(!resource.input_file(inputfile))
	{
		die("resource",inputfile);
		return;
	}
	lexer.scanner(resource.get_file());
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
	preprocessor.preprocessing(parse.get_root());
	if(preprocessor.get_error())
	{
		die("import",inputfile);
		return;
	}
	runtime.set_root(preprocessor.get_root());
	code_generator.output_exec(inputfile+".naexec",preprocessor.get_root());
	if(code_generator.get_error())
	{
		die("code",inputfile);
		return;
	}
	return;
}

void execution_start()
{
	code_generator.load_exec(inputfile,preprocessor.get_root());
	if(code_generator.get_error())
	{
		die("code",inputfile);
		return;
	}
	runtime.set_root(preprocessor.get_root());
	runtime.run();
	return;
}

int main()
{
#ifdef _WIN32
	// use chcp 65001 to use unicode io
	system("chcp 65001");
	system("cls");
#endif
	logo();
#ifdef _WIN32
	std::cout<<">> [system] Windows system."<<std::endl;
#endif
#ifdef _linux_
	std::cout<<">> [system] Linux system."<<std::endl;
#endif
#ifdef TARGET_OS_MAC
	std::cout<<">> [system] MacOS system."<<std::endl;
#endif
	
	std::cout<<">> Nasal interpreter ver 3.0 ."<<std::endl;
	std::cout<<">> Code: https://github.com/ValKmjolnir/Nasal-Interpreter"<<std::endl;
	std::cout<<">> Info: http://wiki.flightgear.org/Nasal_scripting_language"<<std::endl;
	std::cout<<">> Input \"help\" to get help ."<<std::endl;
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
		else if(command=="rs")
		{
			if(resource.input_file(inputfile))
				resource.print_file();
		}
		else if(command=="lex")
			lex_func();
		else if(command=="ast")
			ast_print();
		else if(command=="run")
			runtime_start();
		else if(command=="exec")
			codegen_start();
		else if(command=="erun")
			execution_start();
		else if(command=="logo")
			logo();
		else if(command=="exit")
			break;
		else
		{
			inputfile=command;
			std::ifstream fin(command);
			if(fin.fail())
			{
				std::cout<<">> [file] cannot open file \""<<command<<"\"."<<std::endl;
				inputfile="null";
			}
			fin.close();
		}
	}
    return 0;
}