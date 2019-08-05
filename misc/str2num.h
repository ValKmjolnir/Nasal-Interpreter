#ifndef __STR2NUM_H__
#define __STR2NUM_H__

#include <iostream>
#include <cstring>

namespace nasal
{

void Str2Num(std::string &str)
{
	for(int i=0;i<(int)str.length();++i)
		if(!(('0'<=str[i]) && (str[i]<='9') || (str[i]=='.')))
		{
			std::cout<<"[Error] Non-numeric string."<<std::endl;
			return;
		}
	
	bool isFloat=false;
	int DotPlace=0;
	for(int i=0;i<(int)str.length();++i)
		if(str[i]=='.')
		{
			isFloat=true;
			DotPlace=i;
			break;
		}
	if(!isFloat)
	{
		long long int num=0;
		long long int acc=1;
		for(int i=(int)str.length()-1;i>=0;--i)
		{
			num+=acc*((long long int)(str[i]-'0'));
			acc*=10;
		}
		std::cout<<num<<std::endl;
	}
	else
	{
		double num=0;
		double acc=1;
		double aff=0.1;
		for(int i=DotPlace+1;i<(int)str.length();++i)
		{
			num+=aff*((double)(str[i]-'0'));
			aff*=0.1;
		}
		for(int i=DotPlace-1;i>=0;--i)
		{
			num+=acc*((double)(str[i]-'0'));
			acc*=10;
		}
		std::cout<<num<<std::endl;
	}
	return;
}

}
#endif
