import.stl.mat;

rand(time(0));

var new_neuron=func(){
    return {
        in:0,
        out:0,
        w:[],
        bia:0,
        diff:0
    };
}

var tanh=func(x){
    var (a,b)=(math.exp(x),math.exp(-x));
    return (a-b)/(a+b);
}
var difftanh=func(x){
    x=tanh(x);
    return 1-x*x;
}
var sigmoid=func(x){
    return 1/(1+math.exp(-x));
}
var diffsigmoid=func(x){
    x=sigmoid(x);
    return x*(1-x);
}

var (inum,hnum,onum)=(2,5,1);
var training_set=[[0,0],[0,1],[1,0],[1,1]];
var expect=[0,1,1,0];

var hidden=[];
for(var i=0;i<hnum;i+=1){
    append(hidden,new_neuron());
    for(var j=0;j<inum;j+=1)
        append(hidden[i].w,rand()>0.5?-2*rand():2*rand());
    hidden[i].bia=rand()>0.5?-5*rand():5*rand();
}

var output=[];
for(var i=0;i<onum;i+=1){
    append(output,new_neuron());
    for(var j=0;j<hnum;j+=1)
        append(output[i].w,rand()>0.5?-2*rand():2*rand());
    output[i].bia=rand()>0.5?-5*rand():5*rand();
}

var forward=func(x){
    var input=training_set[x];
    for(var i=0;i<hnum;i+=1){
        hidden[i].in=hidden[i].bia;
        for(var j=0;j<inum;j+=1)
            hidden[i].in+=hidden[i].w[j]*input[j];
        hidden[i].out=tanh(hidden[i].in);
    }
    for(var i=0;i<onum;i+=1){
        output[i].in=output[i].bia;
        for(var j=0;j<hnum;j+=1)
            output[i].in+=output[i].w[j]*hidden[j].out;
        output[i].out=sigmoid(output[i].in);
    }
    return;
}
var run=func(vec){
    var input=vec;
    for(var i=0;i<hnum;i+=1){
        hidden[i].in=hidden[i].bia;
        for(var j=0;j<inum;j+=1)
            hidden[i].in+=hidden[i].w[j]*input[j];
        hidden[i].out=tanh(hidden[i].in);
    }
    for(var i=0;i<onum;i+=1){
        output[i].in=output[i].bia;
        for(var j=0;j<hnum;j+=1)
            output[i].in+=output[i].w[j]*hidden[j].out;
        output[i].out=sigmoid(output[i].in);
    }
    return;
}
var get_error=func(x){
    return 0.5*(expect[x]-output[0].out)*(expect[x]-output[0].out);
}
var backward=func(x){
    var input=training_set[x];
    output[0].diff=(expect[x]-output[0].out)*diffsigmoid(output[0].in);
    for(var i=0;i<hnum;i+=1){
        hidden[i].diff=0;
        for(var j=0;j<onum;j+=1)
            hidden[i].diff+=output[j].w[i]*output[j].diff;
        hidden[i].diff*=difftanh(hidden[i].in);
    }
    for(var i=0;i<hnum;i+=1){
        hidden[i].bia+=hidden[i].diff;
        for(var j=0;j<inum;j+=1)
            hidden[i].w[j]+=hidden[i].diff*input[j];
    }
    for(var i=0;i<onum;i+=1){
        output[i].bia+=output[i].diff;
        for(var j=0;j<hnum;j+=1)
            output[i].w[j]+=output[i].diff*hidden[j].out;
    }
    return;
}

var (cnt,error)=(0,100);
while(error>0.0005){
    error=0;
    for(var i=0;i<4;i+=1){
        forward(i);
        error+=get_error(i);
        backward(i);
    }
    cnt+=1;
    if(cnt>=1e4)
        break;
}
if(cnt>=3e5){
    print("failed to train, ",cnt," epoch.\n");
}else{
    print('finished after ',cnt,' epoch.\n');
}
foreach(var v;training_set){
    run(v);
    print(v,': ',output[0].out,'\n');
}

bp_example();