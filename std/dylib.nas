# dylib.nas
# 2023 by ValKmjolnir
# dylib is the core hashmap for developers to load their own library.
# for safe using dynamic library, you could use 'module' in stl/module.nas
use std.os;
use std.io;
use std.unix;

# open dynamic lib. return a hash including dl pointer and function pointers
var dlopen = func(libname) {
    return __dlopen(libname);
}

# close dynamic lib, this operation will make all the symbols loaded from it invalid.
var dlclose = func(lib) {
    return __dlclose;
}

# call the loaded symbol, with infinite parameters:
# Caution: this may cause garbage collection process, be aware of the performance.
var dlcall = func(ptr, args...) {
    return __dlcallv;
}

# get dlcall function with limited parameter list
var limitcall = func(arg_size = 0) {
    if (arg_size==0) {return func(ptr) {return __dlcall};}
    elsif (arg_size==1) {return func(ptr, _0) {return __dlcall};}
    elsif (arg_size==2) {return func(ptr, _0, _1) {return __dlcall};}
    elsif (arg_size==3) {return func(ptr, _0, _1, _2) {return __dlcall};}
    elsif (arg_size==4) {return func(ptr, _0, _1, _2, _3) {return __dlcall};}
    elsif (arg_size==5) {return func(ptr, _0, _1, _2, _3, _4) {return __dlcall};}
    elsif (arg_size==6) {return func(ptr, _0, _1, _2, _3, _4, _5) {return __dlcall};}
    elsif (arg_size==7) {return func(ptr, _0, _1, _2, _3, _4, _5, _6) {return __dlcall};}
    elsif (arg_size==8) {return func(ptr, _0, _1, _2, _3, _4, _5, _6, _7) {return __dlcall};}
    else {return func(ptr, args...) {return __dlcallv};}
}