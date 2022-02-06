import("lib.nas");
for(;;)break;
for(;;)
{
    var a=1;
    break;
}
for(var i=1;;)break;
for(var i=1;;i+=1)break;
for(var i=1;i<10;i+=1)print(i,'\n');

while(1)break;
var j=0;
while(j<10)
{
    print(j,'\n');
    j+=1;
}

forindex(var j;[0,1,2,3])print(j,'\n');
forindex(var j;[0,1,2,3])
{
    var a=j;
    print(a*a,'\n');
}
foreach(var j;[0,1,2,3])print([0,1,2,3][j],'\n');
foreach(var j;[0,1,2,3])
{
    var a=[0,1,2,3][j];
    print(a*a-1,'\n');
}

var f=func(){
    var x=0;
    return func(){x+=1;};
}();
for(var i=0;i<4e6;i+=1)
    f();