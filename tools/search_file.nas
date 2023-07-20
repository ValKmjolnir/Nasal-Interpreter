import.std.file;
import.std.sort;

if (size(arg)!=1) {
    println("need a key string to search files.");
    exit(-1);
}

var needle = arg[0];

var do_flat = func(vec) {
    var flat = [];
    var bfs = [vec];
    while(size(bfs)) {
        var d = pop(bfs);
        foreach(var f;d.files) {
            if (ishash(f)) {
                append(bfs,f);
                continue;
            }
            append(flat, d.dir~"/"~f);
        }
    }
    sort.sort(flat, func(a,b){return cmp(a,b)<0});
    return flat;
}

var count = 0;
foreach(var f;do_flat(file.recursive_find_files("."))) {
    var pos = find(needle, f);
    if (pos == -1) {
        continue;
    }
    count += 1;
    var begin = substr(f, 0, pos);
    var end = pos+size(needle)>=size(f)? "":substr(f, pos+size(needle), size(f));
    println(begin, "\e[95;1m", needle, "\e[0m", end);
}

println("\n", count, " result(s).");