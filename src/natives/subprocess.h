#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

namespace nasal {

void subprocess_destructor(void*);

var builtin_subprocess_popen(context*, gc*);
var builtin_subprocess_pclose(context*, gc*);
var builtin_subprocess_read_stdout(context*, gc*);
extern nasal_builtin_table subprocess_native[];

}