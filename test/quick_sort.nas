
var var_sort = func() {
    srand(); # be aware! this causes global changes
    var quick_sort_core = func(vec,left,right) {
        if (left>=right) return nil;
        var base=left+int(rand()*(right-left));
        (vec[left],vec[base])=(vec[base],vec[left]);
        var (i,j,tmp)=(left,right,vec[left]);
        while(i<j) {
            while(i<j and tmp<vec[j])
                j-=1;
            vec[i]=vec[j];
            while(i<j and vec[i]<tmp)
                i+=1;
            vec[j]=vec[i];
            j-=1;
        }
        vec[i]=tmp;
        quick_sort_core(vec,left,i-1);
        quick_sort_core(vec,i+1,right);
        return nil;
    }
    return func(vec) {
        quick_sort_core(vec,0,size(vec)-1);
        return nil;
    }
}();

var vec = [];
setsize(vec, 1e4);
rand(time(0));
for (var i=0;i<1e4;i+=1)
    vec[i] = int(rand()*1e5);
sort(vec);
for (var i=1;i<1e4;i+=1) {
    if (vec[i]<vec[i-1]) {
        die("incorrect sort result");
    }
}

var test = func(n) {
    var a=[];
    setsize(a,n);
    for (var i=0;i<n;i+=1) {
        a[i]=int(rand()*n);
    }
    var ts=maketimestamp();
    ts.stamp();
    var_sort(a);
    println("[time] ",str(n)," in ",ts.elapsedMSec()/1000," sec (direct cmp)");

    var a=[];
    setsize(a,n);
    for (var i=0;i<n;i+=1) {
        a[i]=int(rand()*n);
    }
    ts.stamp();
    sort(a);
    println("[time] ",str(n)," in ",ts.elapsedMSec()/1000," sec (lambda cmp)");
}

for (var i=1000;i<1e6;i*=10) {
    test(i);
}