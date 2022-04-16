#ifndef __NASAL_H__
#define __NASAL_H__

#define __nasver "9.0"

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
#include <list>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>

#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/wait.h>
#endif

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
        if('0'<=*str && *str<'8')
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

std::string rawstr(const std::string& str)
{
    std::string ret("");
    for(auto i:str)
    {
#ifdef _WIN32
        // windows ps or cmd doesn't output unicode normally
        // if 'chcp65001' is not enabled, so we output the hex
        if(i<=0)
        {
            ret+="\\x";
            ret+="0123456789abcdef"[(i>>4)&15];
            ret+="0123456789abcdef"[i&15];
            continue;
        }
#endif
        switch(i)
        {
            case '\a': ret+="\\a";break;
            case '\b': ret+="\\b";break;
            case '\e': ret+="\\e";break;
            case '\f': ret+="\\f";break;
            case '\n': ret+="\\n";break;
            case '\r': ret+="\\r";break;
            case '\t': ret+="\\t";break;
            case '\v': ret+="\\v";break;
            case '\0': ret+="\\0";break;
            default:   ret+=i;    break;
        }
    }
    return ret;
}
#include "nasal_err.h"
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "nasal_opt.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"
#include "nasal_codegen.h"
#include "nasal_vm.h"
#include "nasal_dbg.h"

#endif
