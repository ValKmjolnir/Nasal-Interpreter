#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <cstdlib>
#include "nasal.h"

using namespace nasal;

int main()
{
	std::cout<<">> input \"help\" to find help."<<std::endl;
	std::string Command;
	while(1)
	{
		std::cout<<">> ";
		std::cin>>Command;
		if(Command=="help")
		{
			std::cout<<">> 1.input file name to run the nasal script."<<std::endl;
			std::cout<<">> 2.command cls to clear the screen."<<std::endl;
			std::cout<<">> 3.command exit to shut down the program."<<std::endl;
		}
		else if(Command=="cls")
			system("cls");
		else if(Command=="exit")
			break;
		else
		{
			RunProcess(Command);
			nasal_lexer.run();
		}
	}
	return 0;
}
