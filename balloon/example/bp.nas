
var neuron=
{
    in:0,
    out:0,
    diff:0,
    bia:0,
    w:[]
};

var INUM=2;
var HNUM=4;
var ONUM=1;

var input_layer=[];
var hidden=[];
var output=[];
var expect=[];

var difftanh=func(x)
{
    var ret=tanh(x);
    return 1-ret*ret;
};

var diffsigmoid=func(x)
{
    var ret=sigmoid(x);
    return ret*(1-ret);
};

var init=func()
{
    for(var i=0;i<INUM;i+=1){append(input_layer,0);}
    for(var i=0;i<HNUM;i+=1)
    {
        append(hidden,neuron);
        for(var j=0;j<INUM;j+=1){append(hidden[i].w,rand());}
        hidden[i].bia=rand();
    }
    for(var i=0;i<ONUM;i+=1)
    {
        append(expect,0);
        append(output,neuron);
        for(var j=0;j<HNUM;j+=1){append(output[i].w,rand());}
        output[i].bia=rand();
    }
    return;
};

var calc=func()
{
    for(var i=0;i<HNUM;i+=1)
    {
        hidden[i].in=hidden[i].bia;
        for(var j=0;j<INUM;j+=1){hidden[i].in+=input_layer[j]*hidden[i].w[j];}
        hidden[i].out=tanh(hidden[i].in);
    }
    for(var i=0;i<ONUM;i+=1)
    {
        output[i].in=output[i].bia;
        for(var j=0;j<HNUM;j+=1){output[i].in+=hidden[j].out*output[i].w[j];}
        output[i].out=sigmoid(output[i].in);
    }
    var error=0;
    for(var i=0;i<ONUM;i+=1){error+=(expect[i]-output[i].out)*(expect[i]-output[i].out);}
    error*=0.5;
    return error;
};

var back=func()
{
    var lr=0.9;
    for(var i=0;i<ONUM;i+=1){output[i].diff=(expect[i]-output[i].out)*diffsigmoid(output[i].in);}
    for(var i=0;i<HNUM;i+=1)
    {
        hidden[i].diff=0;
        for(var j=0;j<ONUM;j+=1){hidden[i].diff+=output[j].diff*output[j].w[i];}
        hidden[i].diff*=difftanh(hidden[i].in);
    }
    for(var i=0;i<HNUM;i+=1)
    {
        hidden[i].bia+=lr*hidden[i].diff;
        for(var j=0;j<INUM;j+=1){hidden[i].w[j]+=lr*input_layer[j]*hidden[i].diff;}
    }
    for(var i=0;i<ONUM;i+=1)
    {
        output[i].bia+=lr*output[i].diff;
        for(var j=0;j<HNUM;j+=1){output[i].w[j]+=lr*hidden[j].out*output[i].diff;}
    }
    return;
};

init();
var cnt=0;
var total_error=10;
while(total_error>0.001)
{
    total_error=0;
    input_layer[0]=1;input_layer[1]=1;expect[0]=1;
    total_error+=calc();
    back();
    input_layer[0]=1;input_layer[1]=0;expect[0]=1;
    total_error+=calc();
    back();
    input_layer[0]=0;input_layer[1]=1;expect[0]=1;
    total_error+=calc();
    back();
    input_layer[0]=0;input_layer[1]=0;expect[0]=0;
    total_error+=calc();
    back();
    cnt+=1;
    if(cnt==10)
    {
        print('10 times: ',total_error,'\n');
        cnt=0;
    }
}

input_layer[0]=1;input_layer[1]=1;expect[0]=1;
calc();
print('1 or 1: ',output[0].out,'\n');
input_layer[0]=1;input_layer[1]=0;expect[0]=1;
calc();
print('1 or 0: ',output[0].out,'\n');
input_layer[0]=0;input_layer[1]=1;expect[0]=1;
calc();
print('0 or 1: ',output[0].out,'\n');
input_layer[0]=0;input_layer[1]=0;expect[0]=0;
calc();
print('0 or 0: ',output[0].out,'\n');