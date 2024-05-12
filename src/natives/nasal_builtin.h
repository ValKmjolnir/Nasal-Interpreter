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

var builtin_unsafe(context*, gc*);
var builtin_print(context*, gc*);
var builtin_println(context*, gc*);
var builtin_exit(context*, gc*);
var builtin_abort(context*, gc*);
var builtin_append(context*, gc*);
var builtin_setsize(context*, gc*);
var builtin_system(context*, gc*);
var builtin_input(context*, gc*);
var builtin_split(context*, gc*);
var builtin_rand(context*, gc*);
var builtin_id(context*, gc*);
var builtin_int(context*, gc*);
var builtin_floor(context*, gc*);
var builtin_ceil(context*, gc*);
var builtin_num(context*, gc*);
var builtin_pop(context*, gc*);
var builtin_str(context*, gc*);
var builtin_size(context*, gc*);
var builtin_time(context*, gc*);
var builtin_contains(context*, gc*);
var builtin_delete(context*, gc*);
var builtin_keys(context*, gc*);
var builtin_die(context*, gc*);
var builtin_find(context*, gc*);
var builtin_type(context*, gc*);
var builtin_substr(context*, gc*);
var builtin_streq(context*, gc*);
var builtin_left(context*, gc*);
var builtin_right(context*, gc*);
var builtin_cmp(context*, gc*);
var builtin_chr(context*, gc*);
var builtin_char(context*, gc*);
var builtin_values(context*, gc*);
var builtin_sleep(context*, gc*);
var builtin_platform(context*, gc*);
var builtin_arch(context*, gc*);

// md5 related functions
std::string tohex(u32);
std::string md5(const std::string&);
var builtin_md5(context*, gc*);

var builtin_maketimestamp(context*, gc*);
var builtin_time_stamp(context*, gc*);
var builtin_elapsed_millisecond(context*, gc*);
var builtin_elapsed_microsecond(context*, gc*);
var builtin_gcextend(context*, gc*);
var builtin_gcinfo(context*, gc*);
var builtin_logtime(context*, gc*);
var builtin_ghosttype(context*, gc*);

// register builtin function's name and it's address here in this table below
// this table must end with {nullptr,nullptr}
struct nasal_builtin_table {
    const char* name;
    var (*func)(context*, gc*);
};

extern nasal_builtin_table builtin[];

}
