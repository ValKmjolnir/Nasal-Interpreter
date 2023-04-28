# lib.nas
# 2019 ValKmjolnir

# print is used to print all things in nasal, try and see how it works.
# this function uses std::cout to output logs.
var print=func(elems...){
    return __print(elems);
}

# append is used to add values into a vector.
var append=func(vec,elems...){
    return __append(vec,elems);
}

# setsize is used to change the size of vector.
# if the size is larger than before,
# this function will fill vm_nil into uninitialized space.
var setsize=func(vec,size){
    return __setsize(vec,size);
}

# system has the same use in C.
var system=func(str){
    return __system(str);
}

# input uses std::cin and returns what we input.
var input=func(end=nil){
    return __input(end);
}

# readline
var readline=func(prompt="> ") {
    print(prompt);
    return input("\n");
}

# split a string by separator for example:
# split("ll","hello world") -> ["he","o world"]
# this function will return a vector.
var split=func(separator,str){
    return __split(separator,str);
}

# rand has the same function as the rand in C
# if seed is nil, it will return the random number.
# if seed is not nil, it will be initialized by this seed.
var rand=func(seed=nil){
    return __rand(seed);
}

# id will return the pointer of an gc-object.
# if this object is not managed by gc, it will return 0.
var id=func(object){
    return __id(object);
}

# int will get the integer of input number/string.
# but carefully use it, because int has range between -2147483648~2147483647
var int=func(val){
    return __int(val);
}

# floor will get the integral number of input argument
# which is less than or equal to this argument
var floor=func(val){
    return __floor(val);
}

# exit using std::exit
var exit=func(val=-1){
    return __exit(val);
}

# abort using std::abort
var abort=func(){
    __abort();
}

# abs gets absolute number.
var abs=func(n){
    return n>0?n:-n;
}

# num will change all the other types into number.
# mostly used to change a numerable string.
var num=func(val){
    return __num(val);
}

# pop used to pop the last element in a vector.
# this function will return the value that poped if vector has element(s).
# if the vector is empty, it will return nil.
var pop=func(vec){
    return __pop(vec);
}

# str is used to change number into string.
var str=func(num){
    return __str(num);
}

# size can get the size of a string/vector/hashmap.
# in fact it can also get the size of number, and the result is the number itself.
# so don't do useless things, though it really works.
var size=func(object){
    return __size(object);
}

# contains is used to check if a key exists in a hashmap/dict.
var contains=func(hash,key){
    return __contains(hash,key);
}

# delete is used to delete a pair in a hashmap/dict by key.
var delete=func(hash,key){
    return __delete(hash,key);
}

# keys is used to get all keys in a hashmap/dict.
# this function will return a vector.
var keys=func(hash){
    return __keys(hash);
}

# time has the same function in C.
var time=func(begin){
    return __time(begin);
}
var systime=func(){
    return time(0);
}

# die is a special native function.
# use it at where you want the program to crash immediately.
var die=func(str){
    return __die(str);
}

# find will give the first position of the needle in haystack
var find=func(needle,haystack){
    return __find(needle,haystack);
}

# typeof is used to get the type of an object.
# this function returns a string.
var typeof=func(object){
    return __type(object);
}

# subvec is used to get part of a vector
var subvec=func(vec,begin,length=nil){
    return vec[begin:(length==nil?nil:begin+length-1)];
}

# substr will get the sub-string.
# it gets the string, the begin index and sub-string's length as arguments.
var substr=func(str,begin,len){
    return __substr(str,begin,len);
}

# streq is used to compare if two strings are the same.
var streq=func(a,b){
    return __streq(a,b);
}

# left is used to get the sub-string like substr.
# but the begin index is 0.
var left=func(str,len){
    return __left(str,len);
}

# right i used to get the sub-string like substr.
# but the begin index is strlen-len.
var right=func(str,len){
    return __right(str,len);
}

# cmp is used to compare two strings.
# normal string will not be correctly compared by operators < > <= >=
# because these operators will turn strings into numbers then compare.
var cmp=func(a,b){
    return __cmp(a,b);
}

# chr is used to get the character by ascii-number.
# for example chr(65) -> 'A'
var chr=func(code){
    return __chr(code);
}

# char will give you the real character of ascii-number
# instead of extend-ascii when number between 128~256
var char=func(code){
    return __char(code);
}

# mut is used to change unmutable strings to mutable.
var mut=func(str){
    return str~"";
}

# srand wraps up rand, using time(0) as the seed.
var srand=func(){
    rand(time(0));
    return 0;
}

# values() gets all values in a hash.
var values=func(hash){
    return __values(hash);
}

# println has the same function as print.
# but it will output a '\n' after using print.
var println=func(elems...){
    return __println(elems);
}

var isfunc=func(f){
    return typeof(f)=="func";
}

