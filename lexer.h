#ifndef __LEXER_H__
#define __LEXER_H__


#include <iostream>
#include <fstream>
#include <cstring>
#include "nasal.h"
#include "process_stack.h"

using namespace nasal;

std::string text;
process_stack _test;
process_stack_unit _unit;

void PrintProcess(std::string content)
{
	std::string Sentence="";
	int len=(int)content.length();
	for(int i=0;i<len;++i)
	{
		if(content[i]==',')
			Sentence="";
		else if(content[i]=='\"')//string mode
		{
			Sentence="";
			for(int j=i+1;j<len;++j)
			{
				if(content[j]=='\\' && j+1<len)
				{
					Sentence+=content[j];
					Sentence+=content[j+1];
					++j;
				}
				else if(content[j]=='\"')
				{
					i=j;
					break;
				}
				else
					Sentence+=content[j];
			}
			PrintString(Sentence);
		}
		else if(content[i]!=' ' && content[i]!='\"' && content[i]!=',')//check var
		{
			Sentence="";
			for(int j=i;j<len;++j)
			{
				if(content[j]!=' ' && content[j]!='\"' && content[j]!=',')
					Sentence+=content[j];
				if(content[j]==',' || content[j]==' ' || j==len-1)
				{
					i=j;
					break;
				}
			}
			if(_test.check_stack(Sentence))
			{
				_test.stack_content_print(Sentence);
			}
			else
				std::cout<<std::endl<<"[Error] "<<Sentence<<" is not declared in this scope."<<std::endl;
		}
	}
	std::cout<<std::endl;
	return;
}

void FileProcess(const char *FileName)
{
	std::ifstream fin(FileName);
	
	fin.close();
	return;
}

void CommandProcess()
{
	while(1)
	{
		std::cout<<">> ";
		std::getline(std::cin,text);
		int len=(int)text.length();
		
		int sharpDetected=0;
		for(int i=len-1;i>=0;--i)
		{
			if(text[i]=='#')
			{
				//ignore sharp
				len=i;
				sharpDetected=i;
			}
			if(text[i]==';')
			{
				len=i+1;
				//find the real end of the sentence
				if(sharpDetected)
				{
					for(int j=sharpDetected-1;j>=len;--j)
						if(text[j]!=' ')
						{
							len=j+1;
							break;
						}
				}
				else
				{
					for(int j=(int)text.length()-1;j>=len;--j)
						if(text[j]!=' ')
						{
							len=j+1;
							break;
						}
				}
				break;
			}
		}
		
		if(text[len-1]==';')
		{
			for(int i=0;i<len;++i)
			{
				if(text[i]=='p' && i+1<len && text[i+1]=='r' && i+2<len && text[i+2]=='i' && i+3<len && text[i+3]=='n' && i+4<len && text[i+4]=='t')
				{
					//In this part every error leads to an error information and breaks the loop!
					
					//check the first char after print is '(' or not
					int string_beg=len-1;
					for(int j=i+5;j<len;++j)
						if(text[j]!=' ')
						{
							string_beg=j;
							break;
						}
						
					if(text[string_beg]!='(')
					{
						std::cout<<std::endl<<"[Error] Missing \'(\' ."<<std::endl;
						break;
					}
					
					//check the ')' and the place 
					int string_end=len-1;
					for(int j=len-2;j>=0;--j)
						if(text[j]!=' ')
						{
							string_end=j;
							break;
						}
					if(text[string_end]!=')')
					{
						std::cout<<std::endl<<"[Error] Missing \')\' ."<<std::endl;
						break;
					}
					
					std::string content="";
					for(int j=string_beg+1;j<string_end;++j)
						content+=text[j];
					
					std::cout<<std::endl<<"Target string: "<<content<<std::endl;
					PrintProcess(content);
					break;
				}
				else if(text[i]=='v' && i+1<len && text[i+1]=='a' && i+2<len && text[i+2]=='r')
				{
					;
				}
				else if(i==len-1)
					std::cout<<std::endl<<"[Error] Incorrect command."<<std::endl;
			}
		}
		else
		{
			std::cout<<std::endl<<"[Error] Expected \';\' after this line."<<std::endl;
		}
	}
	return;
}

#endif
