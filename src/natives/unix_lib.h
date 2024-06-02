#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

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

namespace nasal {

void dir_entry_destructor(void*);

var builtin_pipe(context*, gc*);
var builtin_fork(context*, gc*);
var builtin_waitpid(context*, gc*);
var builtin_opendir(context*, gc*);
var builtin_readdir(context*, gc*);
var builtin_closedir(context*, gc*);
var builtin_chdir(context*, gc*);
var builtin_environ(context*, gc*);
var builtin_getcwd(context*, gc*);
var builtin_getenv(context*, gc*);

extern nasal_builtin_table unix_lib_native[];

}
