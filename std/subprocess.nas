var popen = func(cmd) {
    return __subprocess_popen(cmd);
}

var pclose = func(subproc) {
    return __subprocess_pclose(subproc);
}

var read = func(subproc) {
    return __subprocess_read_stdout(subproc);
}

var fork = func(vec) {
    return __subprocess_fork(vec);
}

var kill = func(pid) {
    return __subprocess_kill(pid);
}
