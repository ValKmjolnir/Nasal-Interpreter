# ray tracing example
# modified from Artem Yashin's project Console3D

# MIT License
# Copyright (c) 2021 Artem Yashin

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
import.module.libmat;

func(){
    # allocate more spaces
    var t=[];
    for(var i=0;i<16384;i+=1)
        append(t,[],[],[],str(i));
    t=[];
}();

var (max,min,sqrt,sin,cos,abs)=(math.max,math.min,math.sqrt,math.sin,math.cos,math.abs);

var (vec2,vec3)=(libmat.vec2.new,libmat.vec3.new);
var (vec2add,vec2sub,vec2mul,vec2div,vec2len)=(
    libmat.vec2.add,
    libmat.vec2.sub,
    libmat.vec2.mul,
    libmat.vec2.div,
    libmat.vec2.len
);
var (vec3add,vec3sub,vec3mul,vec3div,vec3neg,vec3norm,vec3len,vec3dot)=(
    libmat.vec3.add,
    libmat.vec3.sub,
    libmat.vec3.mul,
    libmat.vec3.div,
    libmat.vec3.neg,
    libmat.vec3.norm,
    libmat.vec3.len,
    libmat.vec3.dot
);
var (rotateX,rotateY,rotateZ)=(
    libmat.vec3.rx,
    libmat.vec3.ry,
    libmat.vec3.rz,
);

var use_raw=func(){
    vec2=func(x,y){return [x,y];}
    vec2add=func(v1,v2){return [v1[0]+v2[0],v1[1]+v2[1]];}
    vec2sub=func(v1,v2){return [v1[0]-v2[0],v1[1]-v2[1]];}
    vec2mul=func(v1,v2){return [v1[0]*v2[0],v1[1]*v2[1]];}
    vec2div=func(v1,v2){return [v1[0]/v2[0],v1[1]/v2[1]];}
    vec3=func(x,y,z){return [x,y,z];}
    vec3add=func(v1,v2){return [v1[0]+v2[0],v1[1]+v2[1],v1[2]+v2[2]];}
    vec3sub=func(v1,v2){return [v1[0]-v2[0],v1[1]-v2[1],v1[2]-v2[2]];}
    vec3mul=func(v1,v2){return [v1[0]*v2[0],v1[1]*v2[1],v1[2]*v2[2]];}
    vec3div=func(v1,v2){return [v1[0]/v2[0],v1[1]/v2[1],v1[2]/v2[2]];}
    vec3neg=func(v){return [-v[0],-v[1],-v[2]];}
    vec2len=func(v){var (x,y)=(v[0],v[1]); return sqrt(x*x+y*y);}
    vec3len=func(v){var (x,y,z)=(v[0],v[1],v[2]); return sqrt(x*x+y*y+z*z);}
    vec3norm=func(v){var t=vec3len(v); return vec3div(v,[t,t,t]);}
    vec3dot=func(a,b){return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];}
    rotateX=func(a,angle){return [a[0],a[2]*sin(angle)+a[1]*cos(angle),a[2]*cos(angle)-a[1]*sin(angle)];}
    rotateY=func(a,angle){return [a[0]*cos(angle)-a[2]*sin(angle),a[1],a[0]*sin(angle)+a[2]*cos(angle)];}
    rotateZ=func(a,angle){return [a[0]*cos(angle)-a[1]*sin(angle),a[0]*sin(angle)+a[1]*cos(angle),a[2]];}
}

var clamp=func(value,_min,_max){
    return max(min(value,_max),_min);
}
var sign=func(a){
    return (0<a)-(a<0);
}
var step=func(edge,x){
    return x>edge;
}

var vec3abs=func(v){
    return [abs(v[0]),abs(v[1]),abs(v[2])];
}
var vec3sign=func(v){
    return [sign(v[0]),sign(v[1]),sign(v[2])];
}
var vec3step=func(edge,v){
    return [step(edge[0],v[0]),step(edge[1],v[1]),step(edge[2],v[2])];
}
var vec3reflect=func(rd,n){
    var d=vec3dot(n,rd);
    return vec3sub(rd,vec3mul(n,vec3mul([2,2,2],[d,d,d])));
}

var sphere=func(ro,rd,r) {
    var b=vec3dot(ro,rd);
    var c=vec3dot(ro,ro)-r*r;
    var h=b*b-c;
    if(h<0.0) return [-1.0,-1.0];
    h=sqrt(h);
    return [-b-h,-b+h];
}

