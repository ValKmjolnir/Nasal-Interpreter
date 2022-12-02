var mod=func(n,a){
    return n-int(n/a)*a;
}

var main=func(run_time){
    var ts=maketimestamp();

    var (sin,cos)=(math.sin,math.cos);
    var chars=split("",".,-~:;=!*#$@");

    var (A,B)=(0,0);
    var (z,b)=([],[]);
    setsize(z,1760);
    setsize(b,1760);

    print("\e[2J");
    for(var run=0;run<run_time;run+=1){
        ts.stamp();
        forindex(var i;b){
            b[i]=" ";
            z[i]=0;
        }
        for(var j=0;j<6.28;j+=0.07){
            for(var i=0;i<6.28;i+=0.02){
                var (c,d,e,f,g)=(sin(i),cos(j),sin(A),sin(j),cos(A));
                var h=d+2;
                var D=1/(c*h*e+f*g+5);
                var (l,m,n)=(cos(i),cos(B),sin(B));
                var t=c*h*g-f*e;
                var x=int(40+30*D*(l*h*m-t*n));
                var y=int(12+15*D*(l*h*n+t*m));
                var o=int(x+80*y);
                var N=int(8*((f*e-c*d*g)*m-c*d*e-f*g-l*d*n));
                if(22>y and y>0 and x>0 and 80>x and D>z[o]){
                    z[o]=D;
                    b[o]=chars[N>0?N:0];
                }
            }
        }
        print("\e[H");
        var screen="";
        for(var k=0;k<1761;k+=1) {
            screen~=(mod(k,80)?b[k]:"\n");
            A+=0.00004;
            B+=0.00002;
        }
        println(" ",int(1/(ts.elapsedMSec()/1000))," fps  ");
        print(screen);
    }
}

if(size(runtime.argv()) and !math.isnan(num(runtime.argv()[0]))){
    main(num(runtime.argv()[0]));
}else{
    main(5e2);
}