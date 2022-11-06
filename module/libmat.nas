var libmat=func(){
    var dl=dylib.dlopen("libmat."~(os.platform()=="windows"?"dll":"so"));
    var vec2=dylib.dlsym(dl,"nas_vec2");
    var vec3=dylib.dlsym(dl,"nas_vec3");
    var (vec2add,vec2sub,vec2mul,vec2div,vec2neg,vec2norm,vec2len,vec2dot)=(
        dylib.dlsym(dl,"nas_vec2_add"),
        dylib.dlsym(dl,"nas_vec2_sub"),
        dylib.dlsym(dl,"nas_vec2_mult"),
        dylib.dlsym(dl,"nas_vec2_div"),
        dylib.dlsym(dl,"nas_vec2_neg"),
        dylib.dlsym(dl,"nas_vec2_norm"),
        dylib.dlsym(dl,"nas_vec2_len"),
        dylib.dlsym(dl,"nas_vec2_dot")
    );
    var (vec3add,vec3sub,vec3mul,vec3div,vec3neg,vec3norm,vec3len,vec3dot)=(
        dylib.dlsym(dl,"nas_vec3_add"),
        dylib.dlsym(dl,"nas_vec3_sub"),
        dylib.dlsym(dl,"nas_vec3_mult"),
        dylib.dlsym(dl,"nas_vec3_div"),
        dylib.dlsym(dl,"nas_vec3_neg"),
        dylib.dlsym(dl,"nas_vec3_norm"),
        dylib.dlsym(dl,"nas_vec3_len"),
        dylib.dlsym(dl,"nas_vec3_dot")
    );
    var (rotate_x,rotate_y,rotate_z)=(
        dylib.dlsym(dl,"nas_rotate_x"),
        dylib.dlsym(dl,"nas_rotate_y"),
        dylib.dlsym(dl,"nas_rotate_z")
    );
    var call=dylib.dlcall;
    return {
        vec2:{
            new:func(x,y){return call(vec2,x,y);},
            add:func(v0,v1){return call(vec2add,v0,v1);},
            sub:func(v0,v1){return call(vec2sub,v0,v1);},
            mul:func(v0,v1){return call(vec2mul,v0,v1);},
            div:func(v0,v1){return call(vec2div,v0,v1);},
            neg:func(v0){return call(vec2neg,v0);},
            norm:func(v0){return call(vec2norm,v0);},
            len:func(v0){return call(vec2len,v0);},
            dot:func(v0,v1){return call(vec2dot,v0,v1);}
        },
        vec3:{
            new:func(x,y,z){return call(vec3,x,y,z);},
            add:func(v0,v1){return call(vec3add,v0,v1);},
            sub:func(v0,v1){return call(vec3sub,v0,v1);},
            mul:func(v0,v1){return call(vec3mul,v0,v1);},
            div:func(v0,v1){return call(vec3div,v0,v1);},
            neg:func(v0){return call(vec3neg,v0);},
            norm:func(v0){return call(vec3norm,v0);},
            len:func(v0){return call(vec3len,v0);},
            rx:func(v0,angle){return call(rotate_x,v0,angle);},
            ry:func(v0,angle){return call(rotate_y,v0,angle);},
            rz:func(v0,angle){return call(rotate_z,v0,angle);},
            dot:func(v0,v1){return call(vec3dot,v0,v1);}
        }
    };
}();