use std.file;

var check = func(dir_name) {
    var ts = maketimestamp();
    var files = file.recursive_find_files_with_extension(dir_name, "nas");
    var res = [];
    foreach(var f; files) {
        ts.stamp();
        if (system("nasal -c "~f~" 1>/dev/null 2>/dev/null")!=0) {
            println("\e[31merror\e[0m ", f);
            append(res, f);
        }
        println("compiling ", f, " in \e[32m", ts.elapsedUSec()/1000, "\e[0m ms");
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