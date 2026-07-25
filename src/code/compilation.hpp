#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "util/type_alias.hpp"
#include "code/opcode.hpp"
#include "natives/registry.hpp"

namespace nasal {

class compilation {
private:
    // constant numbers
    std::unordered_map<f64, u64> const_number_map;
    std::vector<f64> const_number_table;

    // constant strings
    std::unordered_map<std::string, u64> const_string_map;
    std::vector<std::string> const_string_table;

    // native functions (intrinsics)
    std::unordered_map<std::string, usize> native_function_mapper;
    std::vector<nasal_builtin_info> native_function;

    // under limited mode, unsafe system api will be banned
    const std::unordered_set<std::string> unsafe_system_api = {
        // builtin
        "__system", "__input", "__terminal_size",
        // io
        "__fout", "__open", "__write", "__stat"
        // bits
        "__fld", "__sfld", "__setfld",
        "__buf",
        // fg
        "__logprint",
        // dylib
        "__dlopen", "__dlclose", "__dlcallv", "__dlcall",
        // unix
        "__chdir", "__environ", "__getcwd", "__getenv",
        // subprocess
        "__subprocess_create",
        "__subprocess_active",
        "__subprocess_terminate"
    };

    // generated opcodes
    std::vector<opcode> code;

    // symbol table
    // global : max VM_STACK_DEPTH - 1 values
    std::unordered_map<std::string, u32> global;

private:
    void init_native_function(bool);

public:
    compilation(bool enable_limited_mode);
    void regist_number(const f64);
    void regist_string(const std::string&);
    void regist_global(const std::string&);
    u64 number_index(const f64) const;
    u64 string_index(const std::string&) const;
    bool has_native(const std::string&) const;
    u64 native_index(const std::string&) const;

public:
    auto& get_code() { return code; }
    const auto& get_code() const { return code; }
    auto code_size() const { return code.size(); }
    const auto& get_number_table() const { return const_number_table; }
    const auto& get_string_table() const { return const_string_table; }
    const auto& get_native_functions() const { return native_function; }
    const auto& get_globals() const { return global; }
};

}
