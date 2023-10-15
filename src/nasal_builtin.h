#pragma once

#include "nasal.h"
#include "nasal_type.h"
#include "nasal_gc.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable:4566) // i know i'm using utf-8, fuck you
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4996)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif

#include <sstream>
#include <cmath>
#include <thread>

// for environ
#if defined __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

namespace nasal {

var builtin_print(var*, gc&);
var builtin_println(var*, gc&);
var builtin_exit(var*, gc&);
var builtin_abort(var*, gc&);
var builtin_append(var*, gc&);
var builtin_setsize(var*, gc&);
var builtin_system(var*, gc&);
var builtin_input(var*, gc&);
var builtin_split(var*, gc&);
var builtin_rand(var*, gc&);
var builtin_id(var*, gc&);
var builtin_int(var*, gc&);
var builtin_floor(var*, gc&);
var builtin_num(var*, gc&);
var builtin_pop(var*, gc&);
var builtin_str(var*, gc&);
var builtin_size(var*, gc&);
var builtin_time(var*, gc&);
var builtin_contains(var*, gc&);
var builtin_delete(var*, gc&);
var builtin_keys(var*, gc&);
var builtin_die(var*, gc&);
var builtin_find(var*, gc&);
var builtin_type(var*, gc&);
var builtin_substr(var*, gc&);
var builtin_streq(var*, gc&);
var builtin_left(var*, gc&);
var builtin_right(var*, gc&);
var builtin_cmp(var*, gc&);
var builtin_chr(var*, gc&);
var builtin_char(var*, gc&);
var builtin_values(var*, gc&);
var builtin_sleep(var*, gc&);
var builtin_platform(var*, gc&);
var builtin_arch(var*, gc&);
// md5 related functions
std::string tohex(u32);
std::string md5(const std::string&);
var builtin_md5(var*, gc&);
var builtin_millisec(var*, gc&);
var builtin_gcextend(var*, gc&);
var builtin_gcinfo(var*, gc&);
var builtin_logtime(var*, gc&);
var builtin_ghosttype(var*, gc&);

// register builtin function's name and it's address here in this table below
// this table must end with {nullptr,nullptr}
struct nasal_builtin_table {
    const char* name;
    var (*func)(var*, gc&);
};

extern nasal_builtin_table builtin[];

}
