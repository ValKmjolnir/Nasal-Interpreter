#include "nasal.h"
int main()
{
	resource_programme_process prog;
	nasal_lexer lex;
	nasal_parser pas;
	ast_generator cod;
	std::string command;
	std::cout<<">> Nasal interpreter by ValKmjolnir"<<std::endl;
	std::cout<<">> Input [help] to find help."<<std::endl;
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
			std::cout<<">> 6. [ast   ] |print the abstract syntax tree."<<std::endl;
			std::cout<<">> 7. [del   ] |delete program in memory."<<std::endl;
			std::cout<<">> 8. [run   ] |run the programme in stack. (-lexer -parser)"<<std::endl;
			std::cout<<">> 9. [rs    ] |check the source program."<<std::endl;
			std::cout<<">>10. [rule  ] |see rules of the language."<<std::endl;
		}
		else if(command=="cls")
		{
			system("cls");
			//windows  system("cls");
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
			lex.lexer_process(prog.use_file());
			lex.token_list_type_detail_edit();
			pas.parse_process(lex.return_list());
			pas.print_parser_stack();
		}
		else if(command=="ast")
		{
			lex.lexer_process(prog.use_file());
			lex.token_list_type_detail_edit();
			pas.parse_process(lex.return_list());
			cod.input_token_stack(pas.return_stack());
			cod.gen_main_prog();
			cod.print_ast();
		}
		else if(command=="run")
		{
			lex.lexer_process(prog.use_file());
			lex.token_list_type_detail_edit();
			pas.parse_process(lex.return_list());
			cod.input_token_stack(pas.return_stack());
			cod.gen_main_prog();
			cod.run();
		}
		else if(command=="rule")
		{
			std::cout<<">>[Rule] Token types that you can use: number + - * /"<<std::endl;
			std::cout<<">>[Rule] Each binary operator should have two numbers around it like: 1 + 1"<<std::endl;
		}
		else
			prog.input_file(command);
	}
	return 0;
}
