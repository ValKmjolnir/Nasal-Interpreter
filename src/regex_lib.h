#pragma once

#include <regex>

#include "nasal.h"
#include "nasal_gc.h"
#include "nasal_builtin.h"

namespace nasal {

var builtin_regex_match(context*, gc*);
var builtin_regex_search(context*, gc*);
var builtin_regex_replace(context*, gc*);
var builtin_regex_match_all(context*, gc*);

extern nasal_builtin_table regex_lib_native[];

}