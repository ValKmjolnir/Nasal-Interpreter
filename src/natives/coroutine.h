#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

namespace nasal {

var builtin_cocreate(context*, gc*);
var builtin_coresume(context*, gc*);
var builtin_coyield(context*, gc*);
var builtin_costatus(context*, gc*);
var builtin_corun(context*, gc*);

extern nasal_builtin_table coroutine_native[];

}
