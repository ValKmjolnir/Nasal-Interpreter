#include "../nasal.h"
#include <cmath>

var nas_vec2(var* args,usize size,gc* ngc){
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back(args[0]);
    res.vec().elems.push_back(args[1]);
    return res;
}

var nas_vec3(var* args,usize size,gc* ngc){
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back(args[0]);
    res.vec().elems.push_back(args[1]);
    res.vec().elems.push_back(args[2]);
    return res;
}

var nas_vec2_add(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=2 || v1.size()!=2)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()+v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()+v1[1].num()});
    return res;
}

var nas_vec2_sub(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=2 || v1.size()!=2)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()-v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()-v1[1].num()});
    return res;
}

var nas_vec2_mult(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=2 || v1.size()!=2)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()*v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()*v1[1].num()});
    return res;
}

var nas_vec2_div(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=2 || v1.size()!=2)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()/v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()/v1[1].num()});
    return res;
}

var nas_vec2_neg(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=2)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,-v0[0].num()});
    res.vec().elems.push_back({vm_num,-v0[1].num()});
    return res;
}

var nas_vec2_norm(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=2)
        return nil;
    auto x=v0[0].num();
    auto y=v0[1].num();
    auto t=std::sqrt(x*x+y*y);
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,x/t});
    res.vec().elems.push_back({vm_num,y/t});
    return res;
}

var nas_vec2_len(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=2)
        return nil;
    auto x=v0[0].num();
    auto y=v0[1].num();
    return {vm_num,std::sqrt(x*x+y*y)};
}

var nas_vec2_dot(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=2 || v1.size()!=2)
        return nil;
    return {vm_num,v0[0].num()*v1[0].num()+v0[1].num()*v1[1].num()};
}

var nas_vec3_add(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=3 || v1.size()!=3)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()+v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()+v1[1].num()});
    res.vec().elems.push_back({vm_num,v0[2].num()+v1[2].num()});
    return res;
}

var nas_vec3_sub(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=3 || v1.size()!=3)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()-v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()-v1[1].num()});
    res.vec().elems.push_back({vm_num,v0[2].num()-v1[2].num()});
    return res;
}

var nas_vec3_mult(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=3 || v1.size()!=3)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()*v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()*v1[1].num()});
    res.vec().elems.push_back({vm_num,v0[2].num()*v1[2].num()});
    return res;
}

var nas_vec3_div(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=3 || v1.size()!=3)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()/v1[0].num()});
    res.vec().elems.push_back({vm_num,v0[1].num()/v1[1].num()});
    res.vec().elems.push_back({vm_num,v0[2].num()/v1[2].num()});
    return res;
}

var nas_vec3_neg(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=3)
        return nil;
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,-v0[0].num()});
    res.vec().elems.push_back({vm_num,-v0[1].num()});
    res.vec().elems.push_back({vm_num,-v0[2].num()});
    return res;
}

var nas_vec3_norm(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=3)
        return nil;
    auto x=v0[0].num();
    auto y=v0[1].num();
    auto z=v0[2].num();
    auto t=std::sqrt(x*x+y*y+z*z);
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,x/t});
    res.vec().elems.push_back({vm_num,y/t});
    res.vec().elems.push_back({vm_num,z/t});
    return res;
}

var nas_vec3_len(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=3)
        return nil;
    auto x=v0[0].num();
    auto y=v0[1].num();
    auto z=v0[2].num();
    return {vm_num,std::sqrt(x*x+y*y+z*z)};
}

var nas_rotate_x(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=3)
        return nil;
    auto angle=args[1].num();
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()});
    res.vec().elems.push_back({vm_num,v0[2].num()*std::sin(angle)+v0[1].num()*std::cos(angle)});
    res.vec().elems.push_back({vm_num,v0[2].num()*std::cos(angle)-v0[1].num()*std::sin(angle)});
    return res;
}

var nas_rotate_y(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=3)
        return nil;
    auto angle=args[1].num();
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()*std::cos(angle)-v0[2].num()*std::sin(angle)});
    res.vec().elems.push_back({vm_num,v0[1].num()});
    res.vec().elems.push_back({vm_num,v0[0].num()*std::sin(angle)+v0[2].num()*std::cos(angle)});
    return res;
}

var nas_rotate_z(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    if(v0.size()!=3)
        return nil;
    auto angle=args[1].num();
    var res=ngc->alloc(vm_vec);
    res.vec().elems.push_back({vm_num,v0[0].num()*std::cos(angle)-v0[1].num()*std::sin(angle)});
    res.vec().elems.push_back({vm_num,v0[0].num()*std::sin(angle)+v0[1].num()*std::cos(angle)});
    res.vec().elems.push_back({vm_num,v0[2].num()});
    return res;
}

var nas_vec3_dot(var* args,usize size,gc* ngc){
    if(args[0].type!=vm_vec || args[1].type!=vm_vec)
        return nil;
    auto& v0=args[0].vec().elems;
    auto& v1=args[1].vec().elems;
    if(v0.size()!=3 || v1.size()!=3)
        return nil;
    return {vm_num,v0[0].num()*v1[0].num()+v0[1].num()*v1[1].num()+v0[2].num()*v1[2].num()};
}

extern "C" mod get(const char* n){
    string name=n;
    if(name=="nas_vec2")
        return nas_vec2;
    else if(name=="nas_vec2_add")
        return nas_vec2_add;
    else if(name=="nas_vec2_sub")
        return nas_vec2_sub;
    else if(name=="nas_vec2_mult")
        return nas_vec2_mult;
    else if(name=="nas_vec2_div")
        return nas_vec2_div;
    else if(name=="nas_vec2_neg")
        return nas_vec2_neg;
    else if(name=="nas_vec2_norm")
        return nas_vec2_norm;
    else if(name=="nas_vec2_len")
        return nas_vec2_len;
    else if(name=="nas_vec2_dot")
        return nas_vec2_dot;
    else if(name=="nas_vec3")
        return nas_vec3;
    else if(name=="nas_vec3_add")
        return nas_vec3_add;
    else if(name=="nas_vec3_sub")
        return nas_vec3_sub;
    else if(name=="nas_vec3_mult")
        return nas_vec3_mult;
    else if(name=="nas_vec3_div")
        return nas_vec3_div;
    else if(name=="nas_vec3_neg")
        return nas_vec3_neg;
    else if(name=="nas_vec3_norm")
        return nas_vec3_norm;
    else if(name=="nas_vec3_len")
        return nas_vec3_len;
    else if(name=="nas_rotate_x")
        return nas_rotate_x;
    else if(name=="nas_rotate_y")
        return nas_rotate_y;
    else if(name=="nas_rotate_z")
        return nas_rotate_z;
    else if(name=="nas_vec3_dot")
        return nas_vec3_dot;
    return nullptr;
}