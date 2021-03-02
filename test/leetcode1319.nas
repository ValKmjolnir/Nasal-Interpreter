import("lib.nas");
# 并查集
var n=4;
var input=[[0,1],[0,2],[1,2]];

var find_root=func(x,parent)
{
    while(parent[x]!=nil)
        x=parent[x];
    return x;
}
var union_root=func(x,y,parent)
{
    var x_root=find_root(x,parent);
    var y_root=find_root(y,parent);
    if(x_root==y_root) return 0;
    else parent[x_root]=y_root;
    return 1;
}
var makeConnect=func(n,connections)
{
    if(size(connections)<n-1) return -1;
    var cnt=n-1;
    var parent=[];
    setsize(parent,n);
    for(var i=0;i<size(connections);i+=1)
        if(union_root(connections[i][0],connections[i][1],parent))
            cnt-=1;
    return cnt;
}

print(makeConnect(n,input));