import("lib.nas");
rand(time(0));
var sort=func(vec,left,right)
{
    if(left>=right) return nil;
    var L=left;
    var R=right;
    var tmp=vec[L];
    while(left<right)
    {
        while(left<right and tmp<=vec[right])
            right-=1;
        while(left<right and tmp>=vec[left])
            left+=1;
        if(left!=right)
        {
            var t=vec[left];
            vec[left]=vec[right];
            vec[right]=t;
        }
    }
    vec[L]=vec[left];
    vec[left]=tmp;
    sort(vec,L,left-1);
    sort(vec,left+1,R);
    return nil;
}
var vec=[];
for(var i=0;i<200;i+=1)
    append(vec,int(rand()*1000));
sort(vec,0,size(vec)-1);
println(vec);