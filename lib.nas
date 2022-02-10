# lib.nas

# import is used to link another file, this lib function will do nothing.
# because nasal_import will recognize this and link files before generating bytecode.
var import=func(filename){
    return __builtin_import(filename);
}

# print is used to print all things in nasal, try and see how it works.
# this function uses std::cout/printf to output logs.
var print=func(elems...){
    return __builtin_print(elems);
}

# append is used to add values into a vector.
var append=func(vec,elems...){
    return __builtin_append(vec,elems);
}

# setsize is used to change the size of vector.
# if the size is larger than before,
# this function will fill vm_nil into uninitialized space.
var setsize=func(vec,size){
    return __builtin_setsize(vec,size);
}

# system has the same use in C.
var system=func(str){
    return __builtin_system(str);
}

# input uses std::cin and returns what we input.
var input=func(){
    return __builtin_input();
}

# split a string by delimiter for example:
# split("ll","hello world") -> ["he","o world"]
# this function will return a vector.
var split=func(deli,str){
    return __builtin_split(deli,str);
}

# rand has the same function as the rand in C
# if seed is nil, it will return the random number.
# if seed is not nil, it will be initialized by this seed.
var rand=func(seed=nil){
    return __builtin_rand(seed);
}

# id will return the pointer of an gc-object.
# if this object is not managed by gc, it will return 0.
var id=func(object){
    return __builtin_id(object);
}

# int will get the integer of input number.
# but carefully use it, because int has range between -2147483648~2147483647
var int=func(val){
    return __builtin_int(val);
}

# num will change all the other types into number.
# mostly used to change a numerable string.
var num=func(val){
    return __builtin_num(val);
}

# pop used to pop the last element in a vector.
# this function will return the value that poped if vector has element(s).
# if the vector is empty, it will return nil.
var pop=func(vec){
    return __builtin_pop(vec);
}

# str is used to change number into string.
var str=func(num){
    return __builtin_str(num);
}

# size can get the size of a string/vector/hashmap.
# in fact it can also get the size of number, and the result is the number itself.
# so don't do useless things, though it really works.
var size=func(object){
    return __builtin_size(object);
}

# contains is used to check if a key exists in a hashmap/dict.
var contains=func(hash,key){
    return __builtin_contains(hash,key);
}

# delete is used to delete a pair in a hashmap/dict by key.
var delete=func(hash,key){
    return __builtin_delete(hash,key);
}

# keys is used to get all keys in a hashmap/dict.
# this function will return a vector.
var keys=func(hash){
    return __builtin_keys(hash);
}

# time has the same function in C.
var time=func(begin){
    return __builtin_time(begin);
}

# die is a special native function.
# use it at where you want the program to crash immediately.
var die=func(str){
    return __builtin_die(str);
}

# typeof is used to get the type of an object.
# this function returns a string.
var typeof=func(object){
    return __builtin_type(object);
}

# substr will get the sub-string.
# it gets the string, the begin index and sub-string's length as arguments.
var substr=func(str,begin,len){
    return __builtin_substr(str,begin,len);
}

# streq is used to compare if two strings are the same.
var streq=func(a,b){
    return __builtin_streq(a,b);
}

# left is used to get the sub-string like substr.
# but the begin index is 0.
var left=func(str,len){
    return __builtin_left(str,len);
}

# right i used to get the sub-string like substr.
# but the begin index is strlen-len.
var right=func(str,len){
    return __builtin_right(str,len);
}

# cmp is used to compare two strings.
# normal string will not be correctly compared by operators < > <= >=
# because these operators will turn strings into numbers then compare.
var cmp=func(a,b){
    return __builtin_cmp(a,b);
}

# chr is used to get the character by ascii-number.
# for example chr(65) -> 'A'
var chr=func(code){
    return __builtin_chr(code);
}

# println has the same function as print.
# but it will output a '\n' after using print.
var println=func(elems...){
    __builtin_print(elems);
    elems=['\n'];
    return __builtin_print(elems);
}

var io=
{
    SEEK_SET:0,
    SEEK_CUR:1,
    SEEK_END:2,
    # get content of a file by filename. returns a string.
    fin:   func(filename){return __builtin_fin(filename);},
    # input a string as the content of a file.
    fout:  func(filename,str){return __builtin_fout(filename,str);},
    # same as C fopen. open file and get the FILE*.
    open:  func(filename,mode="r"){return __builtin_open(filename,mode);},
    # same as C fclose. close file by FILE*.
    close: func(filehandle){return __builtin_close(filehandle);},
    # same as C fread. read file by FILE*.
    read:  func(filehandle,buf,len){return __builtin_read(filehandle,buf,len);},
    # same as C fwrite. write file by FILE*.
    write: func(filehandle,str){return __builtin_write(filehandle,str);},
    # same as C fseek. seek place by FILE*.
    seek:  func(filehandle,pos,whence){return __builtin_seek(filehandle,pos,whence);},
    # same as C ftell.
    tell:  func(filehandle){return __builtin_tell(filehandle);},
    # read file by lines. use FILE*.
    # get nil if EOF
    readln:func(filehandle){return __builtin_readln(filehandle);},
    # same as C stat.
    stat:  func(filename){return __builtin_stat(filename);},
    # same as C feof. check if FILE* gets the end of file(EOF).
    eof:   func(filehandle){return __builtin_eof(filehandle);}
};

