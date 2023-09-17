#include "dylib_lib.h"

namespace nasal {

const auto dylib_type_name = "dylib";
const auto func_addr_type_name = "faddr";

void dylib_destructor(void* ptr) {
#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(ptr));
#else
    dlclose(ptr);
#endif
}

void func_addr_destructor(void* ptr) {}

var builtin_dlopen(var* local, gc& ngc) {
    var dlname = local[1];
    if (dlname.type!=vm_str) {
        return nas_err("dlopen", "\"libname\" must be string");
    }
#ifdef _WIN32
    wchar_t* str = new wchar_t[dlname.str().size()+1];
    if (!str) {
        return nas_err("dlopen", "malloc failed");
    }
    memset(str, 0, sizeof(wchar_t)*dlname.str().size()+1);
    mbstowcs(str, dlname.str().c_str(),dlname.str().size()+1);
    void* ptr = LoadLibraryA(dlname.str().c_str());
    delete []str;
#else
    void* ptr = dlopen(dlname.str().c_str(), RTLD_LOCAL|RTLD_LAZY);
#endif
    if (!ptr) {
        return nas_err("dlopen", "cannot open dynamic lib <"+dlname.str()+">");
    }
    var ret = ngc.temp = ngc.alloc(vm_hash);
    var lib = ngc.alloc(vm_obj);
    lib.obj().set(dylib_type_name, dylib_destructor, ptr);
    ret.hash().elems["lib"] = lib;

#ifdef _WIN32
    void* func = (void*)GetProcAddress(
        static_cast<HMODULE>(lib.obj().ptr),
        "get"
    );
#else
    void* func = dlsym(lib.obj().ptr, "get");
#endif
    if (!func) {
        return nas_err("dlopen", "cannot find <get> function");
    }
    // get function pointer by name
    module_func_info* tbl = reinterpret_cast<get_func_ptr>(func)();
    if (!tbl) {
        return nas_err("dlopen", "failed to get module functions");
    }
    for(u32 i = 0; tbl[i].name; ++i) {
        void* p = (void*)tbl[i].fd;
        var tmp = ngc.alloc(vm_obj);
        tmp.obj().set(func_addr_type_name, func_addr_destructor, p);
        ret.hash().elems[tbl[i].name] = tmp;
    }

    ngc.temp = nil;
    return ret;
}

var builtin_dlclose(var* local, gc& ngc) {
    var libptr = local[1];
    if (!libptr.objchk(dylib_type_name)) {
        return nas_err("dlclose", "\"lib\" is not a valid dynamic lib");
    }
    libptr.obj().clear();
    return nil;
}

var builtin_dlcallv(var* local, gc& ngc) {
    var fp = local[1];
    var args = local[2];
    if (!fp.objchk(func_addr_type_name)) {
        return nas_err("dlcall", "\"ptr\" is not a valid function pointer");
    }
    auto& vec = args.vec().elems;
    return reinterpret_cast<module_func>(fp.obj().ptr)(
        vec.data(),
        vec.size(),
        &ngc
    );
}

var builtin_dlcall(var* local, gc& ngc) {
    var fp = local[1];
    if (!fp.objchk(func_addr_type_name)) {
        return nas_err("dlcall", "\"ptr\" is not a valid function pointer");
    }

    var* local_frame_start = local+2;
    usize local_frame_size = ngc.rctx->top-local_frame_start;
    // arguments' stored place begins at local +2
    return reinterpret_cast<module_func>(fp.obj().ptr)(
        local_frame_start,
        local_frame_size,
        &ngc
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
