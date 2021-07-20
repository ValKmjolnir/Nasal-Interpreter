#ifndef __NASAL_H__
#define __NASAL_H__

#pragma GCC optimize(2)

#include <stdint.h>
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
#include <unordered_map>

/*
	check if a string can be converted to a number
	if this string cannot be converted to a number,it will return nan
*/
inline double hex_to_double(const char* str)
{
	double ret=0;
	for(;*str;++str)
	{
		ret*=16;
		if('0'<=*str && *str<='9')
			ret+=(*str-'0');
		else if('a'<=*str && *str<='f')
			ret+=(*str-'a'+10);
		else if('A'<=*str && *str<='F')
			ret+=(*str-'A'+10);
		else
			return nan("");
	}
	return ret;
}
inline double oct_to_double(const char* str)
{
	double ret=0;
	for(;*str;++str)
	{
		ret*=8;
		if('0'<=*str && *str<='8')
			ret+=(*str-'0');
		else
			return nan("");
	}
	return ret;
}
inline double dec_to_double(const char* str)
{
	double ret=0,negative=1,num_pow=0;
	while('0'<=*str && *str<='9')
		ret=ret*10+(*str++-'0');
	if(!*str) return ret;
	if(*str=='.')
	{
		if(!*++str) return nan("");
		num_pow=0.1;
		while('0'<=*str && *str<='9')
		{
			ret+=num_pow*(*str++-'0');
			num_pow*=0.1;
		}
		if(!*str) return ret;
	}
	if(*str!='e' && *str!='E')
		return nan("");
	if(!*++str) return nan("");
	if(*str=='-' || *str=='+')
		negative=(*str++=='-'? -1:1);
	if(!*str) return nan("");
	num_pow=0;
	for(;*str;++str)
	{
		if('0'<=*str && *str<='9')
			num_pow=num_pow*10+(*str-'0');
		else
			return nan("");
	}
	return ret*std::pow(10,negative*num_pow);
}
double str2num(const char* str)
{
	bool is_negative=false;
	double ret_num=0;
	if(*str=='-' || *str=='+')
		is_negative=(*str++=='-');
	if(!*str)
		return nan("");
	if(str[0]=='0' && str[1]=='x')
		ret_num=hex_to_double(str+2);
	else if(str[0]=='0' && str[1]=='o')
		ret_num=oct_to_double(str+2);
	else
		ret_num=dec_to_double(str);
	return is_negative?-ret_num:ret_num;
}

/*
	trans_number_to_string:
	convert number to string
*/
std::string num2str(double number)
{
	std::ostringstream ss;
	ss<<number;
	return ss.str();
}

/*
	show raw string
*/
void raw_string(std::string& str)
{
	for(auto i:str)
		switch(i)
		{
			case '\a': std::cout<<"\\a";break;
			case '\b': std::cout<<"\\b";break;
			case '\f': std::cout<<"\\f";break;
			case '\n': std::cout<<"\\n";break;
			case '\r': std::cout<<"\\r";break;
			case '\t': std::cout<<"\\t";break;
			case '\v': std::cout<<"\\v";break;
			case '\0': std::cout<<"\\0";break;
			default:   std::cout<<i;    break;
		}
	return;
}
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"
#include "nasal_codegen.h"
#include "nasal_vm.h"

#endif
