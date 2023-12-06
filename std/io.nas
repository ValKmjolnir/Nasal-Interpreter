# io.nas
# 2023 by ValKmjolnir

var SEEK_SET = 0;
var SEEK_CUR = 1;
var SEEK_END = 2;

# get content of a file by filename. returns a string.
var readfile = func(filename) {
    return __readfile(filename);
}

# input a string as the content of a file.
var fout = func(filename, str) {
    return __fout(filename, str);
}

# use C access
var exists = func(filename) {
    return __exists(filename);
}

# same as C fopen. open file and get the FILE*.
var open = func(filename, mode = "r") {
    return __open(filename, mode);
}

# same as C fclose. close file by FILE*.
var close = func(filehandle) {
    return __close(filehandle);
}

# same as C fread. read file by FILE*.
# caution: buf must be a mutable string.use mut("") to get an empty mutable string.
var read = func(filehandle, buf, len) {
    return __read(filehandle, buf, len);
}

# same as C fwrite. write file by FILE*.
var write = func(filehandle, str) {
    return __write(filehandle, str);
}

# same as C fseek. seek place by FILE*.
var seek = func(filehandle, pos, whence) {
    return __seek(filehandle, pos, whence);
}

# same as C ftell.
var tell = func(filehandle) {
    return __tell(filehandle);
}

# read file by lines. use FILE*.
# get nil if EOF
var readln = func(filehandle) {
    return __readln(filehandle);
}

# same as C stat.
var stat = func(filename) {
    return __stat(filename);
}

# same as C feof. check if FILE* gets the end of file(EOF).
var eof = func(filehandle) {
    return __eof(filehandle);
}

var stdin = func() { return __stdin; }();

var stdout = func() { return __stdout;}();

var stderr = func() { return __stderr; }();

# get file status. using data from stat
var fstat = func(filename) {
    var s = stat(filename);
    return {
        st_dev: s[0],
        st_ino: s[1],
        st_mode: s[2],
        st_nlink: s[3],
        st_uid: s[4],
        st_gid: s[5],
        st_rdev: s[6],
        st_size: s[7],
        st_atime: s[8],
        st_mtime: s[9],
        st_ctime: s[10]
    };
}
