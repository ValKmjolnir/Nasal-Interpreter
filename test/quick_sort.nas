import.stl.sort;

var vec=[];
rand(time(0));
for(var i=0;i<1e4;i+=1)
    append(vec,int(rand()*1e5));
sort(vec);
for(var i=1;i<1e4;i+=1) {
    if (vec[i]<vec[i-1]) {
        die("incorrect sort result");
    }
}

var test=func(n){
    var a=[];
    setsize(a,n);
    for(var i=0;i<n;i+=1){
        a[i]=int(rand()*n);
    }
    var ts=maketimestamp();
    ts.stamp();
    var_sort(a);
    println("[time] ",str(n)," in ",ts.elapsedMSec()/1000," sec (value)");

    var a=[];
    setsize(a,n);
    for(var i=0;i<n;i+=1){
        a[i]=int(rand()*n);
    }
    ts.stamp();
    sort(a);
    println("[time] ",str(n)," in ",ts.elapsedMSec()/1000," sec (lambda)");
}

for(var i=1000;i<1e6;i*=10){
    test(i);
}