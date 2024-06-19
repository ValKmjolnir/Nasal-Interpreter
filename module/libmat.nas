use std.dylib;
use std.os;

var _dl = dylib.dlopen("libmat");

var _vec2 = _dl.nas_vec2;

var _vec3 = _dl.nas_vec3;

var (_vec2add, _vec2sub, _vec2mul, _vec2div, _vec2neg, _vec2norm, _vec2len, _vec2dot)=(
    _dl.nas_vec2_add,
    _dl.nas_vec2_sub,
    _dl.nas_vec2_mult,
    _dl.nas_vec2_div,
    _dl.nas_vec2_neg,
    _dl.nas_vec2_norm,
    _dl.nas_vec2_len,
    _dl.nas_vec2_dot
);
var (_vec3add, _vec3sub, _vec3mul, _vec3div, _vec3neg, _vec3norm, _vec3len, _vec3dot)=(
    _dl.nas_vec3_add,
    _dl.nas_vec3_sub,
    _dl.nas_vec3_mult,
    _dl.nas_vec3_div,
    _dl.nas_vec3_neg,
    _dl.nas_vec3_norm,
    _dl.nas_vec3_len,
    _dl.nas_vec3_dot
);
var (_rotate_x, _rotate_y, _rotate_z)=(
    _dl.nas_rotate_x,
    _dl.nas_rotate_y,
    _dl.nas_rotate_z
);
var (_invoke_i, _invoke_ii, _invoke_iii)=(
    dylib.limitcall(1),
    dylib.limitcall(2),
    dylib.limitcall(3)
);

var vec2 = {
    new:  func(x, y) {return _invoke_ii(_vec2, x, y);},
    add:  func(v0, v1) {return _invoke_ii(_vec2add, v0, v1);},
    sub:  func(v0, v1) {return _invoke_ii(_vec2sub, v0, v1);},
    mul:  func(v0, v1) {return _invoke_ii(_vec2mul, v0, v1);},
    div:  func(v0, v1) {return _invoke_ii(_vec2div, v0, v1);},
    neg:  func(v0) {return _invoke_i(_vec2neg, v0);},
    norm: func(v0) {return _invoke_i(_vec2norm, v0);},
    len:  func(v0) {return _invoke_i(_vec2len, v0);},
    dot:  func(v0, v1) {return _invoke_ii(_vec2dot, v0, v1);}
};

var vec3 = {
    new:  func(x, y, z) {return _invoke_iii(_vec3, x, y, z);},
    add:  func(v0, v1) {return _invoke_ii(_vec3add, v0, v1);},
    sub:  func(v0, v1) {return _invoke_ii(_vec3sub, v0, v1);},
    mul:  func(v0, v1) {return _invoke_ii(_vec3mul, v0, v1);},
    div:  func(v0, v1) {return _invoke_ii(_vec3div, v0, v1);},
    neg:  func(v0) {return _invoke_i(_vec3neg, v0);},
    norm: func(v0) {return _invoke_i(_vec3norm, v0);},
    len:  func(v0) {return _invoke_i(_vec3len, v0);},
    rx:   func(v0, angle) {return _invoke_ii(_rotate_x, v0, angle);},
    ry:   func(v0, angle) {return _invoke_ii(_rotate_y, v0, angle);},
    rz:   func(v0, angle) {return _invoke_ii(_rotate_z, v0, angle);},
    dot:  func(v0, v1) {return _invoke_ii(_vec3dot, v0, v1);}
};

