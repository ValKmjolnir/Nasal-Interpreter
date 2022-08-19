#ifndef __NASAL_H__
#define __NASAL_H__
#define __nasver "10.1"

#ifndef _MSC_VER
#include <unistd.h>
#include <dirent.h>
#else
#include <io.h>
#include <direct.h>
#endif

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
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
#include <thread>
#include <chrono>

#include <sys/stat.h>
#include <fcntl.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/wait.h>
#endif

using i32=std::int32_t;
using i64=std::int64_t;
using u8=std::uint8_t;
using u16=std::uint16_t;
using u32=std::uint32_t;
using u64=std::uint64_t;
using usize=std::size_t;
using f64=double;
using std::string;

const u32 STACK_DEPTH=1024;

inline f64 hex2f(const char* str)
{
    f64 ret=0;
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
inline f64 oct2f(const char* str)
{
    f64 ret=0;
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
// we have the same reason not using atof here just as andy's interpreter does.
// it is not platform independent, and may have strange output.
// so we write a new function here to convert str to number manually.
inline f64 dec2f(const char* str)
{
    f64 ret=0,negative=1,num_pow=0;
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
f64 str2num(const char* str)
{
    bool negative=false;
    f64 res=0;
    if(*str=='-' || *str=='+')
        negative=(*str++=='-');
    if(!*str)
        return nan("");
    if(str[0]=='0' && str[1]=='x')
        res=hex2f(str+2);
    else if(str[0]=='0' && str[1]=='o')
        res=oct2f(str+2);
    else
        res=dec2f(str);
    return negative?-res:res;
}

i32 utf8_hdchk(const char head)
{
    // RFC-2279 but now we use RFC-3629 so nbytes is less than 4
    const u8 c=(u8)head;
    if((c>>5)==0x06) // 110x xxxx (10xx xxxx)^1
        return 1;
    if((c>>4)==0x0e) // 1110 xxxx (10xx xxxx)^2
        return 2;
    if((c>>3)==0x1e) // 1111 0xxx (10xx xxxx)^3
        return 3;
    return 0;
}

string chrhex(const char c)
{
    const char hextbl[]="0123456789abcdef";
    return {hextbl[(c&0xf0)>>4],hextbl[c&0x0f]};
}

string rawstr(const string& str,const usize maxlen=0)
{
    string ret("");
    for(auto i:str)
    {
#ifdef _WIN32
        // windows ps or cmd doesn't output unicode normally
        // if 'chcp65001' is not enabled, we output the hex
        if(i<=0)
        {
            ret+="\\x"+chrhex(i);
            continue;
        }
#endif
        switch(i)
        {
            case '\0': ret+="\\0"; break;
            case '\a': ret+="\\a"; break;
            case '\b': ret+="\\b"; break;
            case '\t': ret+="\\t"; break;
            case '\n': ret+="\\n"; break;
            case '\v': ret+="\\v"; break;
            case '\f': ret+="\\f"; break;
            case '\r': ret+="\\r"; break;
#ifdef _MSC_VER
            case '\033':ret+="\\e";break;
#else
            case '\e': ret+="\\e"; break;
#endif
            case '\"': ret+="\\\"";break;
            case '\'': ret+="\\\'";break;
            case '\\': ret+="\\\\";break;
            default:   ret+=i;     break;
        }
    }
    if(maxlen && ret.length()>maxlen)
        ret=ret.substr(0,maxlen)+"...";
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
