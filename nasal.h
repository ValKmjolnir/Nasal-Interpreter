#ifndef __NASAL_H__
#define __NASAL_H__

#pragma GCC optimize(2)

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <list>
#include <stack>
#include <queue>
#include <vector>
#include <map>

/*
	check if a string can be converted to a number
	if this string cannot be converted to a number,it will return nan
*/
inline double hex_to_double(std::string str)
{
	double ret=0;
	for(int i=2;str[i];++i)
	{
		ret*=16;
		if('0'<=str[i] && str[i]<='9')
			ret+=(str[i]-'0');
		else if('a'<=str[i] && str[i]<='f')
			ret+=(str[i]-'a'+10);
		else if('A'<=str[i] && str[i]<='F')
			ret+=(str[i]-'A'+10);
		else
			return std::nan("");
	}
	return ret;
}
inline double oct_to_double(std::string str)
{
	double ret=0;
	for(int i=2;str[i];++i)
	{
		ret*=8;
		if('0'<=str[i] && str[i]<='8')
			ret+=(str[i]-'0');
		else
			return std::nan("");
	}
	return ret;
}
inline double dec_to_double(std::string str,int len)
{
	double ret=0;
	int i=0;
	while('0'<=str[i] && str[i]<='9' && i<len)
		ret=ret*10+(str[i++]-'0');
	if(i==len) return ret;
	if(str[i]!='.' && str[i]!='e' && str[i]!='E')
		return std::nan("");
	if(str[i]=='.')
	{
		++i;
		if(i==len) return std::nan("");
		double num_pow=0.1;
		while('0'<=str[i] && str[i]<='9' && i<len)
		{
			ret+=num_pow*(str[i++]-'0');
			num_pow*=0.1;
		}
	}
	if(i==len) return ret;
	if(str[i]!='e' && str[i]!='E')
		return std::nan("");
	++i;
	if(i==len) return std::nan("");
	double negative=(str[i]=='-'? -1:1);
	if(str[i]=='-' || str[i]=='+')
		++i;
	if(i==len) return std::nan("");
	double num_pow=0;
	for(;i<len;++i)
	{
		if('0'<=str[i] && str[i]<='9')
			num_pow=num_pow*10+(str[i]-'0');
		else
			return std::nan("");
	}
	return ret*std::pow(10,negative*num_pow);
}
double trans_string_to_number(std::string str)
{
	double is_negative=1;
	int len=str.length();
	double ret_num=0;
	if(!len) return std::nan("");
	if(str[0]=='-' || str[0]=='+')
	{
		if(len==1) return std::nan("");
		is_negative=(str[0]=='-'?-1:1);
		str=str.substr(1,len--);
	}
	if(len>1 && str[0]=='0' && str[1]=='x')
		ret_num=hex_to_double(str);
	else if(len>1 && str[0]=='0' && str[1]=='o')
		ret_num=oct_to_double(str);
	else
		ret_num=dec_to_double(str,len);
	return is_negative*ret_num;
}

/*
	trans_number_to_string:
	convert number to string
*/
std::string trans_number_to_string(double number)
{
	std::string res;
	std::stringstream ss;
	ss<<number;
	ss>>res;
	return res;
}
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"
#include "nasal_codegen.h"
#include "nasal_bytecode_vm.h"

#endif
