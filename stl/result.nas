# result.nas
# ValKmjolnir 2021

var Result=func(){
    var (ok,err,flag)=(nil,"",1);
    return{
        Ok:func(val){
            ok=val;
            flag=0;
            return me;
        },
        Err:func(info){
            err=info;
            flag=1;
            return me;
        },
        unwrap:func(){
            if(flag)
                die(err);
            return ok;
        }
    };
};