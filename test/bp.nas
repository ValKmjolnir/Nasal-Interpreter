import("lib.nas");

rand(time(0));

var new_neuron=func()
{
    return {
        in:0,
        out:0,
        w:[],
        bia:0,
        diff:0
    };
}

var sigmoid=func(x)
{
    return 1/(1+math.exp(-x));
}
var diffsigmoid=func(x)
{
    x=sigmoid(x);
    return x*(1-x);
}

var inum=2;
var hnum=4;
var onum=1;
var lr=0.1;
var training_set=[[0,0],[0,1],[1,0],[1,1]];
var expect=[[0],[1],[1],[0]];

var hidden=[];
for(var i=0;i<hnum;i+=1)
{
    append(hidden,new_neuron());
    for(var j=0;j<inum;j+=1)
        append(hidden[i].w,2*rand());
    hidden[i].bia=5*rand();
}

var output=[];
for(var i=0;i<onum;i+=1)
{
    append(output,new_neuron());
    for(var j=0;j<hnum;j+=1)
        append(output[i].w,2*rand());
    output[i].bia=5*rand();
}

var forward=func(x)
{
    var input=training_set[x];
    for(var i=0;i<hnum;i+=1)
    {
        hidden[i].in=hidden[i].bia;
        for(var j=0;j<inum;j+=1)
            hidden[i].in+=hidden[i].w[j]*input[j];
        hidden[i].out=sigmoid(hidden[i].in);
    }
    for(var i=0;i<onum;i+=1)
    {
        output[i].in=output[i].bia;
        for(var j=0;j<hnum;j+=1)
            output[i].in+=output[i].w[j]*hidden[j].out;
        output[i].out=sigmoid(output[i].in);
    }
    return;
}
var run=func(vec)
{
    var input=vec;
    for(var i=0;i<hnum;i+=1)
    {
        hidden[i].in=hidden[i].bia;
        for(var j=0;j<inum;j+=1)
            hidden[i].in+=hidden[i].w[j]*input[j];
        hidden[i].out=sigmoid(hidden[i].in);
    }
    for(var i=0;i<onum;i+=1)
    {
        output[i].in=output[i].bia;
        for(var j=0;j<hnum;j+=1)
            output[i].in+=output[i].w[j]*hidden[j].out;
        output[i].out=sigmoid(output[i].in);
    }
    return;
}
var get_error=func(x)
{
    var error=0;
    var expect_set=expect[x];
    for(var i=0;i<onum;i+=1)
        error+=(expect_set[i]-output[i].out)*(expect_set[i]-output[i].out);
    error*=0.5;
    return error;
}
var backward=func(x)
{
    var input=training_set[x];
    var expect_set=expect[x];
    for(var i=0;i<onum;i+=1)
        output[i].diff=(expect_set[i]-output[i].out)*diffsigmoid(output[i].in);
    for(var i=0;i<hnum;i+=1)
    {
        hidden[i].diff=0;
        for(var j=0;j<onum;j+=1)
            hidden[i].diff+=output[j].w[i]*output[j].diff;
        hidden[i].diff*=diffsigmoid(hidden[i].in);
    }
    for(var i=0;i<hnum;i+=1)
    {
        hidden[i].bia+=hidden[i].diff;
        for(var j=0;j<inum;j+=1)
            hidden[i].w[j]+=hidden[i].diff*input[j];
    }
    for(var i=0;i<onum;i+=1)
    {
        output[i].bia+=output[i].diff;
        for(var j=0;j<hnum;j+=1)
            output[i].w[j]+=output[i].diff*hidden[j].out;
    }
    return;
}

var (cnt,error)=(0,100);
while(error>0.0005)
{
    error=0;
    for(var i=0;i<4;i+=1)
    {
        forward(i);
        error+=get_error(i);
        backward(i);
    }
    cnt+=1;
}
print('finished after ',cnt,' epoch.\n');
foreach(var v;training_set)
{
    run(v);
    print(v,': ',output[0].out,'\n');
}