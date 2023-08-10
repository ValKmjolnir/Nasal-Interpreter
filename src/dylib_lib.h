#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/wait.h>
#endif

void dylib_destructor(void*);
void func_addr_destructor(void*);

var builtin_dlopen(var*, gc&);
var builtin_dlclose(var*, gc&);
var builtin_dlcallv(var*, gc&);
var builtin_dlcall(var*, gc&);

extern nasal_builtin_table dylib_lib_native[];