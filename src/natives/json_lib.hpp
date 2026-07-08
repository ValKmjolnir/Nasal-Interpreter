#pragma once

#include "nasal.hpp"
#include "nasal_gc.hpp"
#include "natives/builtin.hpp"

namespace nasal {

var builtin_json_new(context*, gc*);
var builtin_json_stringify(context*, gc*);
var builtin_json_parse(context*, gc*);
var builtin_json_get_error(context*, gc*);

extern nasal_builtin_table json_lib_native[];

}