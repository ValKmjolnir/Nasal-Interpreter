#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/wait.h>
#endif

namespace nasal {

void dynamic_library_destructor(void*);

var builtin_dlopen(context*, gc*);
var builtin_dlclose(context*, gc*);
var builtin_dlcallv(context*, gc*);
var builtin_dlcall(context*, gc*);

extern nasal_builtin_table dylib_lib_native[];

}
