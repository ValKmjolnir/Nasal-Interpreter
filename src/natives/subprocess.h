#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifndef WIN32
#include <sys/wait.h>
#endif

namespace nasal {

struct subprocess {
#ifndef WIN32
    pid_t pid;
#else
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
#endif

public:
    static const char* name() {
        return "nasal::subprocess";
    }
};

void subprocess_desc_dtor(void*);

var builtin_subprocess_create(context*, gc*);
var builtin_subprocess_terminate(context*, gc*);
extern nasal_builtin_table subprocess_native[];

}