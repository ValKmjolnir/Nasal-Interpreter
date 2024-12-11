#include "../src/nasal.h"
#include "../src/nasal_type.h"
#include "../src/nasal_gc.h"
#include <cmath>

namespace nasal {

var nas_vec2(var* args, usize size, gc* ngc) {
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(args[0]);
    res.vec().elems.push_back(args[1]);
    return res;
}

var nas_vec3(var* args, usize size, gc* ngc) {
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(args[0]);
    res.vec().elems.push_back(args[1]);
    res.vec().elems.push_back(args[2]);
    return res;
}

var nas_vec2_add(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=2 || v1.size()!=2)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()+v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()+v1[1].num()));
    return res;
}

var nas_vec2_sub(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=2 || v1.size()!=2)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()-v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()-v1[1].num()));
    return res;
}

var nas_vec2_mult(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=2 || v1.size()!=2)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()*v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()*v1[1].num()));
    return res;
}

var nas_vec2_div(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=2 || v1.size()!=2)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()/v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()/v1[1].num()));
    return res;
}

var nas_vec2_neg(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=2)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(-v0[0].num()));
    res.vec().elems.push_back(var::num(-v0[1].num()));
    return res;
}

var nas_vec2_norm(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=2)
        return nil;
    auto x = v0[0].num();
    auto y = v0[1].num();
    auto t = std::sqrt(x*x+y*y);
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(x/t));
    res.vec().elems.push_back(var::num(y/t));
    return res;
}

var nas_vec2_len(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=2)
        return nil;
    auto x = v0[0].num();
    auto y = v0[1].num();
    return var::num(std::sqrt(x*x+y*y));
}

var nas_vec2_dot(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=2 || v1.size()!=2)
        return nil;
    return var::num(v0[0].num()*v1[0].num()+v0[1].num()*v1[1].num());
}

var nas_vec3_add(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=3 || v1.size()!=3)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()+v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()+v1[1].num()));
    res.vec().elems.push_back(var::num(v0[2].num()+v1[2].num()));
    return res;
}

var nas_vec3_sub(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=3 || v1.size()!=3)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()-v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()-v1[1].num()));
    res.vec().elems.push_back(var::num(v0[2].num()-v1[2].num()));
    return res;
}

var nas_vec3_mult(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=3 || v1.size()!=3)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()*v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()*v1[1].num()));
    res.vec().elems.push_back(var::num(v0[2].num()*v1[2].num()));
    return res;
}

var nas_vec3_div(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=3 || v1.size()!=3)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()/v1[0].num()));
    res.vec().elems.push_back(var::num(v0[1].num()/v1[1].num()));
    res.vec().elems.push_back(var::num(v0[2].num()/v1[2].num()));
    return res;
}

var nas_vec3_neg(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=3)
        return nil;
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(-v0[0].num()));
    res.vec().elems.push_back(var::num(-v0[1].num()));
    res.vec().elems.push_back(var::num(-v0[2].num()));
    return res;
}

var nas_vec3_norm(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=3)
        return nil;
    auto x = v0[0].num();
    auto y = v0[1].num();
    auto z = v0[2].num();
    auto t = std::sqrt(x*x+y*y+z*z);
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(x/t));
    res.vec().elems.push_back(var::num(y/t));
    res.vec().elems.push_back(var::num(z/t));
    return res;
}

var nas_vec3_len(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=3)
        return nil;
    auto x = v0[0].num();
    auto y = v0[1].num();
    auto z = v0[2].num();
    return var::num(std::sqrt(x*x+y*y+z*z));
}

var nas_rotate_x(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=3)
        return nil;
    auto angle = args[1].num();
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()));
    res.vec().elems.push_back(var::num(v0[2].num()*std::sin(angle)+v0[1].num()*std::cos(angle)));
    res.vec().elems.push_back(var::num(v0[2].num()*std::cos(angle)-v0[1].num()*std::sin(angle)));
    return res;
}

var nas_rotate_y(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=3)
        return nil;
    auto angle = args[1].num();
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()*std::cos(angle)-v0[2].num()*std::sin(angle)));
    res.vec().elems.push_back(var::num(v0[1].num()));
    res.vec().elems.push_back(var::num(v0[0].num()*std::sin(angle)+v0[2].num()*std::cos(angle)));
    return res;
}

var nas_rotate_z(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    if (v0.size()!=3)
        return nil;
    auto angle = args[1].num();
    var res = ngc->alloc(vm_type::vm_vec);
    res.vec().elems.push_back(var::num(v0[0].num()*std::cos(angle)-v0[1].num()*std::sin(angle)));
    res.vec().elems.push_back(var::num(v0[0].num()*std::sin(angle)+v0[1].num()*std::cos(angle)));
    res.vec().elems.push_back(var::num(v0[2].num()));
    return res;
}

var nas_vec3_dot(var* args, usize size, gc* ngc) {
    if (!args[0].is_vec() || !args[1].is_vec())
        return nil;
    auto& v0 = args[0].vec().elems;
    auto& v1 = args[1].vec().elems;
    if (v0.size()!=3 || v1.size()!=3)
        return nil;
    return var::num(v0[0].num()*v1[0].num()+v0[1].num()*v1[1].num()+v0[2].num()*v1[2].num());
}

module_func_info func_tbl[] = {
    {"nas_vec2", nas_vec2},
    {"nas_vec2_add", nas_vec2_add},
    {"nas_vec2_sub", nas_vec2_sub},
    {"nas_vec2_mult", nas_vec2_mult},
    {"nas_vec2_div", nas_vec2_div},
    {"nas_vec2_neg", nas_vec2_neg},
    {"nas_vec2_norm", nas_vec2_norm},
    {"nas_vec2_len", nas_vec2_len},
    {"nas_vec2_dot", nas_vec2_dot},
    {"nas_vec3", nas_vec3},
    {"nas_vec3_add", nas_vec3_add},
    {"nas_vec3_sub", nas_vec3_sub},
    {"nas_vec3_mult", nas_vec3_mult},
    {"nas_vec3_div", nas_vec3_div},
    {"nas_vec3_neg", nas_vec3_neg},
    {"nas_vec3_norm", nas_vec3_norm},
    {"nas_vec3_len", nas_vec3_len},
    {"nas_rotate_x", nas_rotate_x},
    {"nas_rotate_y", nas_rotate_y},
    {"nas_rotate_z", nas_rotate_z},
    {"nas_vec3_dot", nas_vec3_dot},
    {nullptr, nullptr}
};

NASAL_EXPORT module_func_info* get() {
    return func_tbl;
}

}