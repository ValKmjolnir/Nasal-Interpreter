#include "math_lib.h"

namespace nasal {

var builtin_pow(context* ctx, gc* ngc) {
    auto x = ctx->localr[1];
    auto y = ctx->localr[2];
    if (x.type!=vm_type::vm_num || y.type!=vm_type::vm_num) {
        return var::num(std::nan(""));
    }
    return var::num(std::pow(x.num(), y.num()));
}

var builtin_sin(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    return var::num(val.type==vm_type::vm_num? sin(val.num()):std::nan(""));
}

var builtin_cos(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    return var::num(val.type==vm_type::vm_num? cos(val.num()):std::nan(""));
}

var builtin_tan(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    return var::num(val.type==vm_type::vm_num? tan(val.num()):std::nan(""));
}

var builtin_exp(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    return var::num(val.type==vm_type::vm_num? exp(val.num()):std::nan(""));
}

var builtin_lg(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    return var::num(val.type==vm_type::vm_num? log(val.num())/log(10.0):std::nan(""));
}

var builtin_ln(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    return var::num(val.type==vm_type::vm_num? log(val.num()):std::nan(""));
}

var builtin_sqrt(context* ctx, gc* ngc) {
    auto val = ctx->localr[1];
    return var::num(val.type==vm_type::vm_num? sqrt(val.num()):std::nan(""));
}

var builtin_atan2(context* ctx, gc* ngc) {
    auto x = ctx->localr[1];
    auto y = ctx->localr[2];
    if (x.type!=vm_type::vm_num || y.type!=vm_type::vm_num) {
        return var::num(std::nan(""));
    }
    return var::num(atan2(y.num(), x.num()));
}

var builtin_isnan(context* ctx, gc* ngc) {
    auto x = ctx->localr[1];
    return (x.type==vm_type::vm_num && std::isnan(x.num()))? one:zero;
}

nasal_builtin_table math_lib_native[] = {
    {"__pow", builtin_pow},
    {"__sin", builtin_sin},
    {"__cos", builtin_cos},
    {"__tan", builtin_tan},
    {"__exp", builtin_exp},
    {"__lg", builtin_lg},
    {"__ln", builtin_ln},
    {"__sqrt", builtin_sqrt},
    {"__atan2", builtin_atan2},
    {"__isnan", builtin_isnan},
    {nullptr, nullptr}
};

}
