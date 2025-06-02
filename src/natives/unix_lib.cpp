#include "natives/unix_lib.h"

namespace nasal {

const auto dir_type_name = "nasal::DIR";

void dir_entry_destructor(void* ptr) {
#ifndef _MSC_VER
    closedir(static_cast<DIR*>(ptr));
#else
    FindClose(ptr);
#endif
}

var builtin_opendir(context* ctx, gc* ngc) {
    auto path = ctx->localr[1];
    if (!path.is_str()) {
        return nas_err("unix::opendir", "\"path\" must be string");
    }
#ifdef _MSC_VER
    WIN32_FIND_DATAA data;
    HANDLE p;
    p = FindFirstFileA((path.str()+"\\*.*").c_str(), &data);
    if (p==INVALID_HANDLE_VALUE) {
        return nas_err("unix::opendir", "cannot open dir <"+path.str()+">");
    }
#else
    DIR* p = opendir(path.str().c_str());
    if (!p) {
        return nas_err("unix::opendir", "cannot open dir <"+path.str()+">");
    }
#endif
    var ret = ngc->alloc(vm_type::vm_ghost);
    ret.ghost().set(dir_type_name, dir_entry_destructor, nullptr, p);
    return ret;
}

var builtin_readdir(context* ctx, gc* ngc) {
    auto handle = ctx->localr[1];
    if (!handle.object_check(dir_type_name)) {
        return nas_err("unix::readdir", "not a valid dir handle");
    }
#ifdef _MSC_VER
    WIN32_FIND_DATAA data;
    if (!FindNextFileA(handle.ghost().pointer, &data)) {
        return nil;
    }
    return ngc->newstr(data.cFileName);
#else
    dirent* p = readdir(handle.ghost().get<DIR>());
    return p? ngc->newstr(p->d_name):nil;
#endif
}

var builtin_closedir(context* ctx, gc* ngc) {
    auto handle = ctx->localr[1];
    if (!handle.object_check(dir_type_name)) {
        return nas_err("unix::closedir", "not a valid dir handle");
    }
    handle.ghost().clear();
    return nil;
}

var builtin_chdir(context* ctx, gc* ngc) {
    auto path = ctx->localr[1];
    if (!path.is_str()) {
        return var::num(-1.0);
    }
    return var::num(static_cast<f64>(chdir(path.str().c_str())));
}

var builtin_environ(context* ctx, gc* ngc) {
    var res = ngc->temp = ngc->alloc(vm_type::vm_vec);
    auto& vec = res.vec().elems;
    for (char** env = environ; *env; ++env) {
        vec.push_back(ngc->newstr(*env));
    }
    ngc->temp = nil;
    return res;
}

var builtin_getcwd(context* ctx, gc* ngc) {
    char buf[1024];
    if (!getcwd(buf, sizeof(buf))) {
        return nil;
    }
    return ngc->newstr(buf);
}

var builtin_getenv(context* ctx, gc* ngc) {
    auto envvar = ctx->localr[1];
    if (!envvar.is_str()) {
        return nas_err("unix::getenv", "\"envvar\" must be string");
    }
    char* res = getenv(envvar.str().c_str());
    return res? ngc->newstr(res):nil;
}

nasal_builtin_table unix_lib_native[] = {
    {"__opendir", builtin_opendir},
    {"__readdir", builtin_readdir},
    {"__closedir", builtin_closedir},
    {"__chdir", builtin_chdir},
    {"__environ", builtin_environ},
    {"__getcwd", builtin_getcwd},
    {"__getenv", builtin_getenv},
    {nullptr, nullptr}
};

}
