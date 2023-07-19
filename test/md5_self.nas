var check=func(x){
    if(x<0x100000000)
        return x;
    return x-floor(x/0x100000000)*0x100000000;
}
var u32_bits_and=func(x,y){
    return bits.u32_and(check(x),check(y));
}
var u32_bits_or=func(x,y){
    return bits.u32_or(check(x),check(y));
}
var u32_bits_xor=func(x,y){
    return bits.u32_xor(check(x),check(y));
}
var u32_bits_not=func(x){
    return bits.u32_not(check(x));
}

var hex32str=func(){
    var ch=["0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"];
    var tbl=[];
    setsize(tbl,256);
    for(var i=0;i<16;i+=1){
        for(var j=0;j<16;j+=1)
            tbl[i*16+j]=ch[i]~ch[j];
    }
    return func(num){
        var res="";
        for(var i=0;i<4;i+=1){
            res~=tbl[u32_bits_and(num,0xff)];
            num=floor(num/256);
        }
        return res;
    };
}();

var _md5=func(){
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
        return floor(num);
    }
    var rol=func(num,cx){
        return u32_bits_or(l(num,cx),r(num,32-cx));
    }
    # round 1
    var F=func(x,y,z){
        return u32_bits_or(
            u32_bits_and(x,y),
            u32_bits_and(u32_bits_not(x),z)
        );
    }
    # round 2
    var G=func(x,y,z){
        return u32_bits_or(
            u32_bits_and(x,z),
            u32_bits_and(y,u32_bits_not(z))
        );
    }
    # round 3
    var H=func(x,y,z){
        return u32_bits_xor(u32_bits_xor(x,y),z);
    }
    # round 4
    var I=func(x,y,z){
        return u32_bits_xor(
            y,
            u32_bits_or(x,u32_bits_not(z))
        );
    }
    var functions=[
        F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,
        G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,
        H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,H,
        I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I
    ];

    return func(s){
        var (s_size,len,res)=(size(s),size(s)*8,[]);
        setsize(res,s_size);
        for(var i=0;i<s_size;i+=1){
            res[i]=s[i];
        }
        # +------len------+--1~512--+--64--+
        # |      text     |  fill   | size |
        # +---------------+---------+------+ N*512 bit
        var (mod,res_size)=(s_size-floor(s_size/64)*64,0);
        if(mod==56){
            res_size=s_size+64;
        }elsif(mod<56){
            res_size=floor(s_size/64)*64+56;
        }elsif(mod>56){
            res_size=floor(s_size/64)*64+120; # 512+448=960 960/8=120
        }
        setsize(res,res_size);
        res[s_size]=0x80;
        for(var i=s_size+1;i<res_size;i+=1)
            res[i]=0;

        # little endian
        setsize(res,size(res)+8);
        var (s_size,lower32,higher32)=(size(res),check(len),check(len/math.pow(2,32)));
        for(var i=4;i>0;i-=1){
            res[s_size-4-i]=floor(lower32-floor(lower32/256)*256);
            lower32=floor(lower32/256);
        }
        for(var i=4;i>0;i-=1){
            res[s_size-i]=floor(higher32-floor(higher32/256)*256);
            higher32=floor(higher32/256);
        }

        # 1 block=>16 uint32=>64 byte=>512 bit
        # because using double to discribe number
        # this may only work when string's length is under 1<<51
        var tmp=[];
        setsize(tmp,size(res)/4);
        for(var i=0;i<size(res);i+=4){
            tmp[i/4]=res[i+3]*math.pow(2,24)+
                res[i+2]*math.pow(2,16)+
                res[i+1]*math.pow(2,8)+
                res[i];
        }
        res=tmp;

        var A=0x67452301;
        var B=0xefcdab89;
        var C=0x98badcfe;
        var D=0x10325476;

        res_size=size(res);
        for(var i=0;i<res_size;i+=16){
            var (f,a,b,c,d)=(0,A,B,C,D);
            for(var j=0;j<64;j+=1){
                f=functions[j](b,c,d);
                (a,b,c,d)=(d,check(b+rol(a+f+K[j]+res[i+idx[j]],S[j])),b,c);
            }
            (A,B,C,D)=(check(a+A),check(b+B),check(c+C),check(d+D));
        }
        return hex32str(A)~hex32str(B)~hex32str(C)~hex32str(D);
    };
}();

# check if md5 runs correctly
var md5check=func(){
    var test_set=[
        "md5",
        "github.com",
        "helloworld",
        "abc",
        "https://www.github.com/ValKmjolnir/Nasal-Interpreter",
        "https://github.com/andyross/nasal",
        "var (lower32,higher32)=(check(len),check(len/math.pow(2,32)));",
        "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",
        "let the bass kick",
        "f499377c9ae8454c6c8a21ddba7f00de5817fccdc611333ed004d826abb17f4efdacad297f72956e0619002cecffc8e3d18d9b03b082f3cb114bc29173954043",
        "you are our last hope"
    ];
    var result=[
        "1bc29b36f623ba82aaf6724fd3b16718",
        "99cd2175108d157588c04758296d1cfc",
        "fc5e038d38a57032085441e7fe7010b0",
        "900150983cd24fb0d6963f7d28e17f72",
        "6b3a7bbc2240046c4fb1b0b3a4ed8181",
        "14a6afca5f3a7b239c56b5a9678c428e",
        "f499377c9ae8454c6c8a21ddba7f00de",
        "fdacad297f72956e0619002cecffc8e3",
        "16eadccb9799dfb4c1ca512f40638bbb",
        "a7916c5ce54e73b7ddf6a286b36d976d",
        "ec6d5b197ba019db23c719112f3f70b7"
    ];
    forindex(var i;test_set){
        var res=_md5(test_set[i]);
        if(cmp(res,result[i]))
            println(
                "md5 cannot work:\n",
                "    test \""~test_set[i]~"\"\n",
                "    result \""~result[i]~"\"\n",
                "    but get \""~res~"\"\n"
            );
    }
}

# check when loading md5.nas
md5check();