# lib.nas
# 2019 ValKmjolnir

# print is used to print all things in nasal, try and see how it works.
# this function uses std::cout to output logs.
var print = func(elems...) {
    return __print(elems);
}

# append is used to add values into a vector.
var append = func(vec, elems...) {
    return __append(vec, elems);
}

# setsize is used to change the size of vector.
# if the size is larger than before,
# this function will fill vm_nil into uninitialized space.
var setsize = func(vec, size) {
    return __setsize(vec, size);
}

# system has the same use in C.
var system = func(str) {
    return __system(str);
}

# input uses std::cin and returns what we input.
var input = func(end = nil) {
    return __input(end);
}

# readline
var readline = func(prompt = "> ") {
    print(prompt);
    return input("\n");
}

# split a string by separator for example:
# split("ll","hello world") -> ["he","o world"]
# this function will return a vector.
var split = func(separator, str) {
    return __split(separator, str);
}

# rand has the same function as the rand in C
# if seed is nil, it will return the random number.
# if seed is not nil, it will be initialized by this seed.
var rand = func(seed = nil) {
    return __rand(seed);
}

# id will return the pointer of an gc-object.
# if this object is not managed by gc, it will return 0.
var id = func(object) {
    return __id(object);
}

# int will get the integer of input number/string.
# but carefully use it, because int has range between -2147483648~2147483647
var int = func(val) {
    return __int(val);
}

# floor will get the integral number of input argument
# which is less than or equal to this argument
var floor = func(val) {
    return __floor(val);
}

var ceil = func(val) {
    return __ceil(val);
}

# exit using std::exit
var exit = func(val = -1) {
    return __exit(val);
}

# abort using std::abort
var abort = func() {
    __abort();
}

# abs gets absolute number.
var abs = func(n) {
    return n>0? n:-n;
}

# num will change all the other types into number.
# mostly used to change a numerable string.
var num = func(val) {
    return __num(val);
}

# pop used to pop the last element in a vector.
# this function will return the value that poped if vector has element(s).
# if the vector is empty, it will return nil.
var pop = func(vec) {
    return __pop(vec);
}

# str is used to change number into string.
var str = func(num) {
    return __str(num);
}

# size can get the size of a string/vector/hashmap.
# in fact it can also get the size of number, and the result is the number itself.
# so don't do useless things, though it really works.
var size = func(object) {
    return __size(object);
}

# contains is used to check if a key exists in a hashmap/dict.
var contains = func(hash, key) {
    return __contains(hash, key);
}

# delete is used to delete a pair in a hashmap/dict by key.
var delete = func(hash, key) {
    return __delete(hash, key);
}

# keys is used to get all keys in a hashmap/dict.
# this function will return a vector.
var keys = func(hash) {
    return __keys(hash);
}

# time has the same function in C.
var time = func(begin) {
    return __time(begin);
}

var systime = func() {
    return time(0);
}

# die is a special native function.
# use it at where you want the program to crash immediately.
var die = func(str = "error occurred.") {
    return __die(str);
}

# find will give the first position of the needle in haystack
var find = func(needle, haystack) {
    return __find(needle, haystack);
}

# typeof is used to get the type of an object.
# this function returns a string.
var typeof = func(object) {
    return __type(object);
}

# subvec is used to get part of a vector
var subvec = func(vec, begin, length = nil) {
    return vec[begin:(length==nil? nil:begin+length-1)];
}

# substr will get the sub-string.
# it gets the string, the begin index and sub-string's length as arguments.
var substr = func(str, begin, len) {
    return __substr(str, begin, len);
}

# streq is used to compare if two strings are the same.
var streq = func(a, b) {
    return __streq(a, b);
}

# left is used to get the sub-string like substr.
# but the begin index is 0.
var left = func(str, len) {
    return __left(str, len);
}

# right i used to get the sub-string like substr.
# but the begin index is strlen-len.
var right = func(str, len) {
    return __right(str, len);
}

# cmp is used to compare two strings.
# normal string will not be correctly compared by operators < > <= >=
# because these operators will turn strings into numbers then compare.
var cmp = func(a, b) {
    return __cmp(a,b);
}

# chr is used to get the character by ascii-number.
# for example chr(65) -> 'A'
var chr = func(code) {
    return __chr(code);
}

# char will give you the real character of ascii-number
# instead of extend-ascii when number between 128~256
var char = func(code) {
    return __char(code);
}

# mut is used to change unmutable strings to mutable.
var mut = func(str) {
    return str~"";
}

# srand wraps up rand, using time(0) as the seed.
var srand = func() {
    rand(time(0));
    return 0;
}

# values() gets all values in a hash.
var values = func(hash) {
    return __values(hash);
}

# println has the same function as print.
# but it will output a '\n' after using print.
var println = func(elems...) {
    return __println(elems);
}