var isghost=func(g){
    die("this runtime has no ghost object");
    return 0;
}

var ishash=func(h){
    return typeof(h)=="hash";
}

var isint=func(x){
    return x==floor(x);
}

var isnum=func(x){
    return typeof(x)=="num" or !math.isnan(num(x));
}

var isscalar=func(s){
    var t=typeof(s);
    return (t=="num" or t=="str")?1:0;
}

var isstr=func(s){
    return typeof(s)=="str";
}

var isvec=func(v){
    return typeof(v)=="vec";
}


# get the index of val in the vec
var vecindex=func(vec,val){
    forindex(var i;vec)
        if(val==vec[i])
            return i;
    return nil;
}

# check if the object is an instance of the class
var isa=func(object,class){
    if(!contains(object,"parents") or typeof(object.parents)!="vec")
        return 0;
    foreach(var elem;object.parents)
        if(elem==class)
            return 1;
    return 0;
}

# assert aborts when condition is not true
var assert=func(condition,message="assertion failed!"){
    if(condition)
        return 1;
    die(message);
}

# get time stamp, this will return a timestamp object
var maketimestamp=func(){
    var t=0;
    return {
        stamp:func(){t=__millisec();},
        elapsedMSec:func(){return __millisec()-t;},
        elapsedUSec:func(){return (__millisec()-t)*1000;}
    };
}

# md5
var md5=func(str){
    return __md5(str);
}

