#include <iostream>
#include <cstring>
#include <cstdlib>
#include "nasal.h"

int main()
{
	nasal::nasal_list m;
	nasal::nasal_hash n;
	nasal::var k;
	k.type=VAR_STRING;
	k.data=new std::string;
	*((std::string *)k.data)="hello";
	m.append(k);
	m.pop();
	
	std::string command;
	std::cout<<">> input \"help\" to find help."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
		{
			std::cout<<">> 1.input file name to run the nasal script."<<std::endl;
			std::cout<<">> 2.command cls to clear the screen."<<std::endl;
			std::cout<<">> 3.command exit to shut down the program."<<std::endl;
		}
		else if(command=="cls")
		{
			system("cls");
			//linux system("clear");
			//macOS system(clear");
		}
		else if(command=="exit")
			break;
		else
		{
			;
		}
	}
	return 0;
}
