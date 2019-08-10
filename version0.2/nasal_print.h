#ifndef __NASAL_PRINT_H__
#define __NASAL_PRINT_H__

#include<iostream>
#include<cstring>
#include "nasal_hash.cpp"
#include "nasal_list.cpp"

void PrintString(std::string &PrintInfo)
{
	for(int i=0;i<(int)PrintInfo.length();++i)
	{
		if(PrintInfo[i]=='\\' && i+1<(int)PrintInfo.length())
		{
			switch(PrintInfo[i+1])
			{
				case 'n':
					std::cout<<"\n";
					++i;
					break;
				case 't':
					std::cout<<"\t";
					++i;
					break;
				case 'r':
					std::cout<<"\r";
					++i;
					break;
				case '\\':
					std::cout<<"\\";
					++i;
					break;
				case '\'':
					std::cout<<"\'";
					++i;
					break;
				case '\"':
					std::cout<<"\"";
					++i;
					break;
				default:
					//error occurred
					std::cout<<"[Error]: Incorrect escape character \'"<<PrintInfo[i]<<PrintInfo[i+1]<<"\' .";
					++i;
					break;
			}
		}
		else if(PrintInfo[i]=='\\' && i+1>=(int)PrintInfo.length())
		{
			//error occurred
			std::cout<<"[Error]: Missing character after \'\\\'";
		}
		else
			std::cout<<PrintInfo[i];
	}
	return;
}
void PrintVar(var Var)
{
	if(Var.type==VAR_LLINT)
		std::cout<<*((long long int *)Var.data);
	else if(Var.type==VAR_DOUBLE)
		std::cout<<*((double *)Var.data);
	else if(Var.type==VAR_CHAR)
		std::cout<<*((char *)Var.data);
	else if(Var.type==VAR_STRING)
		PrintString(*((std::string *)Var.data));
//	else if(Var.type==VAR_LIST)
//		((NasalList *)Var.data)->PrintList();
//	else if(Var.type==VAR_HASH)
//		((NasalHash *)Var.data)->PrintHash();
	else
		std::cout<<"[Error] Null type or function";
}

#endif
