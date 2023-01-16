#pragma once

#include "nasal.h"
#include "nasal_gc.h"

#ifndef _MSC_VER
#include <unistd.h>
#include <dirent.h>
#else
#pragma warning (disable:4566) // i know i'm using utf-8, fuck you
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#include <io.h>
#include <direct.h>
#endif

#include <sstream>
#include <cmath>
#include <thread>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/wait.h>
#endif

#if defined __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

var builtin_print(var* local,gc& ngc) {
    for(auto& i:local[1].vec().elems) {
        std::cout<<i;
    }
    std::cout<<std::flush;
    return nil;
}

var builtin_println(var* local,gc& ngc) {
    for(auto& i:local[1].vec().elems) {
        std::cout<<i;
    }
    std::cout<<std::endl;
    return nil;
}

var builtin_exit(var* local,gc& ngc) {
    std::exit(local[1].num());
    return nil;
}

var builtin_abort(var* local,gc& ngc) {
    std::abort();
    return nil;
}

var builtin_append(var* local,gc& ngc) {
    var vec=local[1];
    var elem=local[2];
    if (vec.type!=vm_vec) {
        return nas_err("append","\"vec\" must be vector");
    }
    auto& v=vec.vec().elems;
    for(auto& i:elem.vec().elems) {
        v.push_back(i);
    }
    return nil;
}

var builtin_setsize(var* local,gc& ngc) {
    var vec=local[1];
    var size=local[2];
    if (vec.type!=vm_vec) {
        return nas_err("setsize","\"vec\" must be vector");
    }
    if (size.type!=vm_num || size.num()<0) {
        return nil;
    }
    vec.vec().elems.resize((i64)size.num(),nil);
    return nil;
}

var builtin_system(var* local,gc& ngc) {
    var str=local[1];
    if (str.type!=vm_str) {
        return var::num(-1);
    }
    return var::num((f64)system(str.str().c_str()));
}

var builtin_input(var* local,gc& ngc) {
    var end=local[1];
    var ret=ngc.alloc(vm_str);
    if (end.type!=vm_str || end.str().length()>1 || !end.str().length()) {
        std::cin>>ret.str();
    } else {
        std::getline(std::cin,ret.str(),end.str()[0]);
    }
    return ret;
}

var builtin_fin(var* local,gc& ngc) {
    var val=local[1];
    if (val.type!=vm_str) {
        return nas_err("io::fin","\"filename\" must be string");
    }
    std::ifstream in(val.str(),std::ios::binary);
    std::stringstream rd;
    if (!in.fail()) {
        rd<<in.rdbuf();
    }
    return ngc.newstr(rd.str());
}

var builtin_fout(var* local,gc& ngc) {
    var val=local[1];
    var str=local[2];
    if (val.type!=vm_str) {
        return nas_err("io::fout","\"filename\" must be string");
    }
    std::ofstream out(val.str());
    if (out.fail()) {
        return nas_err("io::fout","cannot open <"+val.str()+">");
    }
    out<<str;
    return nil;
}

var builtin_split(var* local,gc& ngc) {
    var delimeter=local[1];
    var str=local[2];
    if (delimeter.type!=vm_str) {
        return nas_err("split","\"separator\" must be string");
    }
    if (str.type!=vm_str) {
        return nas_err("split","\"str\" must be string");
    }
    const string& deli=delimeter.str();
    const string& s=str.str();

    // avoid being sweeped
    var res=ngc.temp=ngc.alloc(vm_vec);
    auto& vec=res.vec().elems;

    if (!deli.length()) {
        for(auto i:s) {
            vec.push_back(ngc.newstr(i));
        }
        ngc.temp=nil;
        return res;
    }
    usize last=0,pos=s.find(deli,0);
    while(pos!=string::npos) {
        if (pos>last) {
            vec.push_back(ngc.newstr(s.substr(last,pos-last)));
        }
        last=pos+deli.length();
        pos=s.find(deli,last);
    }
    if (last!=s.length()) {
        vec.push_back(ngc.newstr(s.substr(last)));
    }
    ngc.temp=nil;
    return res;
}

var builtin_rand(var* local,gc& ngc) {
    var val=local[1];
    if (val.type!=vm_num && val.type!=vm_nil) {
        return nas_err("rand","\"seed\" must be nil or number");
    }
    if (val.type==vm_num) {
        srand((u32)val.num());
        return nil;
    }
    f64 num=0;
    for(u32 i=0;i<5;++i) {
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    }
    return var::num(num);
}

var builtin_id(var* local,gc& ngc) {
    var val=local[1];
    std::stringstream ss;
    ss<<"0";
    if (val.type>vm_num) {
        ss<<"x"<<std::hex<<(u64)val.val.gcobj<<std::dec;
    }
    return ngc.newstr(ss.str());
}

var builtin_int(var* local,gc& ngc) {
    var val=local[1];
    if (val.type!=vm_num && val.type!=vm_str) {
        return nil;
    }
    return var::num(f64((i32)val.tonum()));
}

var builtin_floor(var* local,gc& ngc) {
    var val=local[1];
    return var::num(std::floor(val.num()));
}

var builtin_num(var* local,gc& ngc) {
    var val=local[1];
    if (val.type==vm_num) {
        return val;
    }
    if (val.type!=vm_str) {
        return nil;
    }
    f64 res=val.tonum();
    if (std::isnan(res)) {
        return nil;
    }
    return var::num(res);
}

