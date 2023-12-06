use std.file;
use std.padding;
use std.process_bar;
use std.io;
use std.math;

var tips = func() {
    println("usage:");
    println("  nasal search_file.nas [key]");
}

if (size(arg)<1) {
    println("need a key string to search files.");
    tips();
    exit(-1);
} else if (size(arg)>1) {
    println("too many arguments.");
    tips();
    exit(-1);
}

var needle = arg[0];

var result = [];
var all_files = file.recursive_find_files_flat(".");
sort(all_files, func(a, b) {return cmp(a, b)<=0;});
foreach(var f; all_files) {
    var pos = find(needle, f);
    if (pos == -1) {
        continue;
    }
    var begin = substr(f, 0, pos);
    var end = pos+size(needle)>=size(f)? "":substr(f, pos+size(needle), size(f));
    var file_size = io.fstat(f).st_size;
    var unit = " b";
    if (file_size>1024) {
        file_size/=1024;
        unit = "kb";
    }
    if (file_size>1024) {
        file_size/=1024;
        unit = "mb";
    }
    if (file_size>1024) {
        file_size/=1024;
        unit = "gb";
    }
    file_size = int(file_size);
    append(result, {
        info: begin~"\e[95;1m"~needle~"\e[0m"~end,
        size: file_size,
        unit: unit
    });
}

var max_len = 0;
foreach(var elem; result) {
    var temp = size(str(elem.size)~" "~elem.unit);
    max_len = math.max(max_len, temp);
}
foreach(var elem; result) {
    var temp = padding.leftpad(str(elem.size)~" "~elem.unit, max_len);
    println(temp, " | ", elem.info);
}
println("\n", size(result), " result(s).");