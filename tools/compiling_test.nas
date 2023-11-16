use std.file;

var check = func(dir_name) {
    var ts = maketimestamp();
    var f = file.find_all_files_with_extension(dir_name, "nas");
    var res = [];
    foreach(var k; f) {
        ts.stamp();
        if (system("nasal -c "~dir_name~"/"~k~" 1>/dev/null 2>/dev/null")!=0) {
            println("\e[31merror\e[0m ", dir_name, "/", k);
            append(res, dir_name~"/"~k);
        }
        println("compiling ", dir_name, "/", k, " in \e[32m", ts.elapsedMSec(), "\e[0m ms");
    }
    return res;
}

var result = [
    check("./std"),
    check("./module"),
    check("./tools"),
    check("./test")
];
println();
foreach(var i; result) {
    foreach(var j; i) {
        println("\e[31merror\e[0m ", j);
    }
}