#ifndef __NASAL_PRINT_H__
#define __NASAL_PRINT_H__

#include<iostream>
#include<cstring>
#include "nasal_hash.cpp"
#include "nasal_list.cpp"

namespace nasal
{
	
void PrintVar(int Var)
{
	std::cout<<Var;
}
void PrintVar(float Var)
{
	std::cout<<Var;
}
void PrintVar(double Var)
{
	std::cout<<Var;
}
void PrintVar(char Var)
{
	std::cout<<Var;
}
void PrintVar(long long int Var)
{
	std::cout<<Var;
}
void PrintVar(NasalHash &Var)
{
	Var.PrintHash();
}
void PrintVar(NasalList &Var)
{
	Var.PrintList();
}


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

void PrintString(const char *PrintInfo)
{
	for(int i=0;i<strlen(PrintInfo);++i)
	{
		if(PrintInfo[i]=='\\' && i+1<strlen(PrintInfo))
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
					std::cout<<"[Error]: Incorrect escape character \'"<<PrintInfo[i]<<PrintInfo[i+1]<<"\' ."<<std::endl;
					++i;
					break;
			}
		}
		else if(PrintInfo[i]=='\\' && i+1>=strlen(PrintInfo))
		{
			//error occurred
			std::cout<<"[Error]: Missing character after \'\\\'"<<std::endl;
		}
		else
			std::cout<<PrintInfo[i];
	}
	return;
}

}

#endif
