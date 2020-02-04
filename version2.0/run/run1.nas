var f=func(x)
{
	if(x>9)return;
	x+=1;
	f(x);
	return;
}
f(0);