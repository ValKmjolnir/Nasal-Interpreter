use test.md5_self;
use std.process_bar;
use std.file;
use std.runtime;
use std.io;
use std.math;

srand();

var compare = func() {
    var ch = [
        "0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","+",
        "_","*","/","\'","\"",".",",",";",":","<",">","!","@","#","$","%",
        "^","&","*","(",")","-","=","\\","|","[","]","{","}","`"," ","\t","?"
    ];
    return func(begin, end) {
        var byte = 0;
        var total = end-begin;
        var timestamp = maketimestamp();
        timestamp.stamp();
        var bar = process_bar.high_resolution_bar(40);
        for (var i = begin; i<end; i += 1) {
            var s = "";
            for (var j = 0; j<i; j += 1) {
                s ~= ch[rand()*size(ch)];
            }
            byte += size(s);
            var res = md5(s);
            if (cmp(res, md5_self.md5(s))) {
                die("error: "~str(i));
            }
            if (i-begin-int((i-begin)/4)*4==0) {
                print(
                    "\e[1000D ", bar.bar((i-begin+1)/total),
                    " (", i-begin+1, "/", total, ")\t",
                    res, " byte: ", int(byte/1024), "k"
                );
            }
        }
        print(
            "\e[1000D ", bar.bar((i-begin)/total),
            " (", i-begin, "/", total, ")\t",
            res, " byte: ", int(byte/1024), "k",
            " time: ", timestamp.elapsedMSec()
        );
        print("\n");
    };
}();

var add_all_cpp_files = func(vec, path) {
    foreach(var p; file.find_all_files_with_extension(path,"cpp","h")) {
        append(vec, path~"/"~p);
    }
}

var filechecksum = func() {
    var files = [];
    foreach(var p; file.find_all_files_with_extension("./test","nas")) {
        append(files, "./test/"~p);
    }
    foreach(var p; file.find_all_files_with_extension("./std","nas")) {
        append(files, "./std/"~p);
    }
    foreach(var p; file.find_all_files_with_extension("./module","nas","cpp")) {
        append(files, "./module/"~p);
    }
    foreach(var p; file.find_all_files_with_extension(".","md")) {
        append(files, "./"~p);
    }
    add_all_cpp_files(files, "./src");
    add_all_cpp_files(files, "./src/cli");
    add_all_cpp_files(files, "./src/natives");
    add_all_cpp_files(files, "./src/repl");
    add_all_cpp_files(files, "./src/util");
    foreach(var p; file.find_all_files_with_extension("./doc","md")) {
        append(files, "./doc/"~p);
    }

    var source = [];
    foreach(var f; files) {
        append(source, io.readfile(f));
    }

    var byte = 0;
    var total = size(files);
    var timestamp = maketimestamp();
    timestamp.stamp();
    var bar = process_bar.high_resolution_bar(40);
    forindex(var i; files) {
        var f = source[i];
        var res = md5(f);
        byte += size(f);
        if (cmp(res, md5_self.md5(f))) {
            die("error: "~files[i]);
        }
        print(
            "\e[1000D ", bar.bar((i+1)/total),
            " (", i+1, "/", total, ")\t", res,
            " byte: ", int(byte/1024), "k",
            " time: ", timestamp.elapsedMSec()
        );
    }
    print("\n");
}

var randomchecksum = func() {
    for (var i = 0; i<2048; i += 256) {
        compare(i, i+256);
    }
}

runtime.windows.set_utf8_output();
filechecksum();
randomchecksum();