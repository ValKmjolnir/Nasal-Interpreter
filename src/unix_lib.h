#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

#ifndef _MSC_VER
#include <unistd.h>
#include <dirent.h>
#else
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#include <io.h>
#include <direct.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/wait.h>
#endif

void dir_entry_destructor(void*);

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

extern nasal_builtin_table unix_lib_native[];