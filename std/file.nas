# lib file.nas
# ValKmjolnir 2022/3/6
var file={
    SEEK_SET:io.SEEK_SET,
    SEEK_CUR:io.SEEK_CUR,
    SEEK_END:io.SEEK_END,
    new: func(filename,mode="r"){
        if(!io.exists(filename))
            return nil;
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

var find_all_files_with_extension=func(path,extensions...){
    var in_vec=func(ext){
        foreach(var i;extensions){
            if(ext==i){
                return 1;
            }
        }
        return 0;
    }
    var res=[];
    foreach(var f;find_all_files(path)){
        var tmp=split('.',f);
        if(size(tmp)>1 and in_vec(tmp[-1])){
            append(res,f);
        }
    }
    return res;
}

var find_all_files=func(path){
    if(!io.exists(path))
        return [];
    var dd=unix.opendir(path);
    var res=[];
    while(var n=unix.readdir(dd))
        if(unix.isfile(path~"/"~n))
            append(res,n);
    unix.closedir(dd);
    return res;
}

var recursive_find_files=func(path){
    if(!io.exists(path))
        return nil;
    var dd=unix.opendir(path);
    var res={
        dir:path,
        files:[]
    };
    while(var n=unix.readdir(dd)){
        if(unix.isfile(path~"/"~n)){
            append(res.files,n);
        }elsif(unix.isdir(path~"/"~n) and n!="." and n!=".."){
            var tmp=recursive_find_files(path~"/"~n);
            if(tmp!=nil)
                append(res.files,tmp);
        }

    }
    unix.closedir(dd);
    return res;
}