var builtin_pop(var* local,gc& ngc) {
    var val=local[1];
    if (val.type!=vm_vec) {
        return nas_err("pop","\"vec\" must be vector");
    }
    auto& vec=val.vec().elems;
    if (vec.size()) {
        var tmp=vec.back();
        vec.pop_back();
        return tmp;
    }
    return nil;
}

var builtin_str(var* local,gc& ngc) {
    return ngc.newstr(local[1].tostr());
}

var builtin_size(var* local,gc& ngc) {
    var val=local[1];
    f64 num=0;
    switch(val.type) {
        case vm_num:  num=val.num();         break;
        case vm_str:  num=val.str().length();break;
        case vm_vec:  num=val.vec().size();  break;
        case vm_hash: num=val.hash().size(); break;
    }
    return var::num(num);
}

var builtin_i32xor(var* local,gc& ngc) {
    return var::num((f64)(i32(local[1].num())^i32(local[2].num())));
}

var builtin_i32and(var* local,gc& ngc) {
    return var::num((f64)(i32(local[1].num())&i32(local[2].num())));
}

var builtin_i32or(var* local,gc& ngc) {
    return var::num((f64)(i32(local[1].num())|i32(local[2].num())));
}

var builtin_i32nand(var* local,gc& ngc) {
    return var::num((f64)(~(i32(local[1].num())&i32(local[2].num()))));
}

var builtin_i32not(var* local,gc& ngc) {
    return var::num((f64)(~i32(local[1].num())));
}

var builtin_u32xor(var* local,gc& ngc) {
    return var::num((f64)(u32(local[1].num())^u32(local[2].num())));
}

var builtin_u32and(var* local,gc& ngc) {
    return var::num((f64)(u32(local[1].num())&u32(local[2].num())));
}

var builtin_u32or(var* local,gc& ngc) {
    return var::num((f64)(u32(local[1].num())|u32(local[2].num())));
}

var builtin_u32nand(var* local,gc& ngc) {
    return var::num((f64)(u32)(~(u32(local[1].num())&u32(local[2].num()))));
}

var builtin_u32not(var* local,gc& ngc) {
    return var::num((f64)(u32)(~u32(local[1].num())));
}

var builtin_pow(var* local,gc& ngc) {
    var x=local[1];
    var y=local[2];
    if (x.type!=vm_num || y.type!=vm_num) {
        return var::num(std::nan(""));
    }
    return var::num(std::pow(x.num(),y.num()));
}

var builtin_sin(var* local,gc& ngc) {
    var val=local[1];
    return var::num(val.type==vm_num?sin(val.num()):std::nan(""));
}

var builtin_cos(var* local,gc& ngc) {
    var val=local[1];
    return var::num(val.type==vm_num?cos(val.num()):std::nan(""));
}

var builtin_tan(var* local,gc& ngc) {
    var val=local[1];
    return var::num(val.type==vm_num?tan(val.num()):std::nan(""));
}

var builtin_exp(var* local,gc& ngc) {
    var val=local[1];
    return var::num(val.type==vm_num?exp(val.num()):std::nan(""));
}

var builtin_lg(var* local,gc& ngc) {
    var val=local[1];
    return var::num(val.type==vm_num?log(val.num())/log(10.0):std::nan(""));
}

var builtin_ln(var* local,gc& ngc) {
    var val=local[1];
    return var::num(val.type==vm_num?log(val.num()):std::nan(""));
}

var builtin_sqrt(var* local,gc& ngc) {
    var val=local[1];
    return var::num(val.type==vm_num?sqrt(val.num()):std::nan(""));
}

var builtin_atan2(var* local,gc& ngc) {
    var x=local[1];
    var y=local[2];
    if (x.type!=vm_num || y.type!=vm_num) {
        return var::num(std::nan(""));
    }
    return var::num(atan2(y.num(),x.num()));
}

var builtin_isnan(var* local,gc& ngc) {
    var x=local[1];
    return (x.type==vm_num && std::isnan(x.num()))?one:zero;
}

var builtin_time(var* local,gc& ngc) {
    var val=local[1];
    if (val.type!=vm_num) {
        return nas_err("time","\"begin\" must be number");
    }
    time_t begin=(time_t)val.num();
    return var::num((f64)time(&begin));
}

var builtin_contains(var* local,gc& ngc) {
    var hash=local[1];
    var key=local[2];
    if (hash.type!=vm_hash || key.type!=vm_str) {
        return zero;
    }
    return hash.hash().elems.count(key.str())?one:zero;
}

var builtin_delete(var* local,gc& ngc) {
    var hash=local[1];
    var key=local[2];
    if (hash.type!=vm_hash) {
        return nas_err("delete","\"hash\" must be hash");
    }
    if (key.type!=vm_str) {
        return nil;
    }
    if (hash.hash().elems.count(key.str())) {
        hash.hash().elems.erase(key.str());
    }
    return nil;
}

var builtin_keys(var* local,gc& ngc) {
    var hash=local[1];
    if (hash.type!=vm_hash) {
        return nas_err("keys","\"hash\" must be hash");
    }
    // avoid being sweeped
    var res=ngc.temp=ngc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    for(auto& iter:hash.hash().elems) {
        vec.push_back(ngc.newstr(iter.first));
    }
    ngc.temp=nil;
    return res;
}

var builtin_die(var* local,gc& ngc) {
    return nas_err("error",local[1].tostr());
}

