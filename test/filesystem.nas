import("lib.nas");

var fd=io.open("test/filesystem.nas");
while((var line=io.readln(fd))!=nil)
    println(line);
io.close(fd);
println(io.stat("test/filesystem.nas"));

var dd=unix.opendir("test");
while((var name=unix.readdir(dd))!=nil)
    println(name);
unix.closedir(dd);