var io={
    SEEK_SET:0,
    SEEK_CUR:1,
    SEEK_END:2,
    # get content of a file by filename. returns a string.
    readfile: func(filename){return __readfile(filename);},
    # input a string as the content of a file.
    fout:  func(filename,str){return __fout(filename,str);},
    # use C access
    exists:func(filename){return __exists(filename);},
    # same as C fopen. open file and get the FILE*.
    open:  func(filename,mode="r"){return __open(filename,mode);},
    # same as C fclose. close file by FILE*.
    close: func(filehandle){return __close(filehandle);},
    # same as C fread. read file by FILE*.
    # caution: buf must be a mutable string.use mut("") to get an empty mutable string.
    read:  func(filehandle,buf,len){return __read(filehandle,buf,len);},
    # same as C fwrite. write file by FILE*.
    write: func(filehandle,str){return __write(filehandle,str);},
    # same as C fseek. seek place by FILE*.
    seek:  func(filehandle,pos,whence){return __seek(filehandle,pos,whence);},
    # same as C ftell.
    tell:  func(filehandle){return __tell(filehandle);},
    # read file by lines. use FILE*.
    # get nil if EOF
    readln:func(filehandle){return __readln(filehandle);},
    # same as C stat.
    stat:  func(filename){return __stat(filename);},
    # same as C feof. check if FILE* gets the end of file(EOF).
    eof:   func(filehandle){return __eof(filehandle);}
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
var bits={
    # u32 xor
    u32_xor: func(a,b){return __u32xor(a,b); },
    # u32 and
    u32_and: func(a,b){return __u32and(a,b); },
    # u32 or
    u32_or:  func(a,b){return __u32or(a,b);  },
    # u32 nand
    u32_nand:func(a,b){return __u32nand(a,b);},
    # u32 not
    u32_not: func(a)  {return __u32not(a);   },

    # get bit data from a special string. for example:
    # bits.fld(s,0,3);
    # if s stores 10100010(162)
    # will get 101(5).
    fld:     func(str,startbit,len){return __fld;},
    # get sign-extended data from a special string. for example:
    # bits.sfld(s,0,3);
    # if s stores 10100010(162)
    # will get 101(5) then this will be signed extended to
    # 11111101(-3).
    sfld:    func(str,startbit,len){return __sfld;},
    # set value into a special string to store it. little-endian, for example:
    # bits.setfld(s,0,8,69);
    # set 01000101(69) to string will get this:
    # 10100010(162)
    # so s[0]=162.
    setfld:  func(str,startbit,len,val){return __setfld;},
    # get a special string filled by '\0' to use in setfld.
    buf:     func(len){return __buf;}
};

# mostly used math functions and special constants, you know.
var math={
    e:     2.7182818284590452354,
    pi:    3.14159265358979323846264338327950288,
    D2R:   2.7182818284590452354/180,
    R2D:   180/2.7182818284590452354,
    inf:   1/0,
    nan:   0/0,
    abs:   func(x)  {return x>0?x:-x;    },
    floor: func(x)  {return __floor(x);  },
    pow:   func(x,y){return __pow(x,y);  },
    sin:   func(x)  {return __sin(x);    },
    cos:   func(x)  {return __cos(x);    },
    tan:   func(x)  {return __tan(x);    },
    exp:   func(x)  {return __exp(x);    },
    lg:    func(x)  {return __lg(x);     },
    ln:    func(x)  {return __ln(x);     },
    sqrt:  func(x)  {return __sqrt(x);   },
    atan2: func(x,y){return __atan2(x,y);},
    isnan: func(x)  {return __isnan(x);  },
    max:   func(x,y){return x>y?x:y;     },
    min:   func(x,y){return x<y?x:y;     }
};

var unix={
    pipe:     func(){return __pipe;},
    fork:     func(){return __fork;},
    dup2:     func(fd0,fd1){die("not supported yet");},
    exec:     func(filename,argv,envp){die("not supported yet");},
    waitpid:  func(pid,nohang=0){return __waitpid;},
    isdir:    func(path){return !!bits.u32_and(io.stat(path)[2],0x4000);}, # S_IFDIR 0x4000
    isfile:   func(path){return !!bits.u32_and(io.stat(path)[2],0x8000);}, # S_IFREG 0x8000
    opendir:  func(path){return __opendir;},
    readdir:  func(handle){return __readdir;},
    closedir: func(handle){return __closedir;},
    time:     func(){return time(0);},
    sleep:    func(secs){return __sleep(secs);},
    chdir:    func(path){return __chdir(path);},
    environ:  func(){return __environ();},
    getcwd:   func(){return __getcwd();},
    getenv:   func(envvar){return __getenv(envvar);},
    getpath:  func(){return split(os.platform()=="windows"?";":":",unix.getenv("PATH"));}
};

# dylib is the core hashmap for developers to load their own library.
# for safe using dynamic library, you could use 'module' in stl/module.nas
var dylib={
    # open dynamic lib. return a hash including dl pointer and function pointers
    dlopen:  func(libname){
        # find dynamic lib from local dir first
        libname=(os.platform()=="windows"?".\\":"./")~libname;
        if(io.exists(libname))
            return __dlopen(libname);
        # find dynamic lib through PATH
        var envpath=split(os.platform()=="windows"?";":":",unix.getenv("PATH"));
        # first find ./module
        append(envpath,".");
        var path=os.platform()=="windows"?"\\module\\":"/module/";
        foreach(var p;envpath){
            p~=path~libname;
            if(io.exists(p)){
                libname=p;
                break;
            }
        }
        return __dlopen(libname);
    },
    # close dynamic lib, this operation will make all the symbols loaded from it invalid.
    dlclose: func(lib){return __dlclose;   },
    # call the loaded symbol, with infinite parameters:
    # Caution: this may cause garbage collection process, be aware of the performance.
    dlcall:  func(ptr,args...){return __dlcallv},
    # get dlcall function with limited parameter list
    limitcall: func(arg_size=0){
        if(arg_size==0){return func(ptr){return __dlcall};}
        elsif(arg_size==1){return func(ptr,_0){return __dlcall};}
        elsif(arg_size==2){return func(ptr,_0,_1){return __dlcall};}
        elsif(arg_size==3){return func(ptr,_0,_1,_2){return __dlcall};}
        elsif(arg_size==4){return func(ptr,_0,_1,_2,_3){return __dlcall};}
        elsif(arg_size==5){return func(ptr,_0,_1,_2,_3,_4){return __dlcall};}
        elsif(arg_size==6){return func(ptr,_0,_1,_2,_3,_4,_5){return __dlcall};}
        elsif(arg_size==7){return func(ptr,_0,_1,_2,_3,_4,_5,_6){return __dlcall};}
        elsif(arg_size==8){return func(ptr,_0,_1,_2,_3,_4,_5,_6,_7){return __dlcall};}
        else{return func(ptr,args...){return __dlcallv};}
    }
};

# os is used to use or get some os-related info/functions.
# windows/macOS/linux are supported.
var os={
    # get a string that tell which os it runs on.
    platform: func() {return __platform;},
    time: func() {return __logtime;},
    arch: func() {return __arch;}
};

# runtime gives us some functions that we could manage it manually.
var runtime={
    # command line arguments
    argv: func(){return __sysargv;},
    gc: {
        extend: func(type){return __gcextd;}
    }
};

# functions that not supported in this runtime:
var bind=func(function,locals,outer_scope=nil){
    die("this runtime does not support bind");
}

var call=func(function,args=nil,_me=nil,locals=nil,error=nil){
    die("this runtime does not support call");
}

var caller=func(level=1){
    die("this runtime does not support caller");
}

var closure=func(function,level=1){
    die("this runtime uses \"vm_upval\" instead of \"vm_hash\" as the closure");
}

var compile=func(code,filename="<compile>"){
    die("this runtime uses static code generator");
}

var coroutine={
    create: func(function)   {return __cocreate;},
    resume: func(co,args...) {return __coresume;},
    yield:  func(args...)    {return __coyield; },
    status: func(co)         {return __costatus;},
    running:func()           {return __corun;   }
};