var builtin_find(var* local,gc& ngc) {
    var needle=local[1];
    var haystack=local[2];
    usize pos=haystack.tostr().find(needle.tostr());
    if (pos==string::npos) {
        return var::num((f64)-1);
    }
    return var::num((f64)pos);
}

var builtin_type(var* local,gc& ngc) {
    switch(local[1].type) {
        case vm_none: return ngc.newstr("undefined");break;
        case vm_nil:  return ngc.newstr("nil");      break;
        case vm_num:  return ngc.newstr("num");      break;
        case vm_str:  return ngc.newstr("str");      break;
        case vm_vec:  return ngc.newstr("vec");      break;
        case vm_hash: return ngc.newstr("hash");     break;
        case vm_func: return ngc.newstr("func");     break;
        case vm_obj:  return ngc.newstr("obj");      break;
        case vm_co:   return ngc.newstr("coroutine");break;
    }
    return nil;
}

var builtin_substr(var* local,gc& ngc) {
    var str=local[1];
    var beg=local[2];
    var len=local[3];
    if (str.type!=vm_str) {
        return nas_err("substr","\"str\" must be string");
    }
    if (beg.type!=vm_num || beg.num()<0) {
        return nas_err("substr","\"begin\" should be number >= 0");
    }
    if (len.type!=vm_num || len.num()<0) {
        return nas_err("substr","\"length\" should be number >= 0");
    }
    usize begin=(usize)beg.num();
    usize length=(usize)len.num();
    if (begin>=str.str().length()) {
        return nas_err("susbtr","begin index out of range: "+std::to_string(begin));
    }
    return ngc.newstr(str.str().substr(begin,length));
}

var builtin_streq(var* local,gc& ngc) {
    var a=local[1];
    var b=local[2];
    return var::num(f64((a.type!=vm_str || b.type!=vm_str)?0:(a.str()==b.str())));
}

var builtin_left(var* local,gc& ngc) {
    var str=local[1];
    var len=local[2];
    if (str.type!=vm_str) {
        return nas_err("left","\"string\" must be string");
    }
    if (len.type!=vm_num) {
        return nas_err("left","\"length\" must be number");
    }
    if (len.num()<0) {
        return ngc.newstr("");
    }
    return ngc.newstr(str.str().substr(0,len.num()));
}

var builtin_right(var* local,gc& ngc) {
    var str=local[1];
    var len=local[2];
    if (str.type!=vm_str) {
        return nas_err("right","\"string\" must be string");
    }
    if (len.type!=vm_num) {
        return nas_err("right","\"length\" must be number");
    }
    i32 length=(i32)len.num();
    i32 srclen=str.str().length();
    if (length>srclen) {
        length=srclen;
    }
    if (length<0) {
        length=0;
    }
    return ngc.newstr(str.str().substr(srclen-length,srclen));
}

var builtin_cmp(var* local,gc& ngc) {
    var a=local[1];
    var b=local[2];
    if (a.type!=vm_str || b.type!=vm_str) {
        return nas_err("cmp","\"a\" and \"b\" must be string");
    }
    return var::num((f64)strcmp(a.str().c_str(),b.str().c_str()));
}

var builtin_chr(var* local,gc& ngc) {
    const char* extend[]={
        "€"," ","‚","ƒ","„","…","†","‡",
        "ˆ","‰","Š","‹","Œ"," ","Ž"," ",
        " ","‘","’","“","”","•","–","—",
        "˜","™","š","›","œ"," ","ž","Ÿ",
        " ","¡","¢","£","¤","¥","¦","§",
        "¨","©","ª","«","¬"," ","®","¯",
        "°","±","²","³","´","µ","¶","·",
        "¸","¹","º","»","¼","½","¾","¿",
        "À","Á","Â","Ã","Ä","Å","Æ","Ç",
        "È","É","Ê","Ë","Ì","Í","Î","Ï",
        "Ð","Ñ","Ò","Ó","Ô","Õ","Ö","×",
        "Ø","Ù","Ú","Û","Ü","Ý","Þ","ß",
        "à","á","â","ã","ä","å","æ","ç",
        "è","é","ê","ë","ì","í","î","ï",
        "ð","ñ","ò","ó","ô","õ","ö","÷",
        "ø","ù","ú","û","ü","ý","þ","ÿ"
    };
    i32 num=local[1].num();
    if (0<=num && num<128) {
        return ngc.newstr((char)num);
    } else if (128<=num && num<256) {
        return ngc.newstr(extend[num-128]);
    }
    return ngc.newstr(" ");
}

var builtin_char(var* local,gc& ngc) {
    return ngc.newstr((unsigned char)local[1].num());
}

var builtin_values(var* local,gc& ngc) {
    var hash=local[1];
    if (hash.type!=vm_hash) {
        return nas_err("values","\"hash\" must be hash");
    }
    var vec=ngc.alloc(vm_vec);
    auto& v=vec.vec().elems;
    for(auto& i:hash.hash().elems) {
        v.push_back(i.second);
    }
    return vec;
}

var builtin_exists(var* local,gc& ngc) {
    if (local[1].type!=vm_str) {
        return zero;
    }
    return access(local[1].str().c_str(),F_OK)!=-1?one:zero;
}

