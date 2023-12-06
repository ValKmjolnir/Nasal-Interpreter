use std.math;

var mat = func(width,height) {
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

var rand_init = func(a) {
    var ref=a.mat;
    forindex(var i;ref) {
        ref[i]=rand()*2-1;
    }
}

var mat_print = func(a) {
    var (width,height,ref)=(a.width,a.height,a.mat);
    for(var i=0;i<height;i+=1) {
        for(var j=0;j<width;j+=1) {
            print(ref[i*width+j]," ");
        }
        println();
    }
}

var add = func(a,b) {
    if (a.width!=b.width or a.height!=b.height) {
        println("matrix a: ",a);
        println("matrix b: ",b);
        die("width and height must be the same");
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

var sub = func(a,b) {
    if (a.width!=b.width or a.height!=b.height) {
        println("matrix a: ",a);
        println("matrix b: ",b);
        die("width and height must be the same");
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

var hardamard = func(a,b) {
    if (a.width!=b.width or a.height!=b.height) {
        println("matrix a: ",a);
        println("matrix b: ",b);
        die("width and height must be the same");
        return nil;
    }

    var res=mat(a.width,a.height);
    var (width,height,ref)=(res.width,res.height,res.mat);
    var (aref,bref)=(a.mat,b.mat);

    for(var i=0;i<height;i+=1) {
        for(var j=0;j<width;j+=1) {
            ref[i*width+j]=aref[i*width+j]*bref[i*width+j];
        }
    }
    return res;
}

var neg = func(a) {
    var res=mat(a.width,a.height);
    var (aref,ref)=(a.mat,res.mat);
    forindex(var i;aref) {
        ref[i]=-aref[i];
    }
    return res;
}

var sum = func(a) {
    var res=0;
    var aref=a.mat;
    forindex(var i;aref) {
        res+=aref[i];
    }
    return res;
}

var mult_num = func(a,c) {
    var res=mat(a.width,a.height);
    var ref=res.mat;
    var aref=a.mat;
    forindex(var i;aref) {
        ref[i]=aref[i]*c;
    }
    return res;
}

var trans = func(a) {
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

var activate = func(a,f) {
    var res=mat(a.width,a.height);
    var (aref,ref)=(a.mat,res.mat);
    forindex(var i;aref) {
        ref[i]=f(aref[i]);
    }
    return res;
}

var mult = func(a,b) {
    if (a.width!=b.height) {
        println("matrix a: ",a);
        println("matrix b: ",b);
        die("a.width must equal to b.height, but get a.width:"~str(a.width)~" and b.height"~str(b.height));
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

var sigmoid = func(x) {
    var t=math.exp(-x);
    return 1/(1+t);
}

var diffsigmoid = func(x) {
    x=sigmoid(x);
    return x*(1-x);
}

var tanh = func(x) {
    var t1=math.exp(x);
    var t2=math.exp(-x);
    return (t1-t2)/(t1+t2);
}

var difftanh = func(x) {
    x=tanh(x);
    return 1-x*x;
}

var bp_example = func() {
    srand();
    var lr=0.01;
    var input=[
        {width:2,height:1,mat:[0,0]},
        {width:2,height:1,mat:[0,1]},
        {width:2,height:1,mat:[1,0]},
        {width:2,height:1,mat:[1,1]}
    ];
    # last 2 column is useless, only used to make sure bp runs correctly
    var expect=[
        {width:3,height:1,mat:[0,0,0]},
        {width:3,height:1,mat:[1,0,1]},
        {width:3,height:1,mat:[1,1,0]},
        {width:3,height:1,mat:[0,1,1]}
    ];
    var hidden={
        weight:mat(4,2),
        bias:mat(4,1),
        in:nil,
        out:nil,
        diff:nil
    };
    var output={
        weight:mat(3,4),
        bias:mat(3,1),
        in:nil,
        out:nil,
        diff:nil
    };
    rand_init(hidden.weight);
    rand_init(hidden.bias);
    rand_init(output.weight);
    rand_init(output.bias);

    var epoch=0;
    var total=1e6;
    while(total>0.001) {
        epoch+=1;
        if (epoch>1e4) {
            println("Training failed after ",epoch," epoch.");
            break;
        }

        total=0;
        forindex(var i;input) {
            hidden.in=add(mult(input[i],hidden.weight),hidden.bias);
            hidden.out=activate(hidden.in,tanh);

            output.in=add(mult(hidden.out,output.weight),output.bias);
            output.out=activate(output.in,sigmoid);

            var error=sub(expect[i],output.out);

            output.diff=hardamard(error,activate(output.in,diffsigmoid));
            hidden.diff=hardamard(trans(mult(output.weight,trans(output.diff))),activate(hidden.in,difftanh));

            output.bias=add(output.bias,output.diff);
            hidden.bias=add(hidden.bias,hidden.diff);

            output.weight=add(output.weight,mult(trans(hidden.out),output.diff));
            hidden.weight=add(hidden.weight,mult(trans(input[i]),hidden.diff));

            total+=sum(mult_num(mult(error,trans(error)),0.5));
        }
    }
    if (epoch<=1e4) {
        println("Training succeeded after ",epoch," epoch.");
    }

    forindex(var i;input) {
        hidden.in=add(mult(input[i],hidden.weight),hidden.bias);
        hidden.out=activate(hidden.in,tanh);

        output.in=add(mult(hidden.out,output.weight),output.bias);
        output.out=activate(output.in,sigmoid);

        println(input[i].mat," : ",output.out.mat);
    }
}