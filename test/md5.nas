import("lib.nas");


var check=func(x){
    return x-int(x/0x100000000)*0x100000000;
}
var bits_and=func(x,y){
    x=check(x);
    y=check(y);
    var (res,op)=(0,1);
    for(var i=0;i<32;i+=1){
        var (tmpx,tmpy)=(x-int(x/2)*2,y-int(y/2)*2);
        res+=op*(tmpx==1 and tmpy==1);
        x=int(x/2);
        y=int(y/2);
        op*=2;
    }
    return res;
}
var bits_or=func(x,y){
    x=check(x);
    y=check(y);
    var (res,op)=(0,1);
    for(var i=0;i<32;i+=1){
        var (tmpx,tmpy)=(x-int(x/2)*2,y-int(y/2)*2);
        res+=op*(tmpx==1 or tmpy==1);
        x=int(x/2);
        y=int(y/2);
        op*=2;
    }
    return res;
}
var bits_xor=func(x,y){
    x=check(x);
    y=check(y);
    var (res,op)=(0,1);
    for(var i=0;i<32;i+=1){
        var (tmpx,tmpy)=(x-int(x/2)*2,y-int(y/2)*2);
        res+=op*(tmpx!=tmpy);
        x=int(x/2);
        y=int(y/2);
        op*=2;
    }
    return res;
}
var bits_not=func(x){
    x=check(x);
    var (res,op)=(0,1);
    for(var i=0;i<32;i+=1){
        res+=op*((x-int(x/2)*2)==1?0:1);
        x=int(x/2);
        op*=2;
    }
    return res;
}

var hex32str=func(num){
    var ch="0123456789abcdef";
    var res="";
    for(var i=0;i<4;i+=1){
        var tmp="";
        for(var j=0;j<2;j+=1){
            tmp=chr(ch[bits_and(num,0x0f)])~tmp;
            num=int(num/16);
        }
        res~=tmp;
    }
    return res;
}
var md5=func(s){

    var K=[
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    ];

    var S=[
        7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
        5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
        6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
    ];
    var idx=[
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
        1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,
        5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,
        0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9
    ];

    var l=func(num,cx){
        for(var i=0;i<cx;i+=1){
            num=check(num*2);
        }
        return num;
    }
    var r=func(num,cx){
        num=check(num);
        for(var i=0;i<cx;i+=1){
            num=num/2;
        }
        return int(num);
    }
    var rol=func(num,cx){
        return bits_or(l(num,cx),r(num,32-cx));
    }
    # round 1
    var F=func(x,y,z){
        return bits_or(
            bits_and(x,y),
            bits_and(bits_not(x),z)
        );
    }
    # round 2
    var G=func(x,y,z){
        return bits_or(
            bits_and(x,z),
            bits_and(y,bits_not(z))
        );
    }
    # round 3
    var H=func(x,y,z){
        return bits_xor(bits_xor(x,y),z);
    }
    # round 4
    var I=func(x,y,z){
        return bits_xor(
            y,
            bits_or(x,bits_not(z))
        );
    }

    var len=size(s)*8;
    var res=[];
    var v=[128,64,32,16,8,4,2,1];
    for(var i=0;i<size(s);i+=1){
        var c=s[i];
        foreach(var j;v){
            append(res,bits.bitand(c,j)?1:0);
        }
    }
    # +------len------+--1~512--+--64--+
    # |      text     |  fill   | size |
    # +---------------+---------+------+ N*512
    var mod=size(res)-int(size(res)/512)*512;
    if(mod==448){
        append(res,1);
        for(var i=0;i<511;i+=1)
            append(res,0);
    }elsif(mod<448){
        append(res,1);
        mod+=1;
        for(;mod<448;mod+=1)
            append(res,0);
    }elsif(mod>448){
        append(res,1);
        mod+=1;
        # 512+448=960
        for(;mod<960;mod+=1)
            append(res,0);
    }
    var (tmp,cnt,t)=([],0,0);
    foreach(var i;res){
        if(!cnt)
            t=i;
        else
            t=t*2+i;
        cnt+=1;
        if(cnt==8){
            cnt=0;
            append(tmp,t);
        }
    }

    # little endian, this may have number overflow bug
    # if the number is too large
    var (lower32,higher32)=(check(len),check(len/math.pow(2,32)));
    for(var i=0;i<4;i+=1){
        append(tmp,int(lower32-int(lower32/256)*256));
        lower32=int(lower32/256);
    }
    for(var i=0;i<4;i+=1){
        append(tmp,int(higher32-int(higher32/256)*256));
        higher32=int(higher32/256);
    }
    res=tmp;

    # 1 block=>16 uint32=>64 byte=>512 bit
    tmp=[];
    for(var i=0;i<size(res);i+=4){
        append(tmp,
            res[i+3]*math.pow(2,24)+
            res[i+2]*math.pow(2,16)+
            res[i+1]*math.pow(2,8)+
            res[i]
        );
    }
    res=tmp;

    var A=0x67452301;
    var B=0xefcdab89;
    var C=0x98badcfe;
    var D=0x10325476;

    for(var i=0;i<size(res);i+=16){
        var (f,a,b,c,d)=(0,A,B,C,D);
        for(var j=0;j<64;j+=1){
            if(j<16){
                f=F(b,c,d);
            }elsif(j<32){
                f=G(b,c,d);
            }elsif(j<48){
                f=H(b,c,d);
            }else{
                f=I(b,c,d);
            }

            var tmp=d;
            d=c;
            c=b;
            b=check(b+rol(a+f+K[j]+res[idx[j]],S[j]));
            a=tmp;
        }
        A=check(a+A);
        B=check(b+B);
        C=check(c+C);
        D=check(d+D);
    }
    return hex32str(A)~hex32str(B)~hex32str(C)~hex32str(D);
}

func(){
    var test_set=[
        "github.com",
        "helloworld",
        "abc",
        "https://www.github.com/ValKmjolnir/Nasal-Interpreter",
        "https://github.com/andyross/nasal",
        "var (lower32,higher32)=(check(len),check(len/math.pow(2,32)));",
        "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
    ];
    var result=[
        "99cd2175108d157588c04758296d1cfc",
        "fc5e038d38a57032085441e7fe7010b0",
        "900150983cd24fb0d6963f7d28e17f72",
        "6b3a7bbc2240046c4fb1b0b3a4ed8181",
        "14a6afca5f3a7b239c56b5a9678c428e",
        "f499377c9ae8454c6c8a21ddba7f00de",
        "fdacad297f72956e0619002cecffc8e3"
    ];
    forindex(var i;test_set)
        if(cmp(md5(test_set[i]),result[i]))
            println(
                "md5 cannot work:\n",
                "    test \""~test_set[i]~"\"\n",
                "    result \""~result[i]~"\"\n",
                "    but get \""~md5(test_set[i])~"\"\n"
            );
}();