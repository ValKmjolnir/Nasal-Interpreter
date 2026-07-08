#pragma once

#include "nasal.hpp"
#include "nasal_gc.hpp"
#include "natives/builtin.hpp"

namespace nasal {

var builtin_cocreate(context*, gc*);
var builtin_coresume(context*, gc*);
var builtin_coyield(context*, gc*);
var builtin_costatus(context*, gc*);
var builtin_corun(context*, gc*);

extern nasal_builtin_table coroutine_native[];

}
