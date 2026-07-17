#pragma once

#include "vm/type.hpp"
#include "vm/gc.hpp"

#include <string>
#include <vector>
#include <unordered_set>

namespace nasal {

typedef var (*builtin_func_ptr)(context*, gc*);

// regist builtin function's name and address here in this structure
struct nasal_builtin_info {
    const char* name;
    builtin_func_ptr func;
};

struct nasal_builtin_registry {
    std::vector<nasal_builtin_info> builtin_table;
    std::unordered_set<std::string> registered_builtin;

    static nasal_builtin_registry& get();
    void regist(nasal_builtin_info nbi) {
        if (registered_builtin.count(std::string(nbi.name))) {
            return;
        }
        builtin_table.push_back(nbi);
        registered_builtin.insert(std::string(nbi.name));
    }
};

void load_standard_builtin();
void load_io_builtin();
void load_math_builtin();
void load_bits_builtin();
void load_coroutine_builtin();
void load_flightgear_builtin();
void load_dylib_builtin();
void load_unix_builtin();
void load_json_builtin();
void load_regex_builtin();
void load_subprocess_builtin();

nasal_builtin_info unsafe_builtin_info();

}
