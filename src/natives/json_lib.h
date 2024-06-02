#pragma once

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

namespace nasal {

var builtin_json_new(context*, gc*);
var builtin_json_stringify(context*, gc*);
var builtin_json_parse(context*, gc*);
var builtin_json_get_error(context*, gc*);

extern nasal_builtin_table json_lib_native[];

}