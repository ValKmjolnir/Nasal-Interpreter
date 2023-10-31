use std.file;

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

var do_flat = func(vec) {
    var flat = [];
    var bfs = [vec];
    while(size(bfs)) {
        var d = pop(bfs);
        foreach(var f; d.files) {
            if (ishash(f)) {
                append(bfs,f);
                continue;
            }
            append(flat, d.dir~"/"~f);
        }
    }
    sort(flat, func(a, b){return cmp(a, b)<0});
    return flat;
}

var count = 0;
foreach(var f; do_flat(file.recursive_find_files("."))) {
    var pos = find(needle, f);
    if (pos == -1) {
        continue;
    }
    count += 1;
    var begin = substr(f, 0, pos);
    var end = pos+size(needle)>=size(f)? "":substr(f, pos+size(needle), size(f));
    var file_size = fstat(f).st_size;
    var unit = "b";
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
    println(begin, "\e[95;1m", needle, "\e[0m", end, " | ", file_size, " ", unit);
}

println("\n", count, " result(s).");