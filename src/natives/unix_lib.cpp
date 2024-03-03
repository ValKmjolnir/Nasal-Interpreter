#include "natives/unix_lib.h"

namespace nasal {

const auto dir_type_name = "dir";

void dir_entry_destructor(void* ptr) {
#ifndef _MSC_VER
    closedir(static_cast<DIR*>(ptr));
#else
    FindClose(ptr);
#endif
}

var builtin_pipe(context* ctx, gc* ngc) {
#ifndef _WIN32
    i32 fd[2];
    var res = ngc->alloc(vm_type::vm_vec);
    if (pipe(fd)==-1) {
        return nas_err("unix::pipe", "failed to create pipe");
    }
    res.vec().elems.push_back(var::num(static_cast<f64>(fd[0])));
    res.vec().elems.push_back(var::num(static_cast<f64>(fd[1])));
    return res;
#endif
    return nas_err("unix::pipe", "not supported on windows");
}

var builtin_fork(context* ctx, gc* ngc) {
#ifndef _WIN32
    f64 res = fork();
    if (res<0) {
        return nas_err("unix::fork", "failed to fork a process");
    }
    return var::num(static_cast<f64>(res));
#endif
    return nas_err("unix::fork", "not supported on windows");
}

var builtin_waitpid(context* ctx, gc* ngc) {
    auto pid = ctx->localr[1];
    auto nohang = ctx->localr[2];
    if (!pid.is_num() || !nohang.is_num()) {
        return nas_err("unix::waitpid", "pid and nohang must be number");
    }
#ifndef _WIN32
    i32 ret_pid, status;
    ret_pid = waitpid(pid.num(), &status, nohang.num()==0? 0:WNOHANG);
    var vec = ngc->alloc(vm_type::vm_vec);
    vec.vec().elems.push_back(var::num(static_cast<f64>(ret_pid)));
    vec.vec().elems.push_back(var::num(static_cast<f64>(status)));
    return vec;
#endif
    return nas_err("unix::waitpid", "not supported on windows");
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
    dirent* p = readdir(static_cast<DIR*>(handle.ghost().pointer));
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
    for(char** env = environ; *env; ++env) {
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
    {"__pipe", builtin_pipe},
    {"__fork", builtin_fork},
    {"__waitpid", builtin_waitpid},
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
