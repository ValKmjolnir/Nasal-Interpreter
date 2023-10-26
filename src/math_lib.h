#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

namespace nasal {

var builtin_pow(context*, gc*);
var builtin_sin(context*, gc*);
var builtin_cos(context*, gc*);
var builtin_tan(context*, gc*);
var builtin_exp(context*, gc*);
var builtin_lg(context*, gc*);
var builtin_ln(context*, gc*);
var builtin_sqrt(context*, gc*);
var builtin_atan2(context*, gc*);
var builtin_isnan(context*, gc*);

extern nasal_builtin_table math_lib_native[];

}
