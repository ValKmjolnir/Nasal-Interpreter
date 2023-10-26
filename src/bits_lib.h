#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

namespace nasal {

var builtin_u32xor(context*, gc*);
var builtin_u32and(context*, gc*);
var builtin_u32or(context*, gc*);
var builtin_u32nand(context*, gc*);
var builtin_u32not(context*, gc*);
var builtin_fld(context*, gc*);
var builtin_sfld(context*, gc*);
var builtin_setfld(context*, gc*);
var builtin_buf(context*, gc*);

extern nasal_builtin_table bits_native[];

}
