# lib sort.nas
# valkmjolnir 2021/4/2
var sort=func(vec,left,right,cmp=func(a,b){return a<=b;})
{
    if(left>=right) return nil;
    var (L,R,tmp)=(left,right,vec[left]);
    while(left<right)
    {
        while(left<right and cmp(tmp,vec[right]))
            right-=1;
        while(left<right and cmp(vec[left],tmp))
            left+=1;
        if(left!=right)
            (vec[left],vec[right])=(vec[right],vec[left]);
    }
    (vec[L],vec[left])=(vec[left],tmp);
    sort(vec,L,left-1,cmp);
    sort(vec,left+1,R,cmp);
    return nil;
}