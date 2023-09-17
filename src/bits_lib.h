#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

namespace nasal {

var builtin_u32xor(var*, gc&);
var builtin_u32and(var*, gc&);
var builtin_u32or(var*, gc&);
var builtin_u32nand(var*, gc&);
var builtin_u32not(var*, gc&);
var builtin_fld(var*, gc&);
var builtin_sfld(var*, gc&);
var builtin_setfld(var*, gc&);
var builtin_buf(var*, gc&);

extern nasal_builtin_table bits_native[];

}
