import("lib.nas");

var md5=func(s){
    var K=[
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
        0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
        0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
        0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
        0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
        0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
        0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
        0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
        0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
        0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
        0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
        0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
    ];
    var S=[
        7,12,17,22,
        7,12,17,22,
        7,12,17,22,
        7,12,17,22,
        5,9,14,20,
        5,9,14,20,
        5,9,14,20,
        5,9,14,20,
        4,11,16,23,
        4,11,16,23,
        4,11,16,23,
        4,11,16,23,
        6,10,15,21,
        6,10,15,21,
        6,10,15,21,
        6,10,15,21
    ]
    var l=func(num,cx){
        for(var i=0;i<cx;i+=1)
            num=bits.bitand(0xffffffff,num*2);
        return num;
    }
    var r=func(num,cx){
        for(var i=0;i<cx;i+=1)
            num=int(num/2);
        return num;
    }
    var rol=func(num,cx){
        if(cx>32)
            cx=cx-int(cx/32)*32;
        return bits.bitor(l(num,cx),r(num,32-cx));
    }
    var F=func(x,y,z){
        return bits.bitor(
            bits.bitand(x,y),
            bits.bitand(bits.bitnot(x),z)
        );
    }
    var G=func(x,y,z){
        return bits.bitor(
            bits.bitand(x,z),
            bits.bitand(y,bits.bitnot(z))
        );
    }
    var H=func(x,y,z){
        return bits.bitxor(bits.bitxor(x,y),z);
    }
    var I=func(x,y,z){
        return bits.bitor(
            bits.bitxor(x,y),
            bits.bitnot(z)
        );
    }

    var len=size(s);
    var res=[];
    var v=[256,128,64,32,16,8,4,2,1];
    for(var i=0;i<size(s);i+=1){
        var c=s[i];
        foreach(var j;v){
            append(res,bits.bitand(c,j)?1:0);
        }
    }
    var mod=size(res)-int(size(res)/512)*512;
    if(mod==0){
        append(res,1);
        mod+=1;
        for(;mod<448;mod+=1)
            append(res,0);
    }elsif(mod<448){
        append(res,1);
        mod+=1;
        for(;mod<448;mod+=1)
            append(res,0);
    }elsif(mod>448){
        append(res,1);
        mod+=1;
        for(;mod<512+448;mod+=1)
            append(res,0);
    }
    var tmp=[];

    var A=0x01234567;
    var B=0x89abcdef;
    var C=0xfedcba98;
    var D=0x76543210;
    println(res,' ',size(res));
}

md5("helloworld");