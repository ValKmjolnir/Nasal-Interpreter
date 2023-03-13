import.stl.sort;
import.stl.padding;
import.stl.process_bar;

var mess=func(vec) {
    srand();
    var s=size(vec);
    for(var i=s-1;i>=0;i-=1) {
        var r=int(rand()*i);
        (vec[r],vec[i])=(vec[i],vec[r]);
    }
}

var project=func(n) {
    # get(s) :- color(_, s, _).

    var ts=maketimestamp();
    var bar=process_bar.high_resolution_bar(40);

    var color=[];
    setsize(color,n);

    var last_step=0;
    ts.stamp();
    for(var i=0;i<n;i+=1) {
        color[i]=[i,"color "~i,i+n*10];
        # generate process bar, every 0.2%
        if((i-last_step)/n>1/500) {
            last_step=i;
            print(" ",bar.bar((i+1)/n)," ",leftpad(str(int((i+1)/n*100)),3),"% | \r");
        }
    }
    print(" ",bar.bar(1)~" 100% | ",rightpad(str(ts.elapsedMSec()/1000),5)," s | ");

    mess(color);

    ts.stamp();
    var cnt=0;
    foreach(var c;color) {
        var data=c[2];
        cnt+=1;
    }

    println(rightpad(str(cnt),7)," in ",ts.elapsedMSec()/1000," s");
}

var select=func(n) {
    # get(s) :- color(_, _, x), message(x, s).

    var ts=maketimestamp();
    var bar=process_bar.high_resolution_bar(40);

    var color=[];
    var message=[];
    setsize(color,n);
    setsize(message,n);

    var last_step=0;
    ts.stamp();
    for(var i=0;i<n;i+=1) {
        color[i]=[i,"color "~i,i+n*10];
        message[i]=[i+n*10,"message "~i];
        # generate process bar, every 0.2%
        if((i-last_step)/n>1/500) {
            last_step=i;
            print(" ",bar.bar((i+1)/n)," ",leftpad(str(int((i+1)/n*100)),3),"% | \r");
        }
    }
    print(" ",bar.bar(1)~" 100% | ",rightpad(str(ts.elapsedMSec()/1000),5)," s | ");

    mess(color);
    mess(message);

    ts.stamp();
    sort(color,func(a,b){return a[2]<b[2]});
    sort(message,func(a,b){return a[0]<b[0]});

    var cnt=0;
    foreach(var c;color) {
        var left=0;
        var right=size(message)-1;
        var data=c[2];
        while(left<=right) {
            var mid=int((left+right)/2);
            var res=message[mid][0];
            if(data==res) {
                cnt+=1;
                break;
            } else if(data>res) {
                left=mid+1;
            } else {
                right=mid-1;
            }
        }
    }

    println(rightpad(str(cnt),7)," in ",ts.elapsedMSec()/1000," s");
}

var cartesian=func(n) {
    # get(x, y, z, a, b) :- color(x, y, z), message(a, b).

    var ts=maketimestamp();
    var bar=process_bar.high_resolution_bar(40);

    var color=[];
    var message=[];
    setsize(color,n);
    setsize(message,n);

    var last_step=0;
    ts.stamp();
    for(var i=0;i<n;i+=1) {
        color[i]=[i,"color "~i,i+n*10];
        message[i]=[i+n*10,"message "~i];
        # generate process bar, every 0.2%
        if((i-last_step)/n>1/500) {
            last_step=i;
            print(" ",bar.bar((i+1)/n)," ",leftpad(str(int((i+1)/n*100)),3),"% | \r");
        }
    }
    print(" ",bar.bar(1)~" 100% | ",rightpad(str(ts.elapsedMSec()/1000),5)," s | ");

    mess(color);
    mess(message);

    ts.stamp();

    var res=[];
    foreach(var c;color) {
        foreach(var m;message) {
            var tmp=[];
            foreach(var k;c) {
                append(tmp,k);
            }
            foreach(var k;m) {
                append(tmp,k);
            }
            append(res,tmp);
        }
    }

    println(rightpad(str(size(res)),7)," in ",ts.elapsedMSec()/1000," s");
}

println("project");
for(var i=10;i<1e6;i*=10) {
    project(i);
}
println("select");
for(var i=10;i<1e6;i*=10) {
    select(i);
}
println("cartesian");
for(var i=100;i<600;i+=100) {
    cartesian(i);
}