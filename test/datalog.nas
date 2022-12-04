import.stl.sort;
import.stl.process_bar;

for(var i=0;i<3910;i+=1){
    runtime.gc.extend("str");
    runtime.gc.extend("vec");
}

var test=func(n){
    var ts=maketimestamp();
    var bar=process_bar.high_resolution_bar(50);

    var color=[];
    var message=[];
    setsize(color,n);
    setsize(message,n);
    var last_step=0;
    ts.stamp();
    for(var i=0;i<n;i+=1){
        color[i]=[i,"color "~i,i+n*10];
        message[i]=[i+n*10,"message "~i];
        if((i-last_step)/n>1/100){
            last_step=i;
            print(bar.bar((i+1)/n)~" "~str(int((i+1)/n*100))~"%  \r");
        }
    }
    print(bar.bar((i+1)/n)~" 100% | ",ts.elapsedMSec()/1000," s | ");

    var mess=func(vec){
        var s=size(vec);
        for(var i=s-1;i>=0;i-=1){
            var r=int(rand()*i);
            (vec[r],vec[i])=(vec[i],vec[r]);
        }
    }

    mess(color);
    mess(message);

    
    ts.stamp();
    sort(color,func(a,b){return a[2]<b[2]});
    sort(message,func(a,b){return a[0]<b[0]});

    var cnt=0;
    foreach(var c;color){
        var left=0;
        var right=size(message)-1;
        var data=c[2];
        while(left<=right){
            var mid=int((left+right)/2);
            var res=message[mid][0];
            if(data==res){
                cnt+=1;
                break;
            }else if(data>res){
                left=mid+1;
            }else{
                right=mid-1;
            }
        }
    }

    println(str(cnt)," result(s) in ",ts.elapsedMSec()/1000," sec");
}

for(var i=100;i<1e7;i*=10)
    test(i);