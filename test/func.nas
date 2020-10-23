var f=func(n,m,dynamic...)
{
    print(n+m," ",dynamic);
    n=dynamic;
    m=dynamic;
    n+=m;
    return dynamic;
};
print(f(1,1,0,0,0,0,0,(1+2+3+(1+2+3+4)))[3]);
function([0,1,2,3],{str:"str"});