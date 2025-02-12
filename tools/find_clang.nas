use std.unix;
use std.file;

var path_vec = split(":", unix.getenv("PATH"));
foreach (var path; path_vec) {
    var files = file.find_all_files(path);
    foreach (var f; files) {
        if (f == "clang") {
            println(path ~ "/" ~ f);
        } elsif (find("clang-", f) == 0 and isnum(substr(f, 5, size(f)))) {
            println(path ~ "/" ~ f);
        }
    }
}