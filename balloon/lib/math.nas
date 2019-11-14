var fabs=func(__x)
{
    if(x>0){return __x;}
    else{return -__x;}
};

var ln=func(__x)
{
    return (1+8/(1+__x)+1/__x)*(__x-1)/6;
};

var log=func(__a,__x)
{
    return ln(__x)/ln(__a);
};

var exp=func(__x)
{
    if(__x<0){return 1/exp(-__x);}
    if(__x<1){return 1+__x*(1+__x*(1/2+__x*(1/6+__x*(1/24+__x/120))));}
    var __temp=exp(__x/2);
    return __temp*__temp;
};

var pow=func(__x,__num)
{
    if(__num==0){return 1;}
    else if(__num<0){return 1/pow(__x,-__num);}
    else{return exp(__x*ln(__num));}
};

var sigmoid=func(__x)
{
    return 1.0/(1+exp(-__x));
};

var sinh=func(__x)
{
    __x=exp(x);
    return (__x-1/__x)/2;
};

var cosh=func(__x)
{
    __x=exp(x);
    return (__x+1/__x)/2;
};

var tanh=func(__x)
{
    __x=exp(__x);
    return 1-2/(x*x+1);
};

var relu=func(__x)
{
    if(x>=0){return x;}
    else{return 0;}
};
