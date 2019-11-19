var pi=3.14159265358979;
var ln_2=0.69314718055994530942;
var ln_10=2.30258509299404568402;

var abs=func(__x)
{
    if(__x>0){return __x;}
    else{return -__x;}
};

var ln=func(_x)
{
    var __balloon_lib_ln_asr=func(__x,__y)
    {
        if(abs(__y-__x)<=0.1){return (1/__x+8/(__x+__y)+1/__y)*(__y-__x)/6;}
        var __mid=(__x+__y)/2;
        return __balloon_lib_ln_asr(__x,__mid)+__balloon_lib_ln_asr(__mid,__y);
    };
    return __balloon_lib_ln_asr(1,_x);
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
    else{return exp(__num*ln(__x));}
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
    return 1-2/(__x*__x+1);
};

var relu=func(__x)
{
    if(x>=0){return __x;}
    else{return 0;}
};

var sin=func(__x)
{
    if(__x<0){return -1*sin(-__x);}
    var fl=1;
    while(__x>2*pi){__x-=2*pi;}
    if(__x>pi){__x-=2*pi;}
    elsif(__x<-pi){__x+=2*pi;}
    if(__x>pi/2){__x-=pi;fl*=-1;}
    elsif(__x<-pi/2){__x+=pi;fl*=-1;}
    if(__x>pi/4){return cos(pi/2-__x);}
    else{return fl*(__x*(1-__x*__x*(1/6+__x*__x*(1/120-__x*__x*(1/5040+__x*__x/362880)))));}
};

var cos=func(__x)
{
    __x=abs(__x);
    var fl=1;
    while(__x>2*pi){__x-=2*pi;}
    if(__x>pi){__x-=2*pi;}
    elsif(__x<-pi){__x+=2*pi;}
    if(__x>pi/2){__x-=pi;fl*=-1;}
    elsif(__x<-pi/2){__x+=pi;fl*=-1;}
    if(__x>pi/4){return sin(pi/2-__x);}
    else{return fl*(1-__x*__x*(1/2+__x*__x*(1/24-__x*__x*(1/720+__x*__x/40320))));}
};

var tan=func(__x)
{
    return sin(__x)/cos(__x);
};

var sqrt=func(__x)
{
    if(__x>100){return 10*sqrt(__x/100);}
    var t=__x/8+0.5+2*__x/(4+__x);
    var c=10;
    while(c>0)
    {
        t=(t+__x/t)/2;
        c-=1;
    }
    return t;
};

var asin=func(_x)
{
    var __balloon_lib_asin_asr=func(__x,__y)
    {
        var __mid=(__x+__y)/2;
        if(abs(__y-__x)<=0.01){return (1/sqrt(1-__x*__x)+4/sqrt(1-__mid*__mid)+1/sqrt(1-__y*__y))*(__y-__x)/6;}
        return __balloon_lib_asin_asr(__x,__mid)+__balloon_lib_asin_asr(__mid,__y);
    };
    if(abs(_x)>1){return -1;}
    var fl=1;
    if(_x<0){fl=-fl;_x=-_x;}
    if(abs(_x-1)<0.001){return pi/2;}
    return fl*__balloon_lib_asin_asr(0,_x);
};

var acos=func(__x)
{
    if(abs(__x)>1) {return -1;}
    return pi/2-asin(__x);
};

var atan=func(__x)
{
    if(__x<0){return -atan(-__x);}
    if(__x>1){return pi/2-atan(1/__x);}
    if(__x>0.001){return 2*atan((sqrt(1+__x*__x)-1)/__x);}
    return __x*(1-__x*__x*(1/3+__x*__x*(1/5-__x*__x*(1/7+__x*__x/9))));
};