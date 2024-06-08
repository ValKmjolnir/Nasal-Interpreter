#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

namespace nasal {

var builtin_subprocess_run(context*, gc*);

extern nasal_builtin_table subprocess_native[];

}