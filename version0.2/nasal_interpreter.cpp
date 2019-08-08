#include <iostream>
#include <cstring>
#include <cstdlib>
#include "nasal.h"

int main()
{
	std::string command;
	std::cout<<">> input \"help\" to find help."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
		{
			std::cout<<">> 1. input file name to run the nasal script."<<std::endl;
			std::cout<<">> 2. command cls to clear the screen."<<std::endl;
			std::cout<<">> 3. command exit to shut down the program."<<std::endl;
			std::cout<<">> 4. command lexer to see tokens in stack."<<std::endl;
			std::cout<<">> 5. command del to delete all elements in stack."<<std::endl;
			std::cout<<">> 6. command run to run the programme in stack."<<std::endl;
		}
		else if(command=="cls")
		{
			system("cls");
			//linux system("clear");
			//macOS system("clear");
		}
		else if(command=="exit")
			break;
		else if(command=="lexer")
			nasal::nasal_lexer.print();
		else if(command=="del")
			nasal::nasal_lexer.delete_all();
		else if(command=="run")
		{
			;
		}
		else
			nasal::RunProcess(command);
	}
	return 0;
}
