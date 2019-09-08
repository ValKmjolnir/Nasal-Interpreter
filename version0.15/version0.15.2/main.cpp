#include "nasal.h"
int main()
{
	resource_programme_process prog;
	nasal_lexer lex;
	nasal_parser pas;
	code_generator cod;
	std::string command;
	std::cout<<">> Nasal interpreter by ValKmjolnir"<<std::endl;
	std::cout<<">> input [help] to find help."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::getline(std::cin,command);
		if(command=="help")
		{
			std::cout<<">> Nasal interpreter by ValKmjolnir"<<std::endl;
			std::cout<<">> 1. [      ] |input file name to load the file."<<std::endl;
			std::cout<<">> 2. [cls   ] |clear the screen."<<std::endl;
			std::cout<<">> 3. [exit  ] |shut down the interpreter."<<std::endl;
			std::cout<<">> 4. [lexer ] |run and show the lexer. (-lexer)"<<std::endl;
			std::cout<<">> 5. [parser] |run parser. (-parser)"<<std::endl;
			std::cout<<">> 6. [del   ] |delete program in memory."<<std::endl;
			std::cout<<">> 7. [run   ] |run the programme in stack. (-lexer -parser)"<<std::endl;
			std::cout<<">> 8. [rs    ] |check the source program."<<std::endl;
		}
		else if(command=="cls")
		{
			system("cls");
			//windows  system("cls);
			//linux system("clear");
			//macOS system("clear");
		}
		else if(command=="rs")
			prog.print_file();
		else if(command=="exit")
		{
			break;
		}
		else if(command=="lexer")
		{
			lex.lexer_process(prog.use_file());
			lex.print_lexer();
		}
		else if(command=="del")
		{
			prog.del_file();
			std::cout<<">>[Delete] Complete."<<std::endl;
		}
		else if(command=="parser")
		{
			pas.parse_process(lex.return_list());
			pas.print_parser_stack();
		}
		else if(command=="run")
		{
			lex.lexer_process(prog.use_file());
			pas.parse_process(lex.return_list());
			cod.input_stack(pas.return_stack());
			cod.run();
		}
		else
			prog.input_file(command);
	}
	return 0;
}
