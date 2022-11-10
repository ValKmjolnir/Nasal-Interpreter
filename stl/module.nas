# module.nas
# ValKmjolnir 2022/3/5

# this provides safe usage of dylib
# when dylib is closed,
# all the invalid functions cannot be called

var module_call_func=func(fptr,args){
    return __dlcallv;
}
var extern={
    new: func(fptr){
        var isopen=1;
        return {
            close:func(){isopen=0;},
            call:func(args...){
                return isopen?module_call_func(fptr,args):nil;
            }
        };
    }
};
var module={
    new: func(name){
        var lib=dylib.dlopen(name);
        var f={};
        return {
            get:func(symbol){
                if(contains(f,symbol))
                    return f[symbol];
                var fp=extern.new(dylib.dlsym(lib,symbol));
                f[symbol]=fp;
                return fp;
            },
            close: func(){
                foreach(var i;keys(f))
                    f[i].close();
                dylib.dlclose(lib);
            }
        };
    }
};