var builtin_open(var* local,gc& ngc) {
    var name=local[1];
    var mode=local[2];
    if (name.type!=vm_str) {
        return nas_err("open","\"filename\" must be string");
    }
    if (mode.type!=vm_str) {
        return nas_err("open","\"mode\" must be string");
    }
    FILE* res=fopen(name.str().c_str(),mode.str().c_str());
    if (!res) {
        return nas_err("open","failed to open file <"+name.str()+">");
    }
    var ret=ngc.alloc(vm_obj);
    ret.obj().set(nas_obj::file,res);
    return ret;
}

var builtin_close(var* local,gc& ngc) {
    var fd=local[1];
    if (!fd.objchk(nas_obj::file)) {
        return nas_err("close","not a valid filehandle");
    }
    fd.obj().clear();
    return nil;
}

var builtin_read(var* local,gc& ngc) {
    var fd=local[1];
    var buf=local[2];
    var len=local[3];
    if (!fd.objchk(nas_obj::file)) {
        return nas_err("read","not a valid filehandle");
    }
    if (buf.type!=vm_str || buf.val.gcobj->unmut) {
        return nas_err("read","\"buf\" must be mutable string");
    }
    if (len.type!=vm_num) {
        return nas_err("read","\"len\" must be number");
    }
    if (len.num()<=0 || len.num()>=(1<<30)) {
        return nas_err("read","\"len\" less than 1 or too large");
    }
    char* buff=new char[(usize)len.num()+1];
    if (!buff) {
        return nas_err("read","malloc failed");
    }
    f64 res=fread(buff,1,len.num(),(FILE*)fd.obj().ptr);
    buf.str()=buff;
    buf.val.gcobj->unmut=true;
    delete []buff;
    return var::num(res);
}

var builtin_write(var* local,gc& ngc) {
    var fd=local[1];
    var str=local[2];
    if (!fd.objchk(nas_obj::file)) {
        return nas_err("write","not a valid filehandle");
    }
    if (str.type!=vm_str) {
        return nas_err("write","\"str\" must be string");
    }
    return var::num((f64)fwrite(str.str().c_str(),1,str.str().length(),(FILE*)fd.obj().ptr));
}

var builtin_seek(var* local,gc& ngc) {
    var fd=local[1];
    var pos=local[2];
    var whence=local[3];
    if (!fd.objchk(nas_obj::file)) {
        return nas_err("seek","not a valid filehandle");
    }
    return var::num((f64)fseek((FILE*)fd.obj().ptr,pos.num(),whence.num()));
}

var builtin_tell(var* local,gc& ngc) {
    var fd=local[1];
    if (!fd.objchk(nas_obj::file)) {
        return nas_err("tell","not a valid filehandle");
    }
    return var::num((f64)ftell((FILE*)fd.obj().ptr));
}

var builtin_readln(var* local,gc& ngc) {
    var fd=local[1];
    if (!fd.objchk(nas_obj::file)) {
        return nas_err("readln","not a valid filehandle");
    }
    var str=ngc.alloc(vm_str);
    char c;
    while((c=fgetc((FILE*)fd.obj().ptr))!=EOF) {
        if (c=='\r') {
            continue;
        }
        if (c=='\n') {
            return str;
        }
        str.str()+=c;
    }
    if (str.str().length()) {
        return str;
    }
    return nil;
}

var builtin_stat(var* local,gc& ngc) {
    var name=local[1];
    if (name.type!=vm_str) {
        return nas_err("stat","\"filename\" must be string");
    }
    struct stat buf;
    if (stat(name.str().c_str(),&buf)<0) {
        return nas_err("stat","failed to open file <"+name.str()+">");
    }
    var ret=ngc.alloc(vm_vec);
    ret.vec().elems={
        var::num((f64)buf.st_dev),
        var::num((f64)buf.st_ino),
        var::num((f64)buf.st_mode),
        var::num((f64)buf.st_nlink),
        var::num((f64)buf.st_uid),
        var::num((f64)buf.st_gid),
        var::num((f64)buf.st_rdev),
        var::num((f64)buf.st_size),
        var::num((f64)buf.st_atime),
        var::num((f64)buf.st_mtime),
        var::num((f64)buf.st_ctime)
    };
    return ret;
}

var builtin_eof(var* local,gc& ngc) {
    var fd=local[1];
    if (!fd.objchk(nas_obj::file)) {
        return nas_err("readln","not a valid filehandle");
    }
    return var::num((f64)feof((FILE*)fd.obj().ptr));
}

var builtin_fld(var* local,gc& ngc) {
    // bits.fld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5)
    var str=local[1];
    var startbit=local[2];
    var length=local[3];
    if (str.type!=vm_str || str.val.gcobj->unmut) {
        return nas_err("fld","\"str\" must be mutable string");
    }
    if (startbit.type!=vm_num || length.type!=vm_num) {
        return nas_err("fld","\"startbit\",\"len\" must be number");
    }
    u32 bit=(u32)startbit.num();
    u32 len=(u32)length.num();
    if (bit+len>8*str.str().length()) {
        return nas_err("fld","bitfield out of bounds");
    }
    u32 res=0;
    auto& s=str.str();
    for(u32 i=bit;i<bit+len;++i) {
        if (s[i>>3]&(1<<(7-(i&7)))) {
            res|=1<<(bit+len-i-1);
        }
    }
    return var::num((f64)res);
}

