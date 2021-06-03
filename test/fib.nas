import("lib.nas");
var fib=func(x)
{
	if(x<2) return x;
	return fib(x-1)+fib(x-2);
}
for(var i=0;i<31;i+=1)
	print(fib(i),'\n');
    