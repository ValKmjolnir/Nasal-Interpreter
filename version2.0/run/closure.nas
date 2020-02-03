var a=1;
var f=func
{
	var a=4;
	return func{return a;}
}
var b=f();
print(b());
# output 4

var h=func
{
	var a=4;
	var k=func{return a;};
	a=0;
	return k;
}
b=h();
print(b());
# output 0

var c=0;
var f2=func
{
	var a=4;
	var k=func{return a;};
	a=0;
	c=func{a=2;};
	return k;
}
b=f2();
print(b());
# output 0
c();
print(b());
# output 2

var cl_fun=func(x)
{
	var a=x;
	return func{return a;};
}
b=cl_fun(1);
c=cl_fun(10);
print(b());
# output 1