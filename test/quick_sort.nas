import("lib.nas");
var sort=func(vec,left,right)
{
    if(left>=right) return;
    var (L,R,tmp)=(left,right,vec[left]);
    while(left<right)
    {
        while(left<right and tmp<=vec[right])
            right-=1;
        while(left<right and tmp>=vec[left])
            left+=1;
        if(left!=right)
            (vec[left],vec[right])=(vec[right],vec[left]);
    }
    (vec[L],vec[left])=(vec[left],tmp);
    sort(vec,L,left-1);
    sort(vec,left+1,R);
    return;
}
var vec=[];
rand(time(0));
for(var i=0;i<1e4;i+=1)
    append(vec,int(rand()*1e5));
sort(vec,0,size(vec)-1);
println(vec);