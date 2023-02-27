#pragma once

#include <cmath>

#include "nasal_ast.h"

void const_str(ast& root) {
    auto& vec=root.child();
    root.set_str(vec[0].str()+vec[1].str());
    root.child().clear();
    root.set_type(ast_str);
}

void const_num(ast& root) {
    auto& vec=root.child();
    f64 res=0;
    switch(root.type()) {
        case ast_add:    res=vec[0].num()+vec[1].num(); break;
        case ast_sub:    res=vec[0].num()-vec[1].num(); break;
        case ast_mult:   res=vec[0].num()*vec[1].num(); break;
        case ast_div:    res=vec[0].num()/vec[1].num(); break;
        case ast_less:   res=vec[0].num()<vec[1].num(); break;
        case ast_leq:    res=vec[0].num()<=vec[1].num();break;
        case ast_grt:    res=vec[0].num()>vec[1].num(); break;
        case ast_geq:    res=vec[0].num()>=vec[1].num();break;
        case ast_bitor:  res=i32(vec[0].num())|i32(vec[1].num()); break;
        case ast_bitxor: res=i32(vec[0].num())^i32(vec[1].num()); break;
        case ast_bitand: res=i32(vec[0].num())&i32(vec[1].num()); break;
    }
    // inf and nan will cause number hashmap error in codegen
    if (std::isinf(res) || std::isnan(res)) {
        return;
    }
    root.set_num(res);
    root.child().clear();
    root.set_type(ast_num);
}

void calc_const(ast& root) {
    auto& vec=root.child();
    for(auto& i:vec) {
        calc_const(i);
    }
    if (vec.size()==1 && root.type()==ast_neg && vec[0].type()==ast_num) {
        f64 res=-vec[0].num();
        root.set_num(res);
        root.child().clear();
        root.set_type(ast_num);
        return;
    }
    if (vec.size()!=2) {
        return;
    }
    if (root.type()!=ast_add && root.type()!=ast_sub &&
       root.type()!=ast_mult && root.type()!=ast_div &&
       root.type()!=ast_link && root.type()!=ast_less &&
       root.type()!=ast_leq && root.type()!=ast_grt &&
       root.type()!=ast_geq && root.type()!=ast_bitor &&
       root.type()!=ast_bitxor && root.type()!=ast_bitand) {
        return;
    }
    if (root.type()==ast_link &&
       vec[0].type()==ast_str && vec[1].type()==ast_str) {
        const_str(root);
    } else if (root.type()!=ast_link &&
            vec[0].type()==ast_num && vec[1].type()==ast_num) {
        const_num(root);
    }
}

void optimize(ast& root) {
    for(auto& i:root.child()) {
        calc_const(i);
    }
}
