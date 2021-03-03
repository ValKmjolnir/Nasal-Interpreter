import("lib.nas");
var activate_function=
{
    sigmoid_func:func(x)
    {
        return 1.0/(1.0+math.exp(-x));
    },
    diffsigmoid_func:func(x)
    {
        var t=func(x){return 1.0/(1.0+math.exp(-x));}(x);
        return t*(1-t);
    },
    tanh_func:func(x)
    {
        var t1=math.exp(x);
        var t2=math.exp(-x);
        return (t1-t2)/(t1+t2);
    },
    difftanh_func:func(x)
    {
        var t1=math.exp(x);
        var t2=math.exp(-x);
        var t=(t1-t2)/(t1+t2);
        return 1-t*t;
    },
    relu_func:func(x)
    {
        return x>0? x:0;
    },
    diffrelu_func:func(x)
    {
        return x>0;
    },
    leaky_relu_func:func(k,x)
    {
        return x>0? x:k*x;
    },
    diffleaky_relu_func:func(k,x)
    {
        return x>0? 1:k;
    }
};

var matrix=
{
    new:func(col,row)
    {
        var new_mat=
        {
            col:col,
            row:row,
            mat:[]
        };
        for(var i=0;i<row;i+=1)
        {
            append(new_mat.mat,[]);
            for(var j=0;j<col;j+=1)
                append(new_mat.mat[i],nil);
        }
        return new_mat;
    },
    srand:func(x)
    {
        rand(x);
        return nil;
    },
    rand_init:func(mat)
    {
        for(var i=0;i<mat.row;i+=1)
            for(var j=0;j<mat.col;j+=1)
            {
                if(rand()>0.5)
                    mat.mat[i][j]=-rand();
                else
                    mat.mat[i][j]=rand();
            }
        return;
    },
    prt_mat:func(mat)
    {
        var prt_s='[\n';
        foreach(var i;mat.mat)
        {
            var s='[';
            foreach(var j;i)
                s~=(j~',');
            s~='],\n';
            prt_s~=s;
        }
        prt_s~=']';
        print(prt_s);
        return nil;
    },
    mult_mat:func(mat1,mat2)
    {
        if(mat1.col!=mat2.row)
        {
            die("[error-mult] mat1\'s col does not match mat2\'s row.");
            return nil;
        }
        var new_mat=me.new(mat2.col,mat1.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
            {
                var sum=0;
                for(var k=0;k<mat1.col;k+=1)
                    sum+=mat1.mat[i][k]*mat2.mat[k][j];
                new_mat.mat[i][j]=sum;
            }
        return new_mat;
    },
    add_mat:func(mat1,mat2)
    {
        if(mat1.col!=mat2.col or mat1.row!=mat2.row)
        {
            die("[error-add] mat1\'s col or row does not match mat2\'s.");
            return nil;
        }
        var new_mat=me.new(mat1.col,mat1.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=mat1.mat[i][j]+mat2.mat[i][j];
        return new_mat;
    },
    sub_mat:func(mat1,mat2)
    {
        if(mat1.col!=mat2.col or mat1.row!=mat2.row)
        {
            die("[error-sub] mat1\'s col or row does not match mat2\'s.");
            return nil;
        }
        var new_mat=me.new(mat1.col,mat1.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=mat1.mat[i][j]-mat2.mat[i][j];
        return new_mat;
    },
    hadamard:func(mat1,mat2)
    {
        if(mat1.col!=mat2.col or mat1.row!=mat2.row)
        {
            die("[error-hadamard] mat1\'s col or row does not match mat2\'s.");
            return nil;
        }
        var new_mat=me.new(mat1.col,mat1.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=mat1.mat[i][j]*mat2.mat[i][j];
        return new_mat;
    },
    transpose:func(mat)
    {
        var new_mat=me.new(mat.row,mat.col);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=mat.mat[j][i];
        return new_mat;
    },
    sigmoid:func(mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.sigmoid_func(mat.mat[i][j]);
        return new_mat;
    },
    diffsigmoid:func(mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.diffsigmoid_func(mat.mat[i][j]);
        return new_mat;
    },
    tanh:func(mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.tanh_func(mat.mat[i][j]);
        return new_mat;
    },
    difftanh:func(mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.difftanh_func(mat.mat[i][j]);
        return new_mat;
    },
    relu:func(mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.relu_func(mat.mat[i][j]);
        return new_mat;
    },
    diffrelu:func(mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.diffrelu_func(mat.mat[i][j]);
        return new_mat;
    },
    leaky_relu:func(k,mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.leaky_relu_func(k,mat.mat[i][j]);
        return new_mat;
    },
    diffleaky_relu:func(k,mat)
    {
        var new_mat=me.new(mat.col,mat.row);
        for(var i=0;i<new_mat.row;i+=1)
            for(var j=0;j<new_mat.col;j+=1)
                new_mat.mat[i][j]=activate_function.diffleaky_relu_func(k,mat.mat[i][j]);
        return new_mat;
    }
};

var bp=
{
    inum:2,
    hnum:4,
    onum:1,
    learning_rate:nil,
    hidden_layer:nil,
    hidden_res:nil,
    output_layer:nil,
    output_res:nil,
    result:nil,
    training_set:[],
    expect_set:[],
    init:func()
    {
        matrix.srand(time(0));
        me.hidden_layer=matrix.new(me.hnum,me.inum);
        matrix.rand_init(me.hidden_layer);
        me.output_layer=matrix.new(me.onum,me.hnum);
        matrix.rand_init(me.output_layer);
        return;
    },
    set_learning_rate:func(lr)
    {
        me.learning_rate=matrix.new(me.onum,1);
        for(var i=0;i<me.onum;i+=1)
            me.learning_rate.mat[i][0]=lr;
        return;
    },
    set_training_set:func()
    {
        for(var i=0;i<4;i+=1)
            append(me.training_set,matrix.new(me.inum,1));
        me.training_set[0].mat[0][0]=0;
        me.training_set[0].mat[0][1]=0;
        me.training_set[1].mat[0][0]=0;
        me.training_set[1].mat[0][1]=1;
        me.training_set[2].mat[0][0]=1;
        me.training_set[2].mat[0][1]=0;
        me.training_set[3].mat[0][0]=1;
        me.training_set[3].mat[0][1]=1;
        return;
    },
    set_expect_set:func()
    {
        for(var i=0;i<4;i+=1)
            append(me.expect_set,matrix.new(me.onum,1))
        me.expect_set[0].mat[0][0]=0;
        me.expect_set[1].mat[0][0]=1;
        me.expect_set[2].mat[0][0]=1;
        me.expect_set[3].mat[0][0]=0;
        return;
    },
    forward:func(i)
    {
        var tmp=nil;
        me.hidden_res=matrix.mult_mat(me.training_set[i],me.hidden_layer);
        tmp=matrix.sigmoid(me.hidden_res);
        me.output_res=matrix.mult_mat(tmp,me.output_layer);
        tmp=matrix.sigmoid(me.output_res);
        me.result=tmp;
        return;
    },
    backward:func(i)
    {
        var output_diff=matrix.sub_mat(me.expect_set[i],me.result);
        output_diff=matrix.hadamard(output_diff,me.learning_rate);
        output_diff=matrix.hadamard(output_diff,matrix.diffsigmoid(me.output_res));
        output_diff=matrix.mult_mat(output_diff,matrix.transpose(me.output_layer));
        matrix.prt_mat(output_diff);
        var hidden_diff=matrix.mult_mat();
        matrix.prt_mat(hidden_diff);

        output_layer=matrix.add_mat(output_layer,output_diff);
        var error=0;
        foreach(var i;tmp.mat[0])
            error+=i;
        error*=0.5;
        return error;
    },
    training_process:func()
    {
        var cnt=0;
        var error=1e8;
        while(error>0.01)
        {
            error=0;
            for(var i=0;i<4;i+=1)
            {
                me.forward(i);
                error+=me.backward(i);
            }
            print(error);
        }
        return;
    }
};
var main=func()
{
    bp.init();
    bp.set_learning_rate(0.1);
    bp.set_training_set();
    bp.set_expect_set();
    bp.training_process();
    return nil;
}

main();
