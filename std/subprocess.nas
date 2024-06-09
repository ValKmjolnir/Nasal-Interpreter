var popen = func(cmd) {
    return __subprocess_popen(cmd);
}

var pclose = func(subproc) {
    return __subprocess_pclose(subproc);
}

var read = func(subproc) {
    return __subprocess_read_stdout(subproc);
}