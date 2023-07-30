#pragma once

#include "nasal.h"
#include "nasal_gc.h"

#ifndef _MSC_VER
#include <unistd.h>
#include <dirent.h>
#else
#define F_OK 0 // fuck msc
#pragma warning (disable:4566) // i know i'm using utf-8, fuck you
#pragma warning (disable:4244)
#pragma warning (disable:4267)
#pragma warning (disable:4996)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#include <io.h>
#include <direct.h>
#endif

#include <sstream>
#include <cmath>
#include <thread>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/wait.h>
#endif

#if defined __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

var builtin_print(var*, gc&);
var builtin_println(var*, gc&);
var builtin_exit(var*, gc&);
var builtin_abort(var*, gc&);
var builtin_append(var*, gc&);
var builtin_setsize(var*, gc&);
var builtin_system(var*, gc&);
var builtin_input(var*, gc&);
var builtin_readfile(var*, gc&);
var builtin_fout(var*, gc&);
var builtin_split(var*, gc&);
var builtin_rand(var*, gc&);
var builtin_id(var*, gc&);
var builtin_int(var*, gc&);
var builtin_floor(var*, gc&);
var builtin_num(var*, gc&);
var builtin_pop(var*, gc&);
var builtin_str(var*, gc&);
var builtin_size(var*, gc&);
var builtin_u32xor(var*, gc&);
var builtin_u32and(var*, gc&);
var builtin_u32or(var*, gc&);
var builtin_u32nand(var*, gc&);
var builtin_u32not(var*, gc&);
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
var builtin_exists(var*, gc&);
var builtin_open(var*, gc&);
var builtin_close(var*, gc&);
var builtin_read(var*, gc&);
var builtin_write(var*, gc&);
var builtin_seek(var*, gc&);
var builtin_tell(var*, gc&);
var builtin_readln(var*, gc&);
var builtin_stat(var*, gc&);
var builtin_eof(var*, gc&);
var builtin_fld(var*, gc&);
var builtin_sfld(var*, gc&);
var builtin_setfld(var*, gc&);
var builtin_buf(var*, gc&);
var builtin_sleep(var*, gc&);
var builtin_pipe(var*, gc&);
var builtin_fork(var*, gc&);
var builtin_waitpid(var*, gc&);
var builtin_opendir(var*, gc&);
var builtin_readdir(var*, gc&);
var builtin_closedir(var*, gc&);
var builtin_chdir(var*, gc&);
var builtin_environ(var*, gc&);
var builtin_getcwd(var*, gc&);
var builtin_getenv(var*, gc&);
var builtin_dlopen(var*, gc&);
var builtin_dlclose(var*, gc&);
var builtin_dlcallv(var*, gc&);
var builtin_dlcall(var*, gc&);
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
