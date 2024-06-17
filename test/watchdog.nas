use std.runtime;
use std.os;
use std.io;
use std.unix;

use std.subprocess;

runtime.windows.set_utf8_output();

var os_time = func() {
    return "\e[33;1m[" ~ os.time() ~ "]\e[0m ";
}

var err_hd = func() {
    return "\e[91;1m[error]\e[0m ";
}

var info_hd = func() {
    return "\e[96;1m[info]\e[0m ";
}

var modified_hd = func() {
    return "\e[92;1m[modified]\e[0m ";
}

var usage = func() {
    println(
        os_time(),
        info_hd(),
        "usage: nasal watchdog.nas <filename> [\"argv\"]"
    );
}

var argv = runtime.argv();
if (size(argv)<1) {
    println(os_time(), err_hd(), "need correct file path to watch");
    usage();
    exit(-1);
}

var filename = argv[0];
if (!io.exists(filename)) {
    println(os_time(), err_hd(), "file <", filename, "> does not exist");
    usage();
    exit(-1);
}

var args = [];
if (size(argv)==2) {
    println(os_time(), info_hd(), "with argument(s) ", argv[1]);
    args = split(" ", argv[1]);
}

var modified_time = io.fstat(filename).st_mtime;
println(os_time(), info_hd(), "watching ", filename, " ..");
while(1) {
    unix.sleep(1);
    if (!io.exists(filename)) {
        println(os_time(), err_hd(), "file <", filename, "> does not exist");
        break;
    }

    var latest_modified_time = io.fstat(filename).st_mtime;
    if (latest_modified_time!=modified_time) {
        modified_time = latest_modified_time;
        println(os_time(), modified_hd(), filename);

        var cmd = "nasal " ~ filename;
        foreach(var i; args) {
            cmd ~= " " ~ i;
        }
        println(os_time(), info_hd(), "executing command \"", cmd, "\"");

        var subproc = subprocess.create(["nasal", filename] ~ args);

        # check if active every 0.5s
        var exited = false;
        while(1) {
            unix.sleep(0.5);
            if (!subprocess.active(subproc)) {
                exited = true;
                break;
            }
            if (io.fstat(filename).st_mtime!=modified_time) {
                println(os_time(), modified_hd(), "file changed, reloading..");
                break;
            }
        }

        # get return value
        var ret = subprocess.terminate(subproc);
        println(os_time(), ret!=0? err_hd():info_hd(), "process returned ", ret);
    }
}