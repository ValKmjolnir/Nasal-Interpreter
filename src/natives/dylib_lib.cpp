#include "natives/dylib_lib.h"

namespace nasal {

const auto dynamic_library_type_name = "dylib";
const auto function_address_type_name = "faddr";

void dynamic_library_destructor(void* pointer) {
#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(pointer));
#else
    dlclose(pointer);
#endif
}

var builtin_dlopen(context* ctx, gc* ngc) {
    auto dlname = ctx->localr[1];
    if (!dlname.is_str()) {
        return nas_err("dylib::dlopen", "\"libname\" must be string");
    }

    // get library pointer
#ifdef _WIN32
    wchar_t* wide_string = new wchar_t[dlname.str().size()+1];
    if (!wide_string) {
        return nas_err("dylib::dlopen", "malloc failed");
    }
    memset(wide_string, 0, sizeof(wchar_t) * dlname.str().size() + 1);
    mbstowcs(wide_string, dlname.str().c_str(), dlname.str().size() + 1);
    // load library by using wide string name
    void* dynamic_library_pointer = LoadLibraryA(dlname.str().c_str());
    delete []wide_string;
#else
    void* dynamic_library_pointer = dlopen(
        dlname.str().c_str(), RTLD_LOCAL|RTLD_LAZY
    );
#endif
    
    // check library pointer and insert into returned hashmap
    if (!dynamic_library_pointer) {
        return nas_err("dylib::dlopen",
            "cannot open dynamic lib <" + dlname.str() + ">"
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
        static_cast<HMODULE>(library_object.ghost().pointer), "get"
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
    for(u32 i = 0; table[i].name; ++i) {
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
    return reinterpret_cast<module_func>(function_object.ghost().pointer)(
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
    return reinterpret_cast<module_func>(function_object.ghost().pointer)(
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
