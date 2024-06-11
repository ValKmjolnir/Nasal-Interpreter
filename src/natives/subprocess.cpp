#include "natives/subprocess.h"

// if you ask why, i will say: only MSVC
#ifdef _MSC_VER
#define popen _popen
#define pclose _pclose
#define perror _perror
#endif

#ifndef _MSC_VER
#include <unistd.h>
#include <sys/wait.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <vector>

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

var builtin_subprocess_fork(context* ctx, gc* ngc) {
    auto cmd = ctx->localr[1];
    if (!cmd.is_vec()) {
        return nas_err("subprocess::fork", "expect a string as the command");
    }

#ifdef WIN32
    // STARTUPINFO si;
    // PROCESS_INFORMATION pi;

    // // init STARTUPINFO
    // ZeroMemory(&si, sizeof(si));
    // si.cb = sizeof(si);
    // si.dwFlags = STARTF_USESHOWWINDOW;
    // si.wShowWindow = SW_SHOW;

    // // init PROCESS_INFORMATION
    // ZeroMemory(&pi, sizeof(pi));

    // auto len = MultiByteToWideChar(CP_UTF8, 0, cmd.str().c_str(), -1, nullptr, 0);

    // auto res = CreateProcess(
    //     nullptr,
    //     cmd.str().c_str(),
    //     nullptr,
    //     nullptr,
    //     false,
    //     0,
    //     nullptr,
    //     nullptr,
    //     &si,
    //     &pi
    // );
#else
    // create argv
    char** argv = new char*[cmd.vec().elems.size()+1];
    for(const auto& v : cmd.vec().elems) {
        if (!v.is_str()) {
            return nas_err("subprocess::fork", "non-string arguments");
        }
    }
    for(usize i = 0; i<cmd.vec().elems.size(); ++i) {
        argv[i] = strdup(cmd.vec().elems[i].str().c_str());
    }
    argv[cmd.vec().elems.size()] = nullptr;

    // use the same env of parent process
    char** env = nullptr;
    if (getenv("PATH")) {
        env = new char*[2];
        env[0] = strdup((std::string("PATH=") + getenv("PATH")).c_str());
        env[1] = nullptr;
    } else {
        env = new char*[1];
        env[0] = nullptr;
    }

    // create child process
    auto pid = fork();
    if (pid < 0) {
        return nas_err("subprocess::fork", "failed to create sub-process");
    }
    // child process
    if (pid==0) {
        execve("nasal", argv, env);
        _exit(-1);
    }

    // parent process
    for(usize i = 0; argv[i]; ++i) {
        delete argv[i];
    }
    delete[] argv;
    for(usize i = 0; env[i]; ++i) {
        delete env[i];
    }
    delete[] env;
    return var::num(pid);
#endif

    return nil;
}

var builtin_subprocess_kill(context* ctx, gc* ngc) {
#ifdef WIN32
    // TODO
#else
    auto pid = ctx->localr[1];
    if (!pid.is_num()) {
        return nas_err("subprocess::kill", "need numeral pid");
    }

    int status;
    pid_t result = waitpid(pid.num(), &status, WNOHANG);
    // child process running
    if (result==0) {
        kill(pid.num(), SIGTERM);
    }
#endif
    return nil;
}

nasal_builtin_table subprocess_native[] = {
    {"__subprocess_popen", builtin_subprocess_popen},
    {"__subprocess_pclose", builtin_subprocess_pclose},
    {"__subprocess_read_stdout", builtin_subprocess_read_stdout},
    {"__subprocess_fork", builtin_subprocess_fork},
    {"__subprocess_kill", builtin_subprocess_kill},
    {nullptr, nullptr}
};

}