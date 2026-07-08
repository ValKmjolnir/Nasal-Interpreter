#pragma once

#include <regex>

#include "nasal.hpp"
#include "nasal_gc.hpp"
#include "natives/builtin.hpp"

namespace nasal {

var builtin_regex_match(context*, gc*) noexcept;
var builtin_regex_search(context*, gc*) noexcept;
var builtin_regex_replace(context*, gc*) noexcept;
var builtin_regex_match_all(context*, gc*) noexcept;

extern nasal_builtin_table regex_lib_native[];

}