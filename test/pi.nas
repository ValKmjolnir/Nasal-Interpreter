import("lib.nas");

var t=1;
var res=0;
for(var m=1;m<1e6;m+=2)
{
    res+=t*1/m;
    t*=-1;
}
println(res*4);