var builtin_sfld(var* local,gc& ngc) {
    // bits.sfld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5) then this will be signed extended to
    // 11111101(-3)
    var str=local[1];
    var startbit=local[2];
    var length=local[3];
    if (str.type!=vm_str || str.val.gcobj->unmut) {
        return nas_err("sfld","\"str\" must be mutable string");
    }
    if (startbit.type!=vm_num || length.type!=vm_num) {
        return nas_err("sfld","\"startbit\",\"len\" must be number");
    }
    u32 bit=(u32)startbit.num();
    u32 len=(u32)length.num();
    if (bit+len>8*str.str().length()) {
        return nas_err("sfld","bitfield out of bounds");
    }
    u32 res=0;
    auto& s=str.str();
    for(u32 i=bit;i<bit+len;++i) {
        if (s[i>>3]&(1<<(7-(i&7)))) {
            res|=1<<(bit+len-i-1);
        }
    }
    if (res&(1<<(len-1))) {
        res|=~((1<<len)-1);
    }
    return var::num((f64)((i32)res));
}

var builtin_setfld(var* local,gc& ngc) {
    // bits.setfld(s,0,8,69);
    // set 01000101(69) to string will get this:
    // 10100010(162)
    // so s[0]=162
    var str=local[1];
    var startbit=local[2];
    var length=local[3];
    var value=local[4];
    if (str.type!=vm_str || str.val.gcobj->unmut) {
        return nas_err("setfld","\"str\" must be mutable string");
    }
    if (startbit.type!=vm_num || length.type!=vm_num || value.type!=vm_num) {
        return nas_err("setfld","\"startbit\",\"len\",\"val\" must be number");
    }
    u32 bit=(u32)startbit.num();
    u32 len=(u32)length.num();
    u64 val=(u64)value.num();
    if (bit+len>8*str.str().length()) {
        return nas_err("setfld","bitfield out of bounds");
    }
    auto& s=str.str();
    for(u32 i=bit;i<bit+len;++i) {
        if (val&(1<<(i-bit))) {
            s[i>>3]|=(1<<(7-(i&7)));
        } else {
            s[i>>3]&=~(1<<(7-(i&7)));
        }
    }
    return nil;
}

var builtin_buf(var* local,gc& ngc) {
    var length=local[1];
    if (length.type!=vm_num || length.num()<=0) {
        return nas_err("buf","\"len\" must be number greater than 0");
    }
    var str=ngc.alloc(vm_str);
    auto& s=str.str();
    s.resize(length.num(),'\0');
    return str;
}

var builtin_sleep(var* local,gc& ngc) {
    var val=local[1];
    if (val.type!=vm_num) {
        return nil;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(i64(val.num()*1e6)));
    return nil;
}

var builtin_pipe(var* local,gc& ngc) {
#ifndef _WIN32
    i32 fd[2];
    var res=ngc.alloc(vm_vec);
    if (pipe(fd)==-1) {
        return nas_err("pipe","failed to create pipe");
    }
    res.vec().elems.push_back(var::num((f64)fd[0]));
    res.vec().elems.push_back(var::num((f64)fd[1]));
    return res;
#endif
    return nas_err("pipe","not supported");
}

var builtin_fork(var* local,gc& ngc) {
#ifndef _WIN32
    f64 res=fork();
    if (res<0) {
        return nas_err("fork","failed to fork a process");
    }
    return var::num((f64)res);
#endif
    return nas_err("fork","not supported");
}

var builtin_waitpid(var* local,gc& ngc) {
    var pid=local[1];
    var nohang=local[2];
    if (pid.type!=vm_num || nohang.type!=vm_num) {
        return nas_err("waitpid","pid and nohang must be number");
    }
#ifndef _WIN32
    i32 ret_pid,status;
    ret_pid=waitpid(pid.num(),&status,nohang.num()==0?0:WNOHANG);
    var vec=ngc.alloc(vm_vec);
    vec.vec().elems.push_back(var::num((f64)ret_pid));
    vec.vec().elems.push_back(var::num((f64)status));
    return vec;
#endif
    return nas_err("waitpid","not supported");
}

var builtin_opendir(var* local,gc& ngc) {
    var path=local[1];
    if (path.type!=vm_str) {
        return nas_err("opendir","\"path\" must be string");
    }
#ifdef _MSC_VER
    WIN32_FIND_DATAA data;
    HANDLE p;
    p=FindFirstFileA((path.str()+"\\*.*").c_str(),&data);
    if (p==INVALID_HANDLE_VALUE) {
        return nas_err("opendir","cannot open dir <"+path.str()+">");
    }
#else
    DIR* p=opendir(path.str().c_str());
    if (!p) {
        return nas_err("opendir","cannot open dir <"+path.str()+">");
    }
#endif
    var ret=ngc.alloc(vm_obj);
    ret.obj().set(nas_obj::dir,p);
    return ret;
}

var builtin_readdir(var* local,gc& ngc) {
    var handle=local[1];
    if (!handle.objchk(nas_obj::dir)) {
        return nas_err("readdir","not a valid dir handle");
    }
#ifdef _MSC_VER
    WIN32_FIND_DATAA data;
    if (!FindNextFileA(handle.obj().ptr,&data)) {
        return nil;
    }
    return ngc.newstr(data.cFileName);
#else
    dirent* p=readdir((DIR*)handle.obj().ptr);
    return p?ngc.newstr(p->d_name):nil;
#endif
}

var builtin_closedir(var* local,gc& ngc) {
    var handle=local[1];
    if (!handle.objchk(nas_obj::dir)) {
        return nas_err("closedir","not a valid dir handle");
    }
    handle.obj().clear();
    return nil;
}

