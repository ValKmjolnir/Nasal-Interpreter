# lib module.nas
# ValKmjolnir 2022/3/5

# this provides safe usage of dylib
# when dylib is closed,
# all the invalid functions cannot be called
import("lib.nas");

var module_call_func=func(fptr,args){
    return __builtin_dlcall;
}
var extern={
    new: func(fptr){
        return {
            fptr:fptr,
            isopen:1,
            parents:[extern]
        };
    },
    call: func(args...){
        return (!me.isopen)?nil:module_call_func(me.fptr,args);
    }
};
var module={
    new: func(name){
        return {
            name:name,
            lib:dylib.dlopen(name),
            f:{},
            parents:[module]
        };
    },
    get: func(symbol){
        if(contains(me.f,symbol))
            return me.f[symbol];
        var f=extern.new(dylib.dlsym(me.lib,symbol));
        me.f[symbol]=f;
        return f;
    },
    close: func(){
        foreach(var i;keys(me.f))
            me.f[i].isopen=0;
        dylib.dlclose(me.lib);
    }
};