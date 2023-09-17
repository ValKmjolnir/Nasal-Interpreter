#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

namespace nasal {

var builtin_cocreate(var*, gc&);
var builtin_coresume(var*, gc&);
var builtin_coyield(var*, gc&);
var builtin_costatus(var*, gc&);
var builtin_corun(var*, gc&);

extern nasal_builtin_table coroutine_native[];

}