# get file status. using data from io.stat
var fstat=func(filename){
    var s=io.stat(filename);
    return {
        st_dev:  s[0],
        st_ino:  s[1],
        st_mode: s[2],
        st_nlink:s[3],
        st_uid:  s[4],
        st_gid:  s[5],
        st_rdev: s[6],
        st_size: s[7],
        st_atime:s[8],
        st_mtime:s[9],
        st_ctime:s[10]
    };
}

# functions that do bitwise calculation.
# carefully use it, all the calculations are based on integer.
var bits=
{
    # xor
    bitxor:  func(a,b){return __builtin_xor(a,b); },
    # and
    bitand:  func(a,b){return __builtin_and(a,b); },
    # or
    bitor:   func(a,b){return __builtin_or(a,b);  },
    # nand
    bitnand: func(a,b){return __builtin_nand(a,b);},
    # not
    bitnot:  func(a)  {return __builtin_not(a);   },
    # get bit data from a special string. for example:
    # bits.fld(s,0,3);
    # if s stores 10100010(162)
    # will get 101(5).
    fld:     func(str,startbit,len){return __builtin_fld;},
    # get sign-extended data from a special string. for example:
    # bits.sfld(s,0,3);
    # if s stores 10100010(162)
    # will get 101(5) then this will be signed extended to
    # 11111101(-3).
    sfld:    func(str,startbit,len){return __builtin_sfld;},
    # set value into a special string to store it. little-endian, for example:
    # bits.setfld(s,0,8,69);
    # set 01000101(69) to string will get this:
    # 10100010(162)
    # so s[0]=162.
    setfld:  func(str,startbit,len,val){return __builtin_setfld;},
    # get a special string filled by '\0' to use in setfld.
    buf:     func(len){return __builtin_buf;}
};

# mostly used math functions and special constants, you know.
var math=
{
    e:     2.7182818284590452354,
    pi:    3.14159265358979323846264338327950288,
    inf:   1/0,
    nan:   0/0,
    sin:   func(x)  {return __builtin_sin(x);    },
    cos:   func(x)  {return __builtin_cos(x);    },
    tan:   func(x)  {return __builtin_tan(x);    },
    exp:   func(x)  {return __builtin_exp(x);    },
    lg:    func(x)  {return __builtin_lg(x);     },
    ln:    func(x)  {return __builtin_ln(x);     },
    sqrt:  func(x)  {return __builtin_sqrt(x);   },
    atan2: func(x,y){return __builtin_atan2(x,y);},
    isnan: func(x)  {return __builtin_isnan(x);  }
};

var unix=
{
    pipe:     func(){die("not supported yet");},
    fork:     func(){die("not supported yet");},
    dup2:     func(fd0,fd1){die("not supported yet");},
    exec:     func(filename,argv,envp){die("not supported yet");},
    waitpid:  func(pid,nohang=0){die("not supported yet");},
    isdir:    func(path){return bits.bitand(io.stat(path)[2],0x4000);}, # S_IFDIR 0x4000
    isfile:   func(path){return bits.bitand(io.stat(path)[2],0x8000);}, # S_IFREG 0x8000
    opendir:  func(path){return __builtin_opendir;},
    readdir:  func(handle){return __builtin_readdir;},
    closedir: func(handle){return __builtin_closedir;},
    time:     func(){return time(0);},
    sleep:    func(secs){return __builtin_sleep(secs);},
    chdir:    func(path){return __builtin_chdir(path);},
    environ:  func(){die("not supported yet");},
    getcwd:   func(){return __builtin_getcwd();},
    getenv:   func(envvar){return __builtin_getenv(envvar);}
};

# dylib is the core hashmap for developers to load their own library.
var dylib=
{
    # open dynamic lib.
    dlopen:  func(libname){return __builtin_dlopen;},
    # load symbol from an open dynamic lib.
    dlsym:   func(lib,sym){return __builtin_dlsym; },
    # close dynamic lib, this operation will make all the symbols loaded from it invalid.
    dlclose: func(lib){return __builtin_dlclose;   },
    # call the loaded symbol.
    dlcall:  func(funcptr,args...){return __builtin_dlcall}
};

# os is used to use or get some os-related info/functions.
# windows/macOS/linux are supported.
var os=
{
    # get a string that tell which os it runs on.
    platform: func(){return __builtin_platform;}
};

# runtime gives us some functions that we could manage it manually.
var runtime=
{
    # do garbage collection manually.
    # carefully use it because using it frequently may make program running slower.
    gc: func(){return __builtin_gc;}
};

# important global constants
var D2R=math.pi/180;
var FPS2KT=0.5925;
var FT2M=0.3048;
var GAL2L=3.7854;
var IN2M=0.0254;
var KG2LB=2.2046;
var KT2FPS=1.6878;
var KT2MPS=0.5144;
var L2GAL=0.2642;
var LB2KG=0.4536;
var M2FT=3.2808;
var M2IN=39.3701;
var M2NM=0.00054;
var MPS2KT=1.9438;
var NM2M=1852;
var R2D=180/math.pi;