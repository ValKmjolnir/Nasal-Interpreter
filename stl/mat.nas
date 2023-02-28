
var mat=func(width,height) {
    var res=[];
    setsize(res,width*height);
    forindex(var i;res) {
        res[i]=0;
    }
    return {
        width:width,
        height:height,
        mat:res
    };
}

var rand_init=func(a) {
    srand();
    var ref=a.mat;
    forindex(var i;ref) {
        ref[i]=rand();
    }
}

var mat_print=func(a) {
    var (width,height,ref)=(a.width,a.height,a.mat);
    for(var i=0;i<height;i+=1) {
        for(var j=0;j<width;j+=1) {
            print(ref[i*width+j]," ");
        }
        println();
    }
}

var add=func(a,b) {
    if(a.width!=b.width or a.height!=b.height) {
        return nil;
    }

    var res=mat(a.width,a.height);
    var (width,height,ref)=(res.width,res.height,res.mat);
    var (aref,bref)=(a.mat,b.mat);

    for(var i=0;i<height;i+=1) {
        for(var j=0;j<width;j+=1) {
            ref[i*width+j]=aref[i*width+j]+bref[i*width+j];
        }
    }
    return res;
}

var sub=func(a,b) {
    if(a.width!=b.width or a.height!=b.height) {
        return nil;
    }

    var res=mat(a.width,a.height);
    var (width,height,ref)=(res.width,res.height,res.mat);
    var (aref,bref)=(a.mat,b.mat);

    for(var i=0;i<height;i+=1) {
        for(var j=0;j<width;j+=1) {
            ref[i*width+j]=aref[i*width+j]-bref[i*width+j];
        }
    }
    return res;
}

var neg=func(a) {
    var res=mat(a.width,a.height);
    var (aref,ref)=(a.mat,res.mat);
    forindex(var i;aref) {
        ref[i]=-aref[i];
    }
    return res;
}

var trans=func(a) {
    var res=mat(a.height,a.width);
    var ref=res.mat;
    var (a_width,a_height,aref)=(a.width,a.height,a.mat);
    for(var i=0;i<a_width;i+=1) {
        for(var j=0;j<a_height;j+=1) {
            ref[i*a_height+j]=aref[j*a_width+i];
        }
    }
    return res;
}

var activate=func(a,f) {
    var res=mat(a.width,a.height);
    var (aref,ref)=(a.mat,res.mat);
    forindex(var i;aref) {
        ref[i]=f(aref[i]);
    }
    return res;
}

var mult=func(a,b) {
    if(a.width!=b.height) {
        return nil;
    }

    var res=mat(b.width,a.height);
    var (res_width,res_height,ref)=(res.width,res.height,res.mat);
    var (a_width,aref)=(a.width,a.mat);
    var (b_width,bref)=(b.width,b.mat);

    for(var i=0;i<res_width;i+=1) {
        for(var j=0;j<res_height;j+=1) {
            for(var k=0;k<a_width;k+=1) {
                ref[j*res_width+i]+=aref[j*a_width+k]*bref[k*b_width+i];
            }
        }
    }
    return res;
}

var sigmoid=func(x) {
    var t=math.exp(-x);
    return 1/(1+t);
}

var tanh=func(x) {
    var t1=math.exp(x);
    var t2=math.exp(-x);
    return (t1-t2)/(t1+t2);
}

var test=func() {
    for(var i=0;i<1e4;i+=1) {
        var a=mat(4,20);
        var b=mat(10,4);
        var c=a;
        rand_init(a);
        rand_init(b);
        mult(a,b);
        add(a,c);
        sub(a,c);
        neg(a);
        trans(a);
        activate(a,sigmoid);
        activate(a,tanh);
        if(i-int(i/100)*100==0) {
            print(i/1e4*100,"%   \r");
        }
    }
}

test();