var builtin_chdir(var* local,gc& ngc) {
    var path=local[1];
    if (path.type!=vm_str) {
        return var::num((f64)-1);
    }
    return var::num((f64)chdir(path.str().c_str()));
}

var builtin_environ(var* local,gc& ngc) {
    var res=ngc.temp=ngc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    for(char** env=environ;*env;++env) {
        vec.push_back(ngc.newstr(*env));
    }
    ngc.temp=nil;
    return res;
}

var builtin_getcwd(var* local,gc& ngc) {
    char buf[1024];
    if (!getcwd(buf,sizeof(buf))) {
        return nil;
    }
    return ngc.newstr(buf);
}

var builtin_getenv(var* local,gc& ngc) {
    var envvar=local[1];
    if (envvar.type!=vm_str) {
        return nas_err("getenv","\"envvar\" must be string");
    }
    char* res=getenv(envvar.str().c_str());
    return res?ngc.newstr(res):nil;
}

var builtin_dlopen(var* local,gc& ngc) {
    var dlname=local[1];
    if (dlname.type!=vm_str) {
        return nas_err("dlopen","\"libname\" must be string");
    }
#ifdef _WIN32
    wchar_t* str=new wchar_t[dlname.str().size()+1];
    if (!str) {
        return nas_err("dlopen","malloc failed");
    }
    memset(str,0,sizeof(wchar_t)*dlname.str().size()+1);
    mbstowcs(str,dlname.str().c_str(),dlname.str().size()+1);
    void* ptr=LoadLibraryA(dlname.str().c_str());
    delete []str;
#else
    void* ptr=dlopen(dlname.str().c_str(),RTLD_LOCAL|RTLD_LAZY);
#endif
    if (!ptr) {
        return nas_err("dlopen","cannot open dynamic lib <"+dlname.str()+">");
    }
    var ret=ngc.temp=ngc.alloc(vm_hash);
    var lib=ngc.alloc(vm_obj);
    lib.obj().set(nas_obj::dylib,ptr);
    ret.hash().elems["lib"]=lib;

#ifdef _WIN32
    void* func=(void*)GetProcAddress((HMODULE)lib.obj().ptr,"get");
#else
    void* func=dlsym(lib.obj().ptr,"get");
#endif
    if (!func) {
        return nas_err("dlopen","cannot find <get> function");
    }
    // get function pointer by name
    mod_func* tbl=(mod_func*)((getptr)func)();
    if (!tbl) {
        return nas_err("dlopen","failed to get module functions");
    }
    for(u32 i=0;tbl[i].name;++i) {
        void* p=(void*)tbl[i].fd;
        var tmp=ngc.alloc(vm_obj);
        tmp.obj().set(nas_obj::faddr,p);
        ret.hash().elems[tbl[i].name]=tmp;
    }

    ngc.temp=nil;
    return ret;
}

var builtin_dlclose(var* local,gc& ngc) {
    var libptr=local[1];
    if (!libptr.objchk(nas_obj::dylib)) {
        return nas_err("dlclose","\"lib\" is not a valid dynamic lib");
    }
    libptr.obj().clear();
    return nil;
}

var builtin_dlcallv(var* local,gc& ngc) {
    var fp=local[1];
    var args=local[2];
    if (!fp.objchk(nas_obj::faddr)) {
        return nas_err("dlcall","\"ptr\" is not a valid function pointer");
    }
    auto& vec=args.vec().elems;
    return ((mod)fp.obj().ptr)(vec.data(),vec.size(),&ngc);
}

var builtin_dlcall(var* local,gc& ngc) {
    var fp=local[1];
    if (!fp.objchk(nas_obj::faddr)) {
        return nas_err("dlcall","\"ptr\" is not a valid function pointer");
    }
    // arguments' stored place begins at local +2
    return ((mod)fp.obj().ptr)(local+2,ngc.top-local-2,&ngc);
}

var builtin_platform(var* local,gc& ngc) {
    if (is_windows()) {
        return ngc.newstr("windows");
    } else if (is_linux()) {
        return ngc.newstr("linux");
    } else if (is_macos()) {
        return ngc.newstr("macOS");
    }
    return ngc.newstr("unknown");
}

// md5 related functions
string tohex(u32 num) {
    const char str16[]="0123456789abcdef";
    string str="";
    for(u32 i=0;i<4;i++,num>>=8) {
        string tmp="";
        for(u32 j=0,b=num&0xff;j<2;j++,b>>=4) {
            tmp.insert(0,1,str16[b&0xf]);
        }
        str+=tmp;
    }
    return str;
}

string md5(const string& src) {
    std::vector<u32> buff;
    usize num=((src.length()+8)>>6)+1;
    usize buffsize=num<<4;
    buff.resize(buffsize,0);
    for(usize i=0;i<src.length();i++) {
        buff[i>>2]|=((u8)src[i])<<((i&0x3)<<3);
    }
    buff[src.length()>>2]|=0x80<<(((src.length()%4))<<3);
    buff[buffsize-2]=(src.length()<<3)&0xffffffff;
    buff[buffsize-1]=((src.length()<<3)>>32)&0xffffffff;

    // u32(abs(sin(i+1))*(2pow32))
    const u32 k[]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
        0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
        0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
        0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
        0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
        0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
    };

    // left shift bits
    const u32 s[]={
        7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
        5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
        6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
    };

    // index
    const u32 idx[]={
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, // g=i
        1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12, // g=(5*i+1)%16;
        5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2, // g=(3*i+5)%16;
        0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9  // g=(7*i)%16;
    };
    