var box=func(ro,rd,boxSize,outNormal) {
    var m=vec3div([1.0,1.0,1.0],rd);
    var n=vec3mul(m,ro);
    var k=vec3mul(vec3abs(m),boxSize);
    var t1=vec3sub(vec3neg(n),k);
    var t2=vec3add(vec3neg(n),k);
    var tN=max(max(t1[0],t1[1]),t1[2]);
    var tF=min(min(t2[0],t2[1]),t2[2]);
    if (tN>tF or tF<0.0) return [-1.0,-1.0];
    var yzx=[t1[1],t1[2],t1[0]];
    var zxy=[t1[2],t1[0],t1[1]];
    var tmp=vec3mul(vec3mul(vec3neg(vec3sign(rd)), vec3step(yzx,t1)),vec3step(zxy,t1));
    outNormal[0]=tmp[0];
    outNormal[1]=tmp[1];
    outNormal[2]=tmp[2];
    return [tN, tF];
}

var plane=func(ro,rd,p,w) {
    return -(vec3dot(ro,p)+w)/vec3dot(rd,p);
}

var main=func(frame) {

    var height=15*2;
    var width=int(height*1/0.618)*2;

    var aspect=width/height;
    var pixelAspect=11.0/24.0;

    var gradient=split(""," .:!/r(l1Z4H9W8$");
    var gradientSize=size(gradient)-1;

    var screen=[];
    setsize(screen,width*height);

    var light=vec3norm([-0.5,0.5,-1.0]);
    var spherePos=[0,3,0];
    var vec2_2_2=[2,2];
    var vec2_1_1=[1,1];
    var vec3_000=[0,0,0];
    var vec3_00n1=[0,0,-1];
    var vec3_111=[1,1,1];

    print("\e[2J");
    var stamp=maketimestamp();
    for(var t=0;t<frame;t+=1){
        stamp.stamp();
        for(var i=0;i<width;i+=1){
            for(var j=0;j<height;j+=1){
                var uv=vec2sub(vec2mul(vec2div([i,j],[width,height]),vec2_2_2),vec2_1_1);
                uv[0]*=aspect*pixelAspect;
                var ro=[-6,0,0];
                var rd=vec3norm([2,uv[0],uv[1]]);
                ro=rotateY(ro,0.25);
                rd=rotateY(rd,0.25);
                ro=rotateZ(ro,t*0.03);
                rd=rotateZ(rd,t*0.03);
                var diff=1;
                for (var k=0;k<5;k+=1){
                    var minIt=99999;
                    var intersection=sphere(vec3sub(ro,spherePos),rd,1);
                    var n=vec3_000;
                    var albedo=1;
                    if(intersection[0]>0){
                        var itPoint=vec3add(vec3sub(ro,spherePos),vec3mul(rd,[intersection[0],intersection[0],intersection[0]]));
                        minIt=intersection[0];
                        n=vec3norm(itPoint);
                    }
                    var boxN=[0,0,0];
                    intersection=box(ro,rd,vec3_111,boxN);
                    if(intersection[0]>0 and intersection[0]<minIt){
                        minIt=intersection[0];
                        n=boxN;
                    }
                    var tmp=plane(ro,rd,vec3_00n1,1);
                    intersection=[tmp,tmp];
                    if(intersection[0]>0 and intersection[0]<minIt){
                        minIt=intersection[0];
                        n=vec3_00n1;
                        albedo=0.5;
                    }
                    if(minIt<99999){
                        diff*=(vec3dot(n,light)*0.5+0.5)*albedo;
                        ro=vec3add(ro,vec3mul(rd,[minIt-0.01,minIt-0.01,minIt-0.01]));
                        rd=vec3reflect(rd,n);
                    }
                    else break;
                }
                var color=int(diff*20);
                color=clamp(color,0,gradientSize);
                screen[i+j*width]=gradient[color];
            }
        }
        var s="";
        forindex(var index;screen){
            s~=screen[index];
            if(index+1-int((index+1)/width)*width==0)
                s~="\n";
        }
        var elt=stamp.elapsedMSec()/1000;
        print("\e[H",int(1/elt)," fps  \n",s);
    }
}

var st=maketimestamp();
var run=[0,0];
var frame=1e3;
if(size(runtime.argv())!=0){
    var n=num(runtime.argv()[0]);
    if(!math.isnan(n)){
        frame=n;
    }
}
st.stamp();
main(frame);
run[0]=st.elapsedMSec();
use_raw();
st.stamp();
main(frame);
run[1]=st.elapsedMSec();

println("test 0: ",run[0]/1000,"s ",frame*1000/run[0]," fps");
println("test 1: ",run[1]/1000,"s ",frame*1000/run[1]," fps");