# sort function using quick sort
# not very efficient... :(
var sort = func() {
    srand();  # be aware! this causes global changes
    var quick_sort_core = func(vec, left, right, cmp) {
        if (left>=right) return nil;
        var base = left+int(rand()*(right-left));
        (vec[left], vec[base]) = (vec[base], vec[left]);
        var (i, j, tmp) = (left, right, vec[left]);
        while(i<j) {
            while(i<j and cmp(tmp,vec[j])) {
                j -= 1;
            }
            vec[i] = vec[j];
            while(i<j and cmp(vec[i],tmp)) {
                i += 1;
            }
            vec[j] = vec[i];
            j -= 1;
        }
        vec[i] = tmp;
        quick_sort_core(vec, left, i-1, cmp);
        quick_sort_core(vec, i+1, right, cmp);
        return nil;
    }
    return func(vec, cmp = func(a, b) {return a<b;}) {
        quick_sort_core(vec, 0, size(vec)-1, cmp);
        return nil;
    }
}();

var isfunc = func(f) {
    return typeof(f)=="func";
}

var isghost = func(g) {
    return typeof(g)=="obj";
}

var ishash = func(h) {
    return typeof(h)=="hash";
}

var isint = func(x) {
    return x==floor(x);
}

var isnum = func(x) {
    if (typeof(x)=="num") {
        return true;
    }
    x = num(x);
    if (!__isnan(x) and x!=nil) {
        return true;
    }
    return false;
}

var isscalar = func(s) {
    var t = typeof(s);
    return (t=="num" or t=="str")? 1:0;
}

var isstr = func(s) {
    return typeof(s)=="str";
}

var isvec = func(v) {
    return typeof(v)=="vec";
}

var ghosttype = func(ghost_object) {
    return __ghosttype(ghost_object);
}

# get the index of val in the vec
var vecindex = func(vec, val) {
    forindex(var i; vec) {
        if (val==vec[i]) {
            return i;
        }
    }
    return nil;
}

# check if the object is an instance of the class
var isa = func(object, class) {
    if (!ishash(object)) {
        return false;
    }
    if (!contains(object, "parents") or !isvec(object.parents)) {
        return false;
    }
    foreach(var elem; object.parents) {
        if (elem==class or isa(elem, class)) {
            return true;
        }
    }
    return false;
}

# assert aborts when condition is not true
var assert = func(condition, message = "assertion failed!") {
    condition or die(message);
}

# get time stamp, this will return a timestamp object
var maketimestamp = func() {
    var stamp = __maketimestamp();
    var time_stamp = func(stamp) {
        return __time_stamp(stamp);
    }
    var elapsed_millisecond = func(stamp) {
        return __elapsed_millisecond(stamp);
    }
    var elapsed_microsecond = func(stamp) {
        return __elapsed_microsecond(stamp);
    }
    return {
        stamp: func() {return time_stamp(stamp);},
        elapsedMSec: func() {return elapsed_millisecond(stamp);},
        elapsedUSec: func() {return elapsed_microsecond(stamp);}
    };
}

# md5
var md5 = func(str) {
    return __md5(str);
}

# important global constants
var D2R = 3.14159265358979323846264338327950288 / 180; # degree to radian
var R2D = 180 / 3.14159265358979323846264338327950288; # radian to degree

var FT2M = 0.3048;               # feet to meter
var M2FT = 1 / FT2M;
var IN2M = FT2M / 12;
var M2IN = 1 / IN2M;
var NM2M = 1852;                 # nautical miles to meter
var M2NM = 1 / NM2M;

var KT2MPS = 0.5144444444;       # knots to m/s
var MPS2KT = 1 / KT2MPS;

var FPS2KT = 0.5924838012958964; # fps to knots
var KT2FPS = 1 / FPS2KT;

var LB2KG = 0.45359237;          # pounds to kg
var KG2LB = 1 / LB2KG;

var GAL2L = 3.785411784;         # US gallons to liter
var L2GAL = 1 / GAL2L;

# functions that not supported in this runtime:
var bind = func(function, locals, outer_scope = nil) {
    die("this runtime does not support bind");
}

var call = func(function ,args = nil, _me = nil, locals = nil, error = nil) {
    die("this runtime does not support call");
}

var caller = func(level = 1) {
    die("this runtime does not support caller");
}

var closure = func(function, level = 1) {
    die("this runtime uses \"vm_upval\" instead of \"vm_hash\" as the closure");
}

var compile = func(code, filename = "<compile>") {
    die("this runtime uses static code generator");
}

# for log print
var LOG_BULK = 1;
var LOG_DEBUG = 2;
var LOG_INFO = 3;
var LOG_WARN = 4;
var LOG_ALERT = 5;
var DEV_WARN = 7;
var DEV_ALERT = 8;
var MANDATORY_INFO = 9;

var logprint = func(level, elem...) {
    return _logprint(level, elem);
}

var fgcommand = func(cmd, node=nil) {
    # if (isa(node, props.Node)) node = node._g;
    # elsif (ishash(node)) node = props.Node.new(node)._g;
    # _fgcommand(cmd, node);
    println("in progress, not supported yet.");
}