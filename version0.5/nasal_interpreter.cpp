#include "nasal.h"

int main()
{
	std::string command;
	std::cout<<">> nasal-- script by ValKmjolnir"<<std::endl;
	std::cout<<">> input \"help\" to find help."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::getline(std::cin,command);
		if(command=="help")
		{
			std::cout<<">> nasal script by ValKmjolnir"<<std::endl;
			std::cout<<">> 1.         |input file name to run the lexer."<<std::endl;
			std::cout<<">> 2. \"cls\"   |clear the screen."<<std::endl;
			std::cout<<">> 3. \"exit\"  |shut down the program."<<std::endl;
			std::cout<<">> 4. \"lexer\" |see tokens in stack."<<std::endl;
			std::cout<<">> 5. \"parser\"|run parser."<<std::endl;
			std::cout<<">> 6. \"del\"   |delete all elements in stack."<<std::endl;
			std::cout<<">> 7. \"run\"   |run the programme in stack."<<std::endl;
			std::cout<<">> 8. \"rs\"    |check the source program."<<std::endl;
		}
		else if(command=="cls")
		{
			system("cls");
			//linux system("clear");
			//macOS system("clear");
		}
		else if(command=="rs")
			nasal::PrintSourceFile();
		else if(command=="exit")
		{
			break;
		}
		else if(command=="lexer")
			nasal::nasal_lexer.print();
		else if(command=="del")
		{
			nasal::ResourceProgram[0]=0;
			nasal::nasal_lexer.delete_all();
			nasal::nasal_parse.stack_set_empty();
			nasal::nasal_var_stack.delete_all();
			nasal::nasal_func_stack.delete_all();
		}
		else if(command=="parser")
		{
			nasal::nasal_parse.print_parser(nasal::nasal_lexer);
			nasal::nasal_parse.print_error();
		}
		else if(command=="run")
		{
			nasal::nasal_parse.run_parser(nasal::nasal_lexer);
			nasal::nasal_parse.print_error();
		}
		else
			nasal::RunProcess(command);// in this section the lexer is cleaned
	}
	return 0;
}
