# module json
# 2023/11/27 ValKmjolnir

use std.dylib;

var _dynamic_lib = dylib.dlopen("libjson."~(os.platform()=="windows"?"dll":"so"));
var _stringify = _dynamic_lib.stringify;
var _parse = _dynamic_lib.parse;
var _call = dylib.limitcall(1);

var stringify = func(object) {
    return _call(_stringify, object);
}

var parse = func(input_string) {
    return _call(_parse, input_string);
}