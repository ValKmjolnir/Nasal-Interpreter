#pragma once

#include <regex>

#include "nasal.h"
#include "nasal_gc.h"
#include "natives/builtin.h"

namespace nasal {

var builtin_regex_match(context*, gc*) noexcept;
var builtin_regex_search(context*, gc*) noexcept;
var builtin_regex_replace(context*, gc*) noexcept;
var builtin_regex_match_all(context*, gc*) noexcept;

extern nasal_builtin_table regex_lib_native[];

}