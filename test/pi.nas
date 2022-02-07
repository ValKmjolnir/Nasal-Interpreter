import("lib.nas");

var (t,res)=(1,0);
for(var m=1;m<4e6;m+=2)
{
    res+=t/m;
    t=-t;
}
println(res*4);