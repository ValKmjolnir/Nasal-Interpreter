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
			std::cout<<">> nasal-- script by ValKmjolnir"<<std::endl;
			std::cout<<">> 1. input file name to run the lexer."<<std::endl;
			std::cout<<">> 2. command \"cls\" to clear the screen."<<std::endl;
			std::cout<<">> 3. command \"exit\" to shut down the program."<<std::endl;
			std::cout<<">> 4. command \"lexer\" to see tokens in stack."<<std::endl;
			std::cout<<">> 5. command \"parser\" to run parser."<<std::endl;
			std::cout<<">> 6. command \"del\" to delete all elements in stack."<<std::endl;
			std::cout<<">> 7. command \"run\" to run the programme in stack."<<std::endl;
			std::cout<<">> 8. command \"rs\" to check the source program."<<std::endl;
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
			nasal::nasal_lexer.delete_all();
			nasal::nasal_var_stack.delete_all();
			nasal::nasal_func_stack.delete_all();
		}
		else if(command=="parser")
			nasal::nasal_parse.parse_work(nasal::nasal_lexer);
		else if(command=="run")
		{
			;
		}
		else
			nasal::RunProcess(command);
	}
	return 0;
}
