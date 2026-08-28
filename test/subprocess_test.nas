use std.subprocess;
use std.unix;

var process = subprocess.create([
    "./nasal",
    "test/subprocess_test/infinite_loop.nas"
]);

unix.sleep(1);

println("kill subprocess...");
var res = subprocess.terminate(process);
println("subprocess killed, code ", res);