#define shift(x,n)  (((x)<<(n))|((x)>>(32-(n)))) // cycle left shift
#define md5f(x,y,z) (((x)&(y))|((~x)&(z)))    
#define md5g(x,y,z) (((x)&(z))|((y)&(~z)))
#define md5h(x,y,z) ((x)^(y)^(z))
#define md5i(x,y,z) ((y)^((x)|(~z)))
    
    u32 atmp=0x67452301,btmp=0xefcdab89;
    u32 ctmp=0x98badcfe,dtmp=0x10325476;
    for(u32 i=0;i<buffsize;i+=16) {
        u32 f,a=atmp,b=btmp,c=ctmp,d=dtmp;
        for(u32 j=0;j<64;j++) {
            if (j<16)      f=md5f(b,c,d);
            else if (j<32) f=md5g(b,c,d);
            else if (j<48) f=md5h(b,c,d);
            else          f=md5i(b,c,d);
            u32 tmp=d;
            d=c;
            c=b;
            b=b+shift((a+f+k[j]+buff[i+idx[j]]),s[j]);
            a=tmp;
        }
        atmp+=a;
        btmp+=b;
        ctmp+=c;
        dtmp+=d;
    }
    return tohex(atmp)+tohex(btmp)+tohex(ctmp)+tohex(dtmp);
}

var builtin_md5(var* local,gc& ngc) {
    var str=local[1];
    if (str.type!=vm_str) {
        return nas_err("md5","\"str\" must be string");
    }
    return ngc.newstr(md5(str.str()));
}

var builtin_cocreate(var* local,gc& ngc) {
    // +-------------+
    // | old pc      | <- top[0]
    // +-------------+
    // | old localr  | <- top[-1]
    // +-------------+
    // | old upvalr  | <- top[-2]
    // +-------------+
    // | local scope |
    // | ...         |
    // +-------------+ <- local pointer stored in localr
    // | old funcr   | <- old function stored in funcr
    // +-------------+
    var func=local[1];
    if (func.type!=vm_func) {
        return nas_err("coroutine::create","must use a function to create coroutine");
    }
    if (ngc.cort) {
        return nas_err("coroutine::create","cannot create another coroutine in a coroutine");
    }
    var co=ngc.alloc(vm_co);
    nas_co& cort=co.co();
    cort.pc=func.func().entry-1;

    cort.top[0]=nil;
    cort.localr=cort.top+1;
    cort.top=cort.localr+func.func().lsize;
    cort.localr[0]=func.func().local[0];
    cort.top[0]=nil; // old upvalr
    cort.top++;
    cort.top[0]=var::addr((var*)nullptr); // old localr
    cort.top++;
    cort.top[0]=var::ret(0); // old pc, set to zero to make op_ret recognizing this as coroutine function

    cort.funcr=func; // make sure the coroutine function can use correct upvalues
    cort.status=nas_co::suspended;
    
    return co;
}

var builtin_coresume(var* local,gc& ngc) {
    if (ngc.cort) {
        return nas_err("coroutine::resume","cannot start another coroutine when one is running");
    }
    var co=local[1];
    // return nil if is not a coroutine object
    if (co.type!=vm_co) {
        return nil;
    }
    // cannot resume a dead coroutine
    if (co.co().status==nas_co::dead) {
        return nil;
    }

    // change to coroutine context
    ngc.ctxchg(co.co());

    // fetch coroutine's stack top and return
    // so the coroutine's stack top in fact is not changed
    if (ngc.top[0].type==vm_ret) {
        // when first calling this coroutine, the stack top must be vm_ret
        return ngc.top[0];
    }

    // after first calling the coroutine, each time coroutine.yield triggered
    // a new space will be reserved on stack with value nil
    // so we could fill this place with args

    // the coroutine seems like coroutine.yield returns the value
    // but in fact coroutine.yield stop the coroutine
    // until main context calls the coroutine.resume
    return local[2];
}

var builtin_coyield(var* local,gc& ngc) {
    if (!ngc.cort) {
        return nas_err("coroutine::yield","no coroutine is running");
    }

    // this will set to main stack top
    ngc.ctxreserve();
    
    // then this will return value to main's stack top[0]
    // the procedure seems like coroutine.resume returns the value
    // but in fact coroutine.resume stop the main context
    // until coroutine calls the coroutine.yield
    return local[1];
}

var builtin_costatus(var* local,gc& ngc) {
    var co=local[1];
    if (co.type!=vm_co) {
        return ngc.newstr("error");
    }
    switch(co.co().status) {
        case nas_co::suspended: return ngc.newstr("suspended");break;
        case nas_co::running:   return ngc.newstr("running");  break;
        case nas_co::dead:      return ngc.newstr("dead");     break;
    }
    return nil;
}

var builtin_corun(var* local,gc& ngc) {
    return ngc.cort?one:zero;
}

var builtin_millisec(var* local,gc& ngc) {
    f64 res=std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return var::num(res);
}

var builtin_sysargv(var* local,gc& ngc) {
    var res=ngc.alloc(vm_vec);
    res.vec().elems=ngc.env_argv;
    return res;
}

