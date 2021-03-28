import("lib.nas");
var fib=func(x)
{
	if(x<2) return x;
	return fib(x-1)+fib(x-2);
}
for(var i=0;i<31;i+=1)
    print(fib(i),'\n');

var m=[0,1,1,2,3,5,8];
setsize(m,32);
var fib=func(n)
{
    if(m[n]!=nil) return m[n];
    var t=fib(n-1)+fib(n-2);
    m[n]=t;
    return t;
}
print(fib(31),'\n');