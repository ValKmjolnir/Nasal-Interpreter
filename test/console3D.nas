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

var t=[];
for(var i=0;i<8192;i+=1)
    append(t,{});
t=[];

var vec2=func(x,y){
    return {x:x,y:y};
}
var vec2add=func(v1,v2){
    return vec2(v1.x+v2.x,v1.y+v2.y);
}
var vec2sub=func(v1,v2){
    return vec2(v1.x-v2.x,v1.y-v2.y);
}
var vec2mul=func(v1,v2){
    return vec2(v1.x*v2.x,v1.y*v2.y);
}
var vec2div=func(v1,v2){
    return vec2(v1.x/v2.x,v1.y/v2.y);
}

var vec3=func(x,y,z){
    return {x:x,y:y,z:z};
}
var vec3add=func(v1,v2){
    return vec3(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}
var vec3sub=func(v1,v2){
    return vec3(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
}
var vec3mul=func(v1,v2){
    return vec3(v1.x*v2.x,v1.y*v2.y,v1.z*v2.z);
}
var vec3div=func(v1,v2){
    return vec3(v1.x/v2.x,v1.y/v2.y,v1.z/v2.z);
}
var vec3neg=func(v){
    return vec3(-v.x,-v.y,-v.z);
}

var clamp=func(value,_min,_max){
    return math.max(math.min(value,_max),_min);
}
var sign=func(a){
    return (0<a)-(a<0);
}
var step=func(edge,x){
    return x>edge;
}
var vec2len=func(v){
    return math.sqrt(v.x*v.x+v.y*v.y);
}
var vec3len=func(v){
    return math.sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

var vec3norm=func(v){
    var t=vec3len(v);
    return vec3div(v,vec3(t,t,t));
}
var vec3dot=func(a,b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
var vec3abs=func(v){
    return vec3(math.abs(v.x),math.abs(v.y),math.abs(v.z));
}
var vec3sign=func(v){
    return vec3(sign(v.x),sign(v.y),sign(v.z));
}
var vec3step=func(edge,v){
    return vec3(step(edge.x,v.x),step(edge.y,v.y),step(edge.z,v.z));
}
var vec3reflect=func(rd,n){
    var d=vec3dot(n,rd);
    return vec3sub(rd,vec3mul(n,vec3mul(vec3(2,2,2),vec3(d,d,d))));
}

var rotateX=func(a,angle){
    var b=vec3(a.x,a.y,a.z);
    b.z=a.z*math.cos(angle)-a.y*math.sin(angle);
    b.y=a.z*math.sin(angle)+a.y*math.cos(angle);
    return b;
}

var rotateY=func(a,angle)
{
    var b=vec3(a.x,a.y,a.z);
    b.x=a.x*math.cos(angle)-a.z*math.sin(angle);
    b.z=a.x*math.sin(angle)+a.z*math.cos(angle);
    return b;
}

var rotateZ=func(a,angle)
{
    var b=vec3(a.x,a.y,a.z);
    b.x=a.x*math.cos(angle)-a.y*math.sin(angle);
    b.y=a.x*math.sin(angle)+a.y*math.cos(angle);
    return b;
}

var sphere=func(ro,rd,r) {
    var b=vec3dot(ro,rd);
    var c=vec3dot(ro,ro)-r*r;
    var h=b*b-c;
    if(h<0.0) return vec2(-1.0,-1.0);
    h=math.sqrt(h);
    return vec2(-b-h,-b+h);
}

var box=func(ro,rd,boxSize,outNormal) {
    var m=vec3div(vec3(1.0,1.0,1.0),rd);
    var n=vec3mul(m,ro);
    var k=vec3mul(vec3abs(m),boxSize);
    var t1=vec3sub(vec3neg(n),k);
    var t2=vec3add(vec3neg(n),k);
    var tN=math.max(math.max(t1.x,t1.y),t1.z);
    var tF=math.min(math.min(t2.x,t2.y),t2.z);
    if (tN>tF or tF<0.0) return vec2(-1.0,-1.0);
    var yzx=vec3(t1.y,t1.z,t1.x);
    var zxy=vec3(t1.z,t1.x,t1.y);
    var tmp=vec3mul(vec3mul(vec3neg(vec3sign(rd)), vec3step(yzx,t1)),vec3step(zxy, t1));
    outNormal.x=tmp.x;
    outNormal.y=tmp.y;
    outNormal.z=tmp.z;
    return vec2(tN, tF);
}

var plane=func(ro,rd,p,w) {
    return -(vec3dot(ro,p)+w)/vec3dot(rd,p);
}

var main=func() {
	var width=68*2;
	var height=17*2;

	var aspect=width/height;
	var pixelAspect=11.0/24.0;

	var gradient=split(""," .:!/r(l1Z4H9W8$@");
	var gradientSize=size(gradient)-2;

    var screen=[];
    setsize(screen,width*height);

    print("\e[2J");
    var stamp=maketimestamp();
	for(var t=0;t<10000;t+=1){
        stamp.stamp();
		var light=vec3norm(vec3(-0.5,0.5,-1.0));
		var spherePos=vec3(0,3,0);
		for(var i=0;i<width;i+=1){
			for(var j=0;j<height;j+=1){
				var uv=vec2sub(vec2mul(vec2div(vec2(i,j),vec2(width,height)),vec2(2.0,2.0)),vec2(1.0,1.0));
				uv.x*=aspect*pixelAspect;
				var ro=vec3(-6,0,0);
				var rd=vec3norm(vec3(2,uv.x,uv.y));
				ro=rotateY(ro,0.25);
				rd=rotateY(rd,0.25);
				ro=rotateZ(ro,t*0.025);
				rd=rotateZ(rd,t*0.025);
				var diff = 1;
				for (var k=0;k<5;k+=1){
					var minIt=99999;
					var intersection=sphere(vec3sub(ro,spherePos),rd,1);
					var n=vec3(0,0,0);
					var albedo=1;
					if(intersection.x>0){
						var itPoint=vec3add(vec3sub(ro,spherePos),vec3mul(rd,vec3(intersection.x,intersection.x,intersection.x)));
						minIt= intersection.x;
						n=vec3norm(itPoint);
					}
					var boxN=vec3(0,0,0);
					intersection=box(ro,rd,vec3(1,1,1),boxN);
					if(intersection.x>0 and intersection.x<minIt){
						minIt=intersection.x;
						n=boxN;
					}
                    var tmp=plane(ro,rd,vec3(0,0,-1),1);
					intersection=vec2(tmp,tmp);
					if(intersection.x>0 and intersection.x<minIt){
						minIt=intersection.x;
						n=vec3(0,0,-1);
						albedo=0.5;
					}
					if(minIt<99999){
						diff*=(vec3dot(n,light)*0.5+0.5)*albedo;
						ro=vec3add(ro,vec3mul(rd,vec3(minIt-0.01,minIt-0.01,minIt-0.01)));
						rd=vec3reflect(rd, n);
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
        println("\e[H",1/elt," fps\n",s);
        
	}
}

main();