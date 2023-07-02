# sort.nas
# valkmjolnir 2021/4/2

var sort=func(){
    srand(); # be aware! this causes global changes
    var quick_sort_core=func(vec,left,right,cmp){
        if(left>=right) return nil;
        var base=left+int(rand()*(right-left));
        (vec[left],vec[base])=(vec[base],vec[left]);
        var (i,j,tmp)=(left,right,vec[left]);
        while(i<j){
            while(i<j and cmp(tmp,vec[j]))
                j-=1;
            vec[i]=vec[j];
            while(i<j and cmp(vec[i],tmp))
                i+=1;
            vec[j]=vec[i];
            j-=1;
        }
        vec[i]=tmp;
        quick_sort_core(vec,left,i-1,cmp);
        quick_sort_core(vec,i+1,right,cmp);
        return nil;
    }
    return func(vec,cmp=func(a,b){return a<b;}){
        quick_sort_core(vec,0,size(vec)-1,cmp);
        return nil;
    }
}();

var var_sort=func(){
    srand(); # be aware! this causes global changes
    var quick_sort_core=func(vec,left,right){
        if(left>=right) return nil;
        var base=left+int(rand()*(right-left));
        (vec[left],vec[base])=(vec[base],vec[left]);
        var (i,j,tmp)=(left,right,vec[left]);
        while(i<j){
            while(i<j and tmp<vec[j])
                j-=1;
            vec[i]=vec[j];
            while(i<j and vec[i]<tmp)
                i+=1;
            vec[j]=vec[i];
            j-=1;
        }
        vec[i]=tmp;
        quick_sort_core(vec,left,i-1,cmp);
        quick_sort_core(vec,i+1,right,cmp);
        return nil;
    }
    return func(vec){
        quick_sort_core(vec,0,size(vec)-1,cmp);
        return nil;
    }
}();