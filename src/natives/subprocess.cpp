#include "natives/subprocess.h"

#include <cstdio>

namespace nasal {

const auto subprocess_object_name = "subprocess";

void subprocess_destructor(void* ptr) {
    pclose(static_cast<FILE*>(ptr));
}

var builtin_subprocess_popen(context* ctx, gc* ngc) {
    auto command = ctx->localr[1];
    if (!command.is_str()) {
        return nas_err("subprocess::popen", "expect a string as the command");
    }

    auto fd = popen(command.str().c_str(), "r");
    if (fd == nullptr) {
        return nas_err("subprocess::popen", "failed to create sub-process");
    }

    var ret = ngc->alloc(vm_type::vm_ghost);
    ret.ghost().set(subprocess_object_name, subprocess_destructor, nullptr, fd);
    return ret;
}

var builtin_subprocess_pclose(context* ctx, gc* ngc) {
    auto fd = ctx->localr[1];
    if (!fd.object_check(subprocess_object_name)) {
        return nas_err("subprocess::pclose", "not a valid subprocess");
    }
    auto res = pclose(static_cast<FILE*>(fd.ghost().pointer));
    fd.ghost().pointer = nullptr;
    fd.ghost().clear();
    return var::num(res);
}

var builtin_subprocess_read_stdout(context* ctx, gc* ngc) {
    auto fd = ctx->localr[1];
    if (!fd.object_check(subprocess_object_name)) {
        return nas_err("subprocess::pclose", "not a valid subprocess");
    }
    char buffer[1025];
    auto res = ngc->alloc(vm_type::vm_str);
    while(fgets(
        buffer,
        sizeof(buffer)-1,
        static_cast<FILE*>(fd.ghost().pointer))!=nullptr) {
        res.str() += buffer;
    }
    return res;
}

nasal_builtin_table subprocess_native[] = {
    {"__subprocess_popen", builtin_subprocess_popen},
    {"__subprocess_pclose", builtin_subprocess_pclose},
    {"__subprocess_read_stdout", builtin_subprocess_read_stdout},
    {nullptr, nullptr}
};

}