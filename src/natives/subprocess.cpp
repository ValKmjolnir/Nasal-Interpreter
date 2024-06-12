#include "natives/subprocess.h"

// if you ask why, i will say: only MSVC
#ifdef _MSC_VER
#define popen _popen
#define pclose _pclose
#define perror _perror
#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifndef WIN32
#include <sys/wait.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <clocale>
#include <vector>
#include <csignal>

namespace nasal {

const auto subproc_desc_name = "subprocess_descriptor";

void subprocess_desc_dtor(void* ptr) {
#ifdef WIN32
    auto pi = &static_cast<subprocess_descriptor*>(ptr)->pi;

    WaitForSingleObject(pi->hProcess, 0);
    TerminateProcess(pi->hProcess, 0);
    CloseHandle(pi->hProcess);
    CloseHandle(pi->hThread);
#else
    auto pid = static_cast<subprocess_descriptor*>(ptr)->pid;

    int status;
    pid_t result = waitpid(pid, &status, WNOHANG);

    // child process running
    if (result==0) {
        kill(pid, SIGTERM);
    }
#endif
}

var builtin_subprocess_create(context* ctx, gc* ngc) {
    auto cmd = ctx->localr[1];
    if (!cmd.is_vec()) {
        return nas_err("subprocess::create", "expect a string as the command");
    }

    for(const auto& v : cmd.vec().elems) {
        if (!v.is_str()) {
            return nas_err("subprocess::create", "non-string arguments");
        }
    }

    auto obj = ngc->alloc(vm_type::vm_ghost);
    obj.ghost().set(
        subproc_desc_name,
        subprocess_desc_dtor,
        nullptr,
        new subprocess_descriptor
    );

#ifdef WIN32
    auto si = &static_cast<subprocess_descriptor*>(obj.ghost().pointer)->si;
    auto pi = &static_cast<subprocess_descriptor*>(obj.ghost().pointer)->pi;

    // init STARTUPINFO
    ZeroMemory(si, sizeof(STARTUPINFOW));
    si->cb = sizeof(STARTUPINFOW);
    si->dwFlags = STARTF_USESHOWWINDOW;
    si->wShowWindow = SW_SHOW;

    // init PROCESS_INFORMATION
    ZeroMemory(pi, sizeof(PROCESS_INFORMATION));

    auto command = cmd.vec().elems[0].str();
    for(usize i = 1; i<cmd.vec().elems.size(); ++i) {
        command += " " + cmd.vec().elems[i].str();
    }

    std::setlocale(LC_ALL, ".UTF8");
    auto str_length = 1 + (command.length()<<1);
    auto buffer = new wchar_t[str_length];
    auto converted = mbstowcs(buffer, command.c_str(), str_length);
    if (converted == size_t(-1)) {
        delete[] buffer;
        return nas_err("subprocess::create", "failed to convert wchat_t*");
    }

    buffer[converted] = L'\0';

    if (!CreateProcessW(
        nullptr,
        buffer,
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        nullptr,
        si,
        pi
    )) {
        return nas_err("subprocess::create",
            "failed to create subprocess: " +
            std::to_string(GetLastError())
        );
    }
    delete[] buffer;
#else
    // create argv
    char** argv = new char*[cmd.vec().elems.size()+1];
    for(usize i = 0; i<cmd.vec().elems.size(); ++i) {
        argv[i] = strdup(cmd.vec().elems[i].str().c_str());
    }
    argv[cmd.vec().elems.size()] = nullptr;

    // create child process
    auto pid = fork();
    if (pid < 0) {
        return nas_err("subprocess::create", "failed to create sub-process");
    }
    // child process
    if (pid==0) {
        execvp(argv[0], argv);
        nas_err("subprocess::create", "failed to execute command");
        _exit(-1);
    }

    // parent process
    static_cast<subprocess_descriptor*>(obj.ghost().pointer)->pid = pid;
    for(usize i = 0; argv[i]; ++i) {
        delete argv[i];
    }
    delete[] argv;

#endif

    return obj;
}

var builtin_subprocess_terminate(context* ctx, gc* ngc) {
    auto obj = ctx->localr[1];
    if (!obj.object_check(subproc_desc_name)) {
        return nas_err("subprocess::terminate",
            "need correct subprocess descriptor"
        );
    }

#ifdef WIN32
    auto pi = &static_cast<subprocess_descriptor*>(obj.ghost().pointer)->pi;

    WaitForSingleObject(pi->hProcess, 0);
    TerminateProcess(pi->hProcess, -1);

    DWORD res;
    GetExitCodeProcess(pi->hProcess, &res);

    CloseHandle(pi->hProcess);
    CloseHandle(pi->hThread);
#else
    auto pid = static_cast<subprocess_descriptor*>(obj.ghost().pointer)->pid;

    int status;
    pid_t result = waitpid(pid, &status, WNOHANG);

    // child process running
    if (result==0) {
        kill(pid, SIGTERM);
    }

    auto res = WIFEXITED(status)? WEXITSTATUS(status):-1;
#endif

    return var::num(res);
}

nasal_builtin_table subprocess_native[] = {
    {"__subprocess_create", builtin_subprocess_create},
    {"__subprocess_terminate", builtin_subprocess_terminate},
    {nullptr, nullptr}
};

}