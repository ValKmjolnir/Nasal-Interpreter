# lib file.nas
# ValKmjolnir 2022/3/6
import("lib.nas");

var file={
    SEEK_SET:io.SEEK_SET,
    SEEK_CUR:io.SEEK_CUR,
    SEEK_END:io.SEEK_END,
    new: func(filename,mode="r"){
        var fd=io.open(filename,mode);
        return {
            close:  func(){io.close(fd);},
            read:   func(len){
                var buf=mut("");
                io.read(fd,buf,len);
                return buf;
            },
            write:  func(str){return io.write(fd,str);},
            seek:   func(pos,whence){return io.seek(fd,pos,whence);},
            tell:   func(){return io.tell(fd);},
            readln: func(){return io.readln(fd);},
            stat:   func(){return io.stat(filename);},
            eof:    func(){return io.eof(fd);}
        };
    }
};

var find_all_files=func(path){
    var dd=unix.opendir(path);
    var res=[];
    while(var n=unix.readdir(dd))
        if(unix.isfile(path~"/"~n))
            append(res,n);
    unix.closedir(dd);
    return res;
}