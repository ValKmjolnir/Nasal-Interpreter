#include "code/compilation.hpp"

namespace nasal {

compilation::compilation(bool enable_limited_mode) {
    init_native_function(enable_limited_mode);
}

void compilation::init_native_function(bool limited_mode) {
    load_standard_builtin();
    load_eval_builtin();
    load_io_builtin();
    load_math_builtin();
    load_bits_builtin();
    load_coroutine_builtin();
    load_flightgear_builtin();
    load_dylib_builtin();
    load_unix_builtin();
    load_json_builtin();
    load_regex_builtin();
    load_subprocess_builtin();

    auto& registry = nasal_builtin_registry::get();
    for (const auto& intrinsic : registry.builtin_table) {
        // skip confliction
        if (native_function_mapper.count(intrinsic.name)) {
            continue;
        }

        // replace unsafe native functions with redirect function in limit mode
        if (limited_mode && unsafe_system_api.count(intrinsic.name)) {
            native_function.push_back(unsafe_builtin_info());
        } else {
            native_function.push_back(intrinsic);
        }

        // insert into mapper
        auto index = native_function_mapper.size();
        native_function_mapper[intrinsic.name] = index;
    }
}

void compilation::regist_number(const f64 num) {
    if (const_number_map.count(num)) {
        return;
    }

    auto size = const_number_map.size();
    const_number_map[num] = size;
    const_number_table.push_back(num);
}

void compilation::regist_string(const std::string& str) {
    if (const_string_map.count(str)) {
        return;
    }

    auto size = const_string_map.size();
    const_string_map[str] = size;
    const_string_table.push_back(str);
}

void compilation::regist_global(const std::string& name) {
    if (global.count(name)) {
        return;
    }

    auto size = static_cast<u32>(global.size());
    global[name] = size;
    return;
}

u64 compilation::number_index(const f64 num) const {
    return const_number_map.at(num);
}

u64 compilation::string_index(const std::string& str) const {
    return const_string_map.at(str);
}

bool compilation::has_native(const std::string& name) const {
    return native_function_mapper.count(name);
}

u64 compilation::native_index(const std::string& name) const {
    return native_function_mapper.at(name);
}

}