var builtin_gcextend(var* local,gc& ngc) {
    var type=local[1];
    if (type.type!=vm_str) {
        return nil;
    }
    auto& s=type.str();
    if (s=="str") {
        ngc.extend(vm_str);
    } else if (s=="vec") {
        ngc.extend(vm_vec);
    } else if (s=="hash") {
        ngc.extend(vm_hash);
    } else if (s=="func") {
        ngc.extend(vm_func);
    } else if (s=="upval") {
        ngc.extend(vm_upval);
    } else if (s=="obj") {
        ngc.extend(vm_obj);
    } else if (s=="co") {
        ngc.extend(vm_co);
    }
    return nil;
}

var builtin_logtime(var* local,gc& ngc) {
    time_t t=time(nullptr);
    tm* tm_t=localtime(&t);
    char s[128];
    sprintf(
        s,"%d-%.2d-%.2d %.2d:%.2d:%.2d",
        tm_t->tm_year+1900,
        tm_t->tm_mon+1,
        tm_t->tm_mday,
        tm_t->tm_hour,
        tm_t->tm_min,
        tm_t->tm_sec
    );
    return ngc.newstr(s);
}

// register builtin function's name and it's address here in this table below
// this table must end with {nullptr,nullptr}
struct {
    const char* name;
    var (*func)(var*,gc&);
} builtin[]= {
    {"__print",   builtin_print   },
    {"__println", builtin_println },
    {"__exit",    builtin_exit    },
    {"__abort",   builtin_abort   },
    {"__append",  builtin_append  },
    {"__setsize", builtin_setsize },
    {"__system",  builtin_system  },
    {"__input",   builtin_input   },
    {"__fin",     builtin_fin     },
    {"__fout",    builtin_fout    },
    {"__split",   builtin_split   },
    {"__rand",    builtin_rand    },
    {"__id",      builtin_id      },
    {"__int",     builtin_int     },
    {"__floor",   builtin_floor   },
    {"__num",     builtin_num     },
    {"__pop",     builtin_pop     },
    {"__str",     builtin_str     },
    {"__size",    builtin_size    },
    {"__i32xor",  builtin_i32xor  },
    {"__i32and",  builtin_i32and  },
    {"__i32or",   builtin_i32or   },
    {"__i32nand", builtin_i32nand },
    {"__i32not",  builtin_i32not  },
    {"__u32xor",  builtin_u32xor  },
    {"__u32and",  builtin_u32and  },
    {"__u32or",   builtin_u32or   },
    {"__u32nand", builtin_u32nand },
    {"__u32not",  builtin_u32not  },
    {"__pow",     builtin_pow     },
    {"__sin",     builtin_sin     },
    {"__cos",     builtin_cos     },
    {"__tan",     builtin_tan     },
    {"__exp",     builtin_exp     },
    {"__lg",      builtin_lg      },
    {"__ln",      builtin_ln      },
    {"__sqrt",    builtin_sqrt    },
    {"__atan2",   builtin_atan2   },
    {"__isnan",   builtin_isnan   },
    {"__time",    builtin_time    },
    {"__contains",builtin_contains},
    {"__delete",  builtin_delete  },
    {"__keys",    builtin_keys    },
    {"__die",     builtin_die     },
    {"__find",    builtin_find    },
    {"__type",    builtin_type    },
    {"__substr",  builtin_substr  },
    {"__streq",   builtin_streq   },
    {"__left",    builtin_left    },
    {"__right",   builtin_right   },
    {"__cmp",     builtin_cmp     },
    {"__chr",     builtin_chr     },
    {"__char",    builtin_char    },
    {"__values",  builtin_values  },
    {"__exists",  builtin_exists  },
    {"__open",    builtin_open    },
    {"__close",   builtin_close   },
    {"__read",    builtin_read    },
    {"__write",   builtin_write   },
    {"__seek",    builtin_seek    },
    {"__tell",    builtin_tell    },
    {"__readln",  builtin_readln  },
    {"__stat",    builtin_stat    },
    {"__eof",     builtin_eof     },
    {"__fld",     builtin_fld     },
    {"__sfld",    builtin_sfld    },
    {"__setfld",  builtin_setfld  },
    {"__buf",     builtin_buf     },
    {"__sleep",   builtin_sleep   },
    {"__pipe",    builtin_pipe    },
    {"__fork",    builtin_fork    },
    {"__waitpid", builtin_waitpid },
    {"__opendir", builtin_opendir },
    {"__readdir", builtin_readdir },
    {"__closedir",builtin_closedir},
    {"__chdir",   builtin_chdir   },
    {"__environ", builtin_environ },
    {"__getcwd",  builtin_getcwd  },
    {"__getenv",  builtin_getenv  },
    {"__dlopen",  builtin_dlopen  },
    {"__dlclose", builtin_dlclose },
    {"__dlcallv", builtin_dlcallv },
    {"__dlcall",  builtin_dlcall  },
    {"__platform",builtin_platform},
    {"__md5",     builtin_md5     },
    {"__cocreate",builtin_cocreate},
    {"__coresume",builtin_coresume},
    {"__coyield", builtin_coyield },
    {"__costatus",builtin_costatus},
    {"__corun"   ,builtin_corun   },
    {"__millisec",builtin_millisec},
    {"__sysargv", builtin_sysargv },
    {"__gcextd",  builtin_gcextend},
    {"__logtime", builtin_logtime },
    {nullptr,     nullptr         }
};
