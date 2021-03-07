var print=func(elements...)
{
    nasal_call_builtin_std_cout(elements);
    return nil;
};
var setsize=func(vector,size)
{
    nasal_call_builtin_set_size(vector,size);
    return nil;
}
var fib=func(x)
{
	if(x<2) return x;
	return fib(x-1)+fib(x-2);
}
print(fib(30),'\n');

var m=[0,1,1,2,3,5,8];
setsize(m,101);
var fib=func(n)
{
    if(m[n]!=nil) return m[n];
    var t=fib(n-1)+fib(n-2);
    m[n]=t;
    return t;
}
print(fib(100),'\n');