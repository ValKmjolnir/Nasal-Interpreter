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

#ifdef __linux__
#define PRTHEX64   "%lx"
#define PRTHEX64_8 "%.8lx"
#define PRTINT64   "%ld"
#else
#define PRTHEX64   "%llx"
#define PRTHEX64_8 "%.8llx"
#define PRTINT64   "%lld"
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

int utf8_hdchk(char head)
{
    // RFC-2279 but in fact now we use RFC-3629 so nbytes is less than 4
    uint8_t c=(uint8_t)head;
    uint32_t nbytes=0;
    if((c>>5)==0x06) // 110x xxxx (10xx xxxx)^1
        nbytes=1;
    if((c>>4)==0x0e) // 1110 xxxx (10xx xxxx)^2
        nbytes=2;
    if((c>>3)==0x1e) // 1111 0xxx (10xx xxxx)^3
        nbytes=3;
    // these should not be true
    if((c>>2)==0x3e) // 1111 10xx (10xx xxxx)^4
        nbytes=4;
    if((c>>1)==0x7e) // 1111 110x (10xx xxxx)^5
        nbytes=5;
    if(c==0xfe)      // 1111 1110 (10xx xxxx)^6
        nbytes=6;
    return nbytes;
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
            case '\0': ret+="\\0"; break;
            case '\a': ret+="\\a"; break;
            case '\b': ret+="\\b"; break;
            case '\e': ret+="\\e"; break;
            case '\t': ret+="\\t"; break;
            case '\n': ret+="\\n"; break;
            case '\v': ret+="\\v"; break;
            case '\f': ret+="\\f"; break;
            case '\r': ret+="\\r"; break;
            case '\\': ret+="\\\\";break;
            case '\'': ret+="\\\'";break;
            case '\"': ret+="\\\"";break;
            default:   ret+=i;     break;
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
