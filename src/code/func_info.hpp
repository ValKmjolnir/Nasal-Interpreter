#pragma once

#include "util/type_alias.hpp"
#include "util/densemap.hpp"

#include <string>
#include <iostream>
#include <vector>

namespace nasal {

struct func_info {
    i64 dynamic_parameter_index = -1; // dynamic param index in hash.
    u64 entry;                        // pc will set to entry-1 to call this function
    u32 parameter_size;               // used to load default parameters to a new function
    u64 local_size;                   // used to expand memory space for local values on stack

    // parameter table, u32 begins from 1
    util::densemap<std::string, u32> param_index_map;
    std::vector<std::string> param_table;
    std::vector<u8> default_param_flags;

    // dynamic param name
    std::string dynamic_parameter_name;

public:
    void dump(std::ostream&) const;
    void set_entry(u64 e) { entry = e; }
    void set_local_size(u64 s) { local_size = s; }
    void add_param(const std::string& p) {
        param_table.push_back(p);
        // param index starts from 1, 0 is reserved for 'me'
        param_index_map.insert(p, param_table.size());
        default_param_flags.push_back(0);
    }
    void add_default_param(const std::string& p) {
        param_table.push_back(p);
        // param index starts from 1, 0 is reserved for 'me'
        param_index_map.insert(p, param_table.size());
        default_param_flags.push_back(1);
    }
    void add_dynamic_param(const std::string& p) {
        // if called, make sure it is called after all parameters are loaded
        dynamic_parameter_name = p;
        dynamic_parameter_index = param_table.size() + 1;
    }
};

}
