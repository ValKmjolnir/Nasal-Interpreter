#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

var builtin_pow(var*, gc&);
var builtin_sin(var*, gc&);
var builtin_cos(var*, gc&);
var builtin_tan(var*, gc&);
var builtin_exp(var*, gc&);
var builtin_lg(var*, gc&);
var builtin_ln(var*, gc&);
var builtin_sqrt(var*, gc&);
var builtin_atan2(var*, gc&);
var builtin_isnan(var*, gc&);

extern nasal_builtin_table math_lib_native[];