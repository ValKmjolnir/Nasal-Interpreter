
import("lib.nas");
var ResultTrait={
    Ok:func(val){
        me.ok=val;
        me.flag=0;
        return me;
    },
    Err:func(info){
        me.err=info;
        me.flag=1;
        return me;
    },
    unwrap:func(){
        if(me.flag)
            die(me.err);
        return me.ok;
    }
};

var Result=func(){
    return{
        ok:nil,
        err:"",
        flag:1,
        parents:[ResultTrait]
    };
};