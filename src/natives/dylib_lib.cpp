#include "natives/dylib_lib.h"
#include "util/util.h"
#include "util/fs.h"

#include <cstdlib>
#include <vector>

namespace nasal {

const auto dynamic_library_type_name = "nasal::dynamic_library";
const auto function_address_type_name = "nasal::function_address";

void dynamic_library_destructor(void* pointer) {
#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(pointer));
#else
    dlclose(pointer);
#endif
}

std::vector<std::string> possible_dylib_path() {
    const auto env_path = std::string(getenv("PATH"));
    const auto sep = (util::is_windows()? ";":":");
    const auto path_front = util::is_windows()? "\\module\\":"/module/";

    // do split string
    std::vector<std::string> env_path_vec = {"."};
    usize last = 0;
    usize pos = env_path.find(sep, 0);
    while (pos != std::string::npos) {
        if (pos > last) {
            env_path_vec.push_back(env_path.substr(last, pos - last));
        }
        last = pos + 1;
        pos = env_path.find(sep, last);
    }
    if (last != env_path.length()) {
        env_path_vec.push_back(env_path.substr(last));
    }

    for (auto& p : env_path_vec) {
        p += path_front;
    }

    return env_path_vec;
}

std::string search_dynamic_library_path(const std::string& dlname) {
    const auto ext = (util::is_windows()? ".dll":".so");
    const auto lib_path = (util::is_windows()? ".\\":"./") + dlname + ext;
    if (fs::exists(lib_path)) {
        return lib_path;
    }
    // macos may use .dylib as extension
    if (util::is_macos()) {
        const auto dylib_path = "./" + dlname + ".dylib";
        if (fs::exists(dylib_path)) {
            return dylib_path;
        }
    }

    // search library in PATH
    const auto possible_path = possible_dylib_path();
    for (const auto& p : possible_path) {
        const auto env_p = p + lib_path;
        if (fs::exists(env_p)) {
            return env_p;
        }
    }

    // macos may use .dylib as extension
    if (util::is_macos()) {
        const auto dylib_path = "./" + dlname + ".dylib";
        for (const auto& p : possible_path) {
            const auto env_p = p + dylib_path;
            if (fs::exists(env_p)) {
                return env_p;
            }
        }
    }
    return "";
}

var builtin_dlopen(context* ctx, gc* ngc) {
    auto dl = ctx->localr[1];
    if (!dl.is_str()) {
        return nas_err("dylib::dlopen", "\"libname\" must be string");
    }

    const auto dlname = search_dynamic_library_path(dl.str());
    if (dlname.empty()) {
        return nas_err("dylib::dlopen",
            "cannot find dynamic library <" + dl.str() + ">"
        );
    }

    // get library pointer
#ifdef _WIN32
    wchar_t* wide_string = new wchar_t[dlname.size()+1];
    if (!wide_string) {
        return nas_err("dylib::dlopen", "malloc failed");
    }
    memset(wide_string, 0, sizeof(wchar_t) * dlname.size() + 1);
    mbstowcs(wide_string, dlname.c_str(), dlname.size() + 1);
    // load library by using wide string name
    void* dynamic_library_pointer = LoadLibraryA(dlname.c_str());
    delete []wide_string;
#else
    void* dynamic_library_pointer = dlopen(
        dlname.c_str(), RTLD_LOCAL|RTLD_LAZY
    );
#endif
    
    // check library pointer and insert into returned hashmap
    if (!dynamic_library_pointer) {
        return nas_err("dylib::dlopen",
            "cannot open dynamic lib <" + dl.str() + ">"
        );
    }
    auto return_hash = ngc->temp = ngc->alloc(vm_type::vm_hash);
    auto library_object = ngc->alloc(vm_type::vm_ghost);
    library_object.ghost().set(
        dynamic_library_type_name,
        dynamic_library_destructor,
        nullptr,
        dynamic_library_pointer
    );
    return_hash.hash().elems["lib"] = library_object;

    // get "get" function, to get the register table
#ifdef _WIN32
    void* register_table_get_function = reinterpret_cast<void*>(GetProcAddress(
        library_object.ghost().convert<HMODULE>(), "get"
    ));
#else
    void* register_table_get_function = dlsym(
        library_object.ghost().pointer, "get"
    );
#endif
    if (!register_table_get_function) {
        return nas_err("dylib::dlopen", "cannot find <get> function");
    }

    // get function pointer by name
    auto table = reinterpret_cast<get_func_ptr>(register_table_get_function)();
    if (!table) {
        return nas_err("dylib::dlopen", "failed to get module functions");
    }
    for (u32 i = 0; table[i].name; ++i) {
        auto function_pointer = reinterpret_cast<void*>(table[i].fd);
        auto function_object = ngc->alloc(vm_type::vm_ghost);
        function_object.ghost().set(
            function_address_type_name,
            nullptr,
            nullptr,
            function_pointer
        );
        return_hash.hash().elems[table[i].name] = function_object;
    }

    ngc->temp = nil;
    return return_hash;
}

var builtin_dlclose(context* ctx, gc* ngc) {
    auto library_pointer = ctx->localr[1];
    if (!library_pointer.object_check(dynamic_library_type_name)) {
        return nas_err("dylib::dlclose", "\"lib\" is not a valid dynamic lib");
    }
    library_pointer.ghost().clear();
    return nil;
}

var builtin_dlcallv(context* ctx, gc* ngc) {
    auto function_object = ctx->localr[1];
    auto arguments = ctx->localr[2];
    if (!function_object.object_check(function_address_type_name)) {
        return nas_err("dylib::dlcall",
            "\"ptr\" is not a valid function pointer"
        );
    }
    auto& vec = arguments.vec().elems;
    return function_object.ghost().convert<module_func>()(
        vec.data(),
        vec.size(),
        ngc
    );
}

var builtin_dlcall(context* ctx, gc* ngc) {
    auto function_object = ctx->localr[1];
    if (!function_object.object_check(function_address_type_name)) {
        return nas_err("dylib::dlcall",
            "\"ptr\" is not a valid function pointer"
        );
    }

    // function pointer is at ctx->localr[1]
    // so arguments starts from ctx->localr[2]
    var* local_frame_start = ctx->localr + 2;
    usize local_frame_size = ngc->running_context->top - local_frame_start;
    return function_object.ghost().convert<module_func>()(
        local_frame_start,
        local_frame_size,
        ngc
    );
}

nasal_builtin_table dylib_lib_native[] = {
    {"__dlopen", builtin_dlopen},
    {"__dlclose", builtin_dlclose},
    {"__dlcallv", builtin_dlcallv},
    {"__dlcall", builtin_dlcall},
    {nullptr, nullptr}
};

}
