#include "nasal.h"

void logo()
{
    std::cout<<"       __                _      "<<std::endl;
	std::cout<<"    /\\ \\ \\__ _ ___  __ _| |  "<<std::endl;
	std::cout<<"   /  \\/ / _` / __|/ _` | |    "<<std::endl;
	std::cout<<"  / /\\  / (_| \\__ \\ (_| | |  "<<std::endl;
	std::cout<<"  \\_\\ \\/ \\__,_|___/\\__,_|_|"<<std::endl;
    return;
}

nasal_resource resource;
nasal_lexer    lexer;
std::string    command;
int main()
{
#ifdef _WIN32
	// use chcp 65001 to use unicode io
	system("chcp 65001");
#endif
	// this curve looks really cool
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
	std::cout<<">> More info: http://wiki.flightgear.org/Nasal_scripting_language"<<std::endl;
	std::cout<<">> Input \"help\" to get help ."<<std::endl;
    while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
		{
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
		else if(command=="del")
		{
			resource.delete_file();
			std::cout<<">> [Delete] complete."<<std::endl;
		}
		else if(command=="lib")
			resource.load_lib();
		else if(command=="rs")
			resource.print_file();
		else if(command=="lex")
		{
			lexer.scanner(resource.get_file());
			if(!lexer.get_error())
				lexer.print_token();
			else
				std::cout<<">> [lexer] error occurred,stop.\n";
		}
		else if(command=="par")
		{
			;
		}
		else if(command=="ast")
		{
			;
		}
		else if(command=="run")
		{
            ;
		}
		else if(command=="logo")
			logo();
		else if(command=="exit")
			break;
		else
			resource.input_file(command);
	}
    return 0;
}