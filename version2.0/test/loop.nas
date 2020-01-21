for(;;)break;
for(;;)
{
    var a=1;
    break;
}
for(var i=1;;)break;
for(var i=1;;i+=1)break;
for(var i=1;i<10;i+=1)print(i);

while(1)break;
var j=0;
while(j<10)
{
    print(j);
    j+=1;
}

forindex(var j;[0,1,2,3])print(j);
forindex(var j;[0,1,2,3])
{
    var a=j;
    print(a*a);
}
foreach(var j;[0,1,2,3])print([0,1,2,3][j]);
foreach(var j;[0,1,2,3])
{
    var a=[0,1,2,3][j];
    print(a*a-1);
}
