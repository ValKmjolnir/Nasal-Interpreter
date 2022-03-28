#include "../nasal.h"

uint32_t strlength;
uint32_t* add(std::string str)
{
    uint32_t num=((str.length()+8)/64)+1;
    uint32_t *strByte=new uint32_t[num*16];
    strlength=num*16;
    for (uint32_t i=0;i<num*16;i++)
        strByte[i]=0;
    for (uint32_t i=0;i<str.length();i++)
        strByte[i>>2]|=(str[i])<<((i%4)*8);
    strByte[str.length()>>2]|=0x80<<(((str.length()%4))*8);
    strByte[num*16-2]=str.length()*8;
    return strByte;
}
std::string changeHex(int a)
{
    const char str16[]="0123456789abcdef";
    std::string str="";
    for(int i=0;i<4;i++)
    {
        std::string str1="";
        int b=((a>>i*8)%(1<<8))&0xff;
        for (int j = 0; j < 2; j++)
        {
            str1.insert(0,1,str16[b%16]);
            b=b/16;
        }
        str+=str1;
    }
    return str;
}
std::string md5(std::string source)
{
    // uint32_t(abs(sin(i+1))*(2pow32))
    const uint32_t k[]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
        0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
        0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
        0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
        0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
        0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
    };
    // left shift
    const uint32_t s[]={
        7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
        5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
        6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
    };
    // index
    const uint32_t idx[]={
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, // g=i
        1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12, // g=(5*i+1)%16;
        5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2, // g=(3*i+5)%16;
        0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9  // g=(7*i)%16;
    };
    uint32_t atmp=0x67452301,btmp=0xefcdab89;
    uint32_t ctmp=0x98badcfe,dtmp=0x10325476;

    uint32_t *strByte=add(source);
#define shift(x,n) (((x)<<(n))|((x)>>(32-(n)))) // cycle left shift
#define F(x,y,z) (((x)&(y))|((~x)&(z)))    
#define G(x,y,z) (((x)&(z))|((y)&(~z)))
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|(~z)))
    for(uint32_t i=0;i<strlength/16;i++)
    {
        uint32_t num[16];
        for(uint32_t j=0;j<16;j++)
            num[j]=strByte[i*16+j];
        uint32_t f,g,a=atmp,b=btmp,c=ctmp,d=dtmp;
        for(uint32_t i=0;i<64;i++)
        {
            if(i<16)      f=F(b,c,d);
            else if(i<32) f=G(b,c,d);
            else if(i<48) f=H(b,c,d);
            else          f=I(b,c,d);
            uint32_t tmp=d;
            d=c;
            c=b;
            b=b+shift((a+f+k[i]+num[idx[i]]),s[i]);
            a=tmp;
        }
        atmp+=a;
        btmp+=b;
        ctmp+=c;
        dtmp+=d;
    }
    delete []strByte;
    return changeHex(atmp).append(changeHex(btmp)).append(changeHex(ctmp)).append(changeHex(dtmp));
}
extern "C" nasal_ref nas_md5(std::vector<nasal_ref>& args,nasal_gc& gc){
    if(!args.size())
        return builtin_err("md5","lack arguments");
    nasal_ref str=args[0];
    if(str.type!=vm_str)
        return builtin_err("md5","\"str\" must be a string");
    nasal_ref res=gc.alloc(vm_str);
    res.str()=md5(str.str());
    return res;
}