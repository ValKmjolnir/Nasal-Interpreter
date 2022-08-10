#ifndef __NASAL_BUILTIN_H__
#define __NASAL_BUILTIN_H__

#if defined __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

nas_ref nas_err(const string& err_f,const string& info)
{
    std::cerr<<"[vm] "<<err_f<<": "<<info<<"\n";
    return {vm_none};
}

void print_core(std::vector<nas_ref>& elems)
{
    for(auto& i:elems)
        switch(i.type)
        {
            case vm_none:std::cout<<"null";        break;
            case vm_nil: std::cout<<"nil";         break;
            case vm_num: std::cout<<i.num();       break;
            case vm_str: std::cout<<i.str();       break;
            case vm_vec: i.vec().print();          break;
            case vm_hash:i.hash().print();         break;
            case vm_func:std::cout<<"func(..){..}";break;
            case vm_obj: std::cout<<"<object>";    break;
            case vm_co:  std::cout<<"<coroutine>"; break;
        }
}
nas_ref builtin_print(nas_ref* local,nasal_gc& gc)
{
    print_core(local[1].vec().elems);
    std::cout<<std::flush;
    return nil;
}
nas_ref builtin_println(nas_ref* local,nasal_gc& gc)
{
    print_core(local[1].vec().elems);
    std::cout<<std::endl;
    return nil;
}
nas_ref builtin_abort(nas_ref* local,nasal_gc& gc)
{
    std::abort();
    return nil;
}
nas_ref builtin_append(nas_ref* local,nasal_gc& gc)
{
    nas_ref vec=local[1];
    nas_ref elem=local[2];
    if(vec.type!=vm_vec)
        return nas_err("append","\"vec\" must be vector");
    auto& v=vec.vec().elems;
    for(auto& i:elem.vec().elems)
        v.push_back(i);
    return nil;
}
nas_ref builtin_setsize(nas_ref* local,nasal_gc& gc)
{
    nas_ref vec=local[1];
    nas_ref size=local[2];
    if(vec.type!=vm_vec)
        return nas_err("setsize","\"vec\" must be vector");
    if(size.type!=vm_num || size.num()<0)
        return nil;
    vec.vec().elems.resize((i64)size.num(),nil);
    return nil;
}
nas_ref builtin_system(nas_ref* local,nasal_gc& gc)
{
    nas_ref str=local[1];
    if(str.type!=vm_str)
        return nas_err("system","\"str\" must be string");
    return {vm_num,(f64)system(str.str().c_str())};
}
nas_ref builtin_input(nas_ref* local,nasal_gc& gc)
{
    nas_ref end=local[1];
    nas_ref ret=gc.alloc(vm_str);
    if(end.type!=vm_str || end.str().length()>1 || !end.str().length())
        std::cin>>ret.str();
    else
        std::getline(std::cin,ret.str(),end.str()[0]);
    return ret;
}
nas_ref builtin_fin(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    if(val.type!=vm_str)
        return nas_err("io.fin","\"filename\" must be string");
    std::ifstream fin(val.str(),std::ios::binary);
    std::stringstream rd;
    if(!fin.fail())
        rd<<fin.rdbuf();
    return gc.newstr(rd.str());
}
nas_ref builtin_fout(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    nas_ref str=local[2];
    if(val.type!=vm_str)
        return nas_err("io.fout","\"filename\" must be string");
    if(str.type!=vm_str)
        return nas_err("io.fout","\"str\" must be string");
    std::ofstream fout(val.str());
    if(fout.fail())
        return nas_err("io.fout","cannot open <"+val.str()+">");
    fout<<str.str();
    return nil;
}
nas_ref builtin_split(nas_ref* local,nasal_gc& gc)
{
    nas_ref delimeter=local[1];
    nas_ref str=local[2];
    if(delimeter.type!=vm_str)
        return nas_err("split","\"separator\" must be string");
    if(str.type!=vm_str)
        return nas_err("split","\"str\" must be string");
    const string& deli=delimeter.str();
    const string& s=str.str();

    // avoid being sweeped
    nas_ref res=gc.temp=gc.alloc(vm_vec);
    std::vector<nas_ref>& vec=res.vec().elems;

    if(!deli.length())
    {
        for(auto i:s)
            vec.push_back(gc.newstr(i));
        gc.temp=nil;
        return res;
    }
    usize last=0,pos=s.find(deli,0);
    while(pos!=string::npos)
    {
        if(pos>last)
            vec.push_back(gc.newstr(s.substr(last,pos-last)));
        last=pos+deli.length();
        pos=s.find(deli,last);
    }
    if(last!=s.length())
        vec.push_back(gc.newstr(s.substr(last)));
    gc.temp=nil;
    return res;
}
nas_ref builtin_rand(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    if(val.type!=vm_num && val.type!=vm_nil)
        return nas_err("rand","\"seed\" must be nil or number");
    if(val.type==vm_num)
    {
        srand((u32)val.num());
        return nil;
    }
    f64 num=0;
    for(u32 i=0;i<5;++i)
        num=(num+rand())*(1.0/(RAND_MAX+1.0));
    return {vm_num,num};
}
nas_ref builtin_id(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    std::stringstream ss;
    ss<<"0";
    if(val.type>vm_num)
        ss<<"x"<<std::hex<<(u64)val.val.gcobj<<std::dec;
    return gc.newstr(ss.str());
}
nas_ref builtin_int(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    if(val.type!=vm_num)
        return nil;
    return {vm_num,f64((i32)val.num())};
}
nas_ref builtin_floor(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,std::floor(val.num())};
}
nas_ref builtin_num(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    if(val.type==vm_num)
        return val;
    if(val.type!=vm_str)
        return nil;
    f64 res=val.tonum();
    if(std::isnan(res))
        return nil;
    return {vm_num,res};
}
nas_ref builtin_pop(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    if(val.type!=vm_vec)
        return nas_err("pop","\"vec\" must be vector");
    auto& vec=val.vec().elems;
    if(vec.size())
    {
        nas_ref tmp=vec.back();
        vec.pop_back();
        return tmp;
    }
    return nil;
}
nas_ref builtin_str(nas_ref* local,nasal_gc& gc)
{
    return gc.newstr(local[1].tostr());
}
nas_ref builtin_size(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    f64 num=0;
    switch(val.type)
    {
        case vm_num:  num=val.num();         break;
        case vm_str:  num=val.str().length();break;
        case vm_vec:  num=val.vec().size();  break;
        case vm_hash: num=val.hash().size(); break;
    }
    return {vm_num,num};
}
nas_ref builtin_i32xor(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(i32(local[1].num())^i32(local[2].num()))};
}
nas_ref builtin_i32and(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(i32(local[1].num())&i32(local[2].num()))};
}
nas_ref builtin_i32or(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(i32(local[1].num())|i32(local[2].num()))};
}
nas_ref builtin_i32nand(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(~(i32(local[1].num())&i32(local[2].num())))};
}
nas_ref builtin_i32not(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(~i32(local[1].num()))};
}
nas_ref builtin_u32xor(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(u32(local[1].num())^u32(local[2].num()))};
}
nas_ref builtin_u32and(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(u32(local[1].num())&u32(local[2].num()))};
}
nas_ref builtin_u32or(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(u32(local[1].num())|u32(local[2].num()))};
}
nas_ref builtin_u32nand(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(~(u32(local[1].num())&u32(local[2].num())))};
}
nas_ref builtin_u32not(nas_ref* local,nasal_gc& gc)
{
    return {vm_num,(f64)(~u32(local[1].num()))};
}
nas_ref builtin_pow(nas_ref* local,nasal_gc& gc)
{
    nas_ref x=local[1];
    nas_ref y=local[2];
    if(x.type!=vm_num || y.type!=vm_num)
        return {vm_num,std::nan("")};
    return {vm_num,std::pow(x.num(),y.num())};
}
nas_ref builtin_sin(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,val.type==vm_num?sin(val.num()):std::nan("")};
}
nas_ref builtin_cos(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,val.type==vm_num?cos(val.num()):std::nan("")};
}
nas_ref builtin_tan(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,val.type==vm_num?tan(val.num()):std::nan("")};
}
nas_ref builtin_exp(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,val.type==vm_num?exp(val.num()):std::nan("")};
}
nas_ref builtin_lg(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,val.type==vm_num?log(val.num())/log(10.0):std::nan("")};
}
nas_ref builtin_ln(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,val.type==vm_num?log(val.num()):std::nan("")};
}
nas_ref builtin_sqrt(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    return {vm_num,val.type==vm_num?sqrt(val.num()):std::nan("")};
}
nas_ref builtin_atan2(nas_ref* local,nasal_gc& gc)
{
    nas_ref x=local[1];
    nas_ref y=local[2];
    if(x.type!=vm_num || y.type!=vm_num)
        return {vm_num,std::nan("")};
    return {vm_num,atan2(y.num(),x.num())};
}
nas_ref builtin_isnan(nas_ref* local,nasal_gc& gc)
{
    nas_ref x=local[1];
    return (x.type==vm_num && std::isnan(x.num()))?one:zero;
}
nas_ref builtin_time(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    if(val.type!=vm_num)
        return nas_err("time","\"begin_time\" must be number");
    time_t begin_time=(time_t)val.num();
    return {vm_num,(f64)time(&begin_time)};
}
nas_ref builtin_contains(nas_ref* local,nasal_gc& gc)
{
    nas_ref hash=local[1];
    nas_ref key=local[2];
    if(hash.type!=vm_hash)
        return nas_err("contains","\"hash\" must be hash");
    if(key.type!=vm_str)
        return zero;
    return hash.hash().elems.count(key.str())?one:zero;
}
nas_ref builtin_delete(nas_ref* local,nasal_gc& gc)
{
    nas_ref hash=local[1];
    nas_ref key=local[2];
    if(hash.type!=vm_hash)
        return nas_err("delete","\"hash\" must be hash");
    if(key.type!=vm_str)
        return nil;
    if(hash.hash().elems.count(key.str()))
        hash.hash().elems.erase(key.str());
    return nil;
}
nas_ref builtin_keys(nas_ref* local,nasal_gc& gc)
{
    nas_ref hash=local[1];
    if(hash.type!=vm_hash)
        return nas_err("keys","\"hash\" must be hash");
    // avoid being sweeped
    nas_ref res=gc.temp=gc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    for(auto& iter:hash.hash().elems)
        vec.push_back(gc.newstr(iter.first));
    gc.temp=nil;
    return res;
}
nas_ref builtin_die(nas_ref* local,nasal_gc& gc)
{
    nas_ref str=local[1];
    if(str.type!=vm_str)
        return nas_err("die","\"str\" must be string");
    std::cerr<<"[vm] error: "<<str.str()<<'\n';
    return {vm_none};
}
nas_ref builtin_find(nas_ref* local,nasal_gc& gc)
{
    nas_ref needle=local[1];
    nas_ref haystack=local[2];
    if(needle.type!=vm_str)
        return nas_err("find","\"needle\" must be string");
    if(haystack.type!=vm_str)
        return nas_err("find","\"haystack\" must be string");
    usize pos=haystack.str().find(needle.str());
    if(pos==string::npos)
        return {vm_num,(f64)-1};
    return {vm_num,(f64)pos};
}
nas_ref builtin_type(nas_ref* local,nasal_gc& gc)
{
    switch(local[1].type)
    {
        case vm_none: return gc.newstr("undefined");break;
        case vm_nil:  return gc.newstr("nil");      break;
        case vm_num:  return gc.newstr("num");      break;
        case vm_str:  return gc.newstr("str");      break;
        case vm_vec:  return gc.newstr("vec");      break;
        case vm_hash: return gc.newstr("hash");     break;
        case vm_func: return gc.newstr("func");     break;
        case vm_obj:  return gc.newstr("obj");      break;
        case vm_co:   return gc.newstr("coroutine");break;
    }
    return nil;
}
nas_ref builtin_substr(nas_ref* local,nasal_gc& gc)
{
    nas_ref str=local[1];
    nas_ref beg=local[2];
    nas_ref len=local[3];
    if(str.type!=vm_str)
        return nas_err("substr","\"str\" must be string");
    if(beg.type!=vm_num || beg.num()<0)
        return nas_err("substr","\"begin\" should be number >= 0");
    if(len.type!=vm_num || len.num()<0)
        return nas_err("substr","\"length\" should be number >= 0");
    usize begin=(usize)beg.num();
    usize length=(usize)len.num();
    if(begin>=str.str().length() || begin+length>str.str().length())
        return nas_err("susbtr","index out of range");
    return gc.newstr(str.str().substr(begin,length));
}
nas_ref builtin_streq(nas_ref* local,nasal_gc& gc)
{
    nas_ref a=local[1];
    nas_ref b=local[2];
    return {vm_num,f64((a.type!=vm_str || b.type!=vm_str)?0:(a.str()==b.str()))};
}
nas_ref builtin_left(nas_ref* local,nasal_gc& gc)
{
    nas_ref str=local[1];
    nas_ref len=local[2];
    if(str.type!=vm_str)
        return nas_err("left","\"string\" must be string");
    if(len.type!=vm_num)
        return nas_err("left","\"length\" must be number");
    if(len.num()<0)
        return gc.newstr("");
    return gc.newstr(str.str().substr(0,len.num()));
}
nas_ref builtin_right(nas_ref* local,nasal_gc& gc)
{
    nas_ref str=local[1];
    nas_ref len=local[2];
    if(str.type!=vm_str)
        return nas_err("right","\"string\" must be string");
    if(len.type!=vm_num)
        return nas_err("right","\"length\" must be number");
    i32 length=(i32)len.num();
    i32 srclen=str.str().length();
    if(length>srclen)
        length=srclen;
    if(length<0)
        length=0;
    return gc.newstr(str.str().substr(srclen-length,srclen));
}
nas_ref builtin_cmp(nas_ref* local,nasal_gc& gc)
{
    nas_ref a=local[1];
    nas_ref b=local[2];
    if(a.type!=vm_str)
        return nas_err("cmp","\"a\" must be string");
    if(b.type!=vm_str)
        return nas_err("cmp","\"b\" must be string");
    return {vm_num,(f64)strcmp(a.str().c_str(),b.str().c_str())};
}
nas_ref builtin_chr(nas_ref* local,nasal_gc& gc)
{
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
    if(0<=num && num<128)
        return gc.newstr((char)num);
    else if(128<=num && num<256)
        return gc.newstr(extend[num-128]);
    return gc.newstr(" ");
}
nas_ref builtin_values(nas_ref* local,nasal_gc& gc)
{
    nas_ref hash=local[1];
    if(hash.type!=vm_hash)
        return nas_err("values","\"hash\" must be hash");
    nas_ref vec=gc.alloc(vm_vec);
    auto& v=vec.vec().elems;
    for(auto& i:hash.hash().elems)
        v.push_back(i.second);
    return vec;
}
nas_ref builtin_exists(nas_ref* local,nasal_gc& gc)
{
    if(local[1].type!=vm_str)
        return zero;
    return access(local[1].str().c_str(),F_OK)!=-1?one:zero;
}
void obj_file_dtor(void* ptr)
{
    fclose((FILE*)ptr);
}
nas_ref builtin_open(nas_ref* local,nasal_gc& gc)
{
    nas_ref name=local[1];
    nas_ref mode=local[2];
    if(name.type!=vm_str)
        return nas_err("open","\"filename\" must be string");
    if(mode.type!=vm_str)
        return nas_err("open","\"mode\" must be string");
    FILE* res=fopen(name.str().c_str(),mode.str().c_str());
    if(!res)
        return nas_err("open","failed to open file <"+name.str()+">");
    nas_ref ret=gc.alloc(vm_obj);
    ret.obj().set(nas_obj::file,res,obj_file_dtor);
    return ret;
}
nas_ref builtin_close(nas_ref* local,nasal_gc& gc)
{
    nas_ref fd=local[1];
    if(!fd.objchk(nas_obj::file))
        return nas_err("close","not a valid filehandle");
    fclose((FILE*)fd.obj().ptr);
    fd.obj().ptr=nullptr;
    return nil;
}
nas_ref builtin_read(nas_ref* local,nasal_gc& gc)
{
    nas_ref fd=local[1];
    nas_ref buf=local[2];
    nas_ref len=local[3];
    if(!fd.objchk(nas_obj::file))
        return nas_err("read","not a valid filehandle");
    if(buf.type!=vm_str || buf.val.gcobj->unmut)
        return nas_err("read","\"buf\" must be mutable string");
    if(len.type!=vm_num)
        return nas_err("read","\"len\" must be number");
    if(len.num()<=0 || len.num()>=(1<<30))
        return nas_err("read","\"len\" less than 1 or too large");
    char* buff=new char[(usize)len.num()+1];
    if(!buff)
        return nas_err("read","malloc failed");
    f64 res=fread(buff,1,len.num(),(FILE*)fd.obj().ptr);
    buf.str()=buff;
    buf.val.gcobj->unmut=true;
    delete []buff;
    return {vm_num,res};
}
nas_ref builtin_write(nas_ref* local,nasal_gc& gc)
{
    nas_ref fd=local[1];
    nas_ref str=local[2];
    if(!fd.objchk(nas_obj::file))
        return nas_err("write","not a valid filehandle");
    if(str.type!=vm_str)
        return nas_err("write","\"str\" must be string");
    return {vm_num,(f64)fwrite(str.str().c_str(),1,str.str().length(),(FILE*)fd.obj().ptr)};
}
nas_ref builtin_seek(nas_ref* local,nasal_gc& gc)
{
    nas_ref fd=local[1];
    nas_ref pos=local[2];
    nas_ref whence=local[3];
    if(!fd.objchk(nas_obj::file))
        return nas_err("seek","not a valid filehandle");
    return {vm_num,(f64)fseek((FILE*)fd.obj().ptr,pos.num(),whence.num())};
}
nas_ref builtin_tell(nas_ref* local,nasal_gc& gc)
{
    nas_ref fd=local[1];
    if(!fd.objchk(nas_obj::file))
        return nas_err("tell","not a valid filehandle");
    return {vm_num,(f64)ftell((FILE*)fd.obj().ptr)};
}
nas_ref builtin_readln(nas_ref* local,nasal_gc& gc)
{
    nas_ref fd=local[1];
    if(!fd.objchk(nas_obj::file))
        return nas_err("readln","not a valid filehandle");
    nas_ref str=gc.alloc(vm_str);
    char c;
    while((c=fgetc((FILE*)fd.obj().ptr))!=EOF)
    {
        if(c=='\r')
            continue;
        if(c=='\n')
            return str;
        str.str()+=c;
    }
    if(str.str().length())
        return str;
    return nil;
}
nas_ref builtin_stat(nas_ref* local,nasal_gc& gc)
{
    nas_ref name=local[1];
    if(name.type!=vm_str)
        return nas_err("stat","\"filename\" must be string");
    struct stat buf;
    if(stat(name.str().c_str(),&buf)<0)
        return nas_err("stat","failed to open file <"+name.str()+">");
    nas_ref ret=gc.alloc(vm_vec);
    ret.vec().elems={
        {vm_num,(f64)buf.st_dev},
        {vm_num,(f64)buf.st_ino},
        {vm_num,(f64)buf.st_mode},
        {vm_num,(f64)buf.st_nlink},
        {vm_num,(f64)buf.st_uid},
        {vm_num,(f64)buf.st_gid},
        {vm_num,(f64)buf.st_rdev},
        {vm_num,(f64)buf.st_size},
        {vm_num,(f64)buf.st_atime},
        {vm_num,(f64)buf.st_mtime},
        {vm_num,(f64)buf.st_ctime}
    };
    return ret;
}
nas_ref builtin_eof(nas_ref* local,nasal_gc& gc)
{
    nas_ref fd=local[1];
    if(!fd.objchk(nas_obj::file))
        return nas_err("readln","not a valid filehandle");
    return {vm_num,(f64)feof((FILE*)fd.obj().ptr)};
}
nas_ref builtin_fld(nas_ref* local,nasal_gc& gc)
{
    // bits.fld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5)
    nas_ref str=local[1];
    nas_ref startbit=local[2];
    nas_ref length=local[3];
    if(str.type!=vm_str || str.val.gcobj->unmut)
        return nas_err("fld","\"str\" must be mutable string");
    if(startbit.type!=vm_num || length.type!=vm_num)
        return nas_err("fld","\"startbit\",\"len\" must be number");
    u32 bit=(u32)startbit.num();
    u32 len=(u32)length.num();
    if(bit+len>8*str.str().length())
        return nas_err("fld","bitfield out of bounds");
    u32 res=0;
    auto& s=str.str();
    for(u32 i=bit;i<bit+len;++i)
        if(s[i>>3]&(1<<(7-(i&7))))
            res|=1<<(bit+len-i-1);
    return {vm_num,(f64)res};
}
nas_ref builtin_sfld(nas_ref* local,nasal_gc& gc)
{
    // bits.sfld(s,0,3);
    // if s stores 10100010(162)
    // will get 101(5) then this will be signed extended to
    // 11111101(-3)
    nas_ref str=local[1];
    nas_ref startbit=local[2];
    nas_ref length=local[3];
    if(str.type!=vm_str || str.val.gcobj->unmut)
        return nas_err("sfld","\"str\" must be mutable string");
    if(startbit.type!=vm_num || length.type!=vm_num)
        return nas_err("sfld","\"startbit\",\"len\" must be number");
    u32 bit=(u32)startbit.num();
    u32 len=(u32)length.num();
    if(bit+len>8*str.str().length())
        return nas_err("sfld","bitfield out of bounds");
    u32 res=0;
    auto& s=str.str();
    for(u32 i=bit;i<bit+len;++i)
        if(s[i>>3]&(1<<(7-(i&7))))
            res|=1<<(bit+len-i-1);
    if(res&(1<<(len-1)))
        res|=~((1<<len)-1);
    return {vm_num,(f64)((i32)res)};
}
nas_ref builtin_setfld(nas_ref* local,nasal_gc& gc)
{
    // bits.setfld(s,0,8,69);
    // set 01000101(69) to string will get this:
    // 10100010(162)
    // so s[0]=162
    nas_ref str=local[1];
    nas_ref startbit=local[2];
    nas_ref length=local[3];
    nas_ref value=local[4];
    if(str.type!=vm_str || str.val.gcobj->unmut)
        return nas_err("setfld","\"str\" must be mutable string");
    if(startbit.type!=vm_num || length.type!=vm_num || value.type!=vm_num)
        return nas_err("setfld","\"startbit\",\"len\",\"val\" must be number");
    u32 bit=(u32)startbit.num();
    u32 len=(u32)length.num();
    u64 val=(u64)value.num();
    if(bit+len>8*str.str().length())
        return nas_err("setfld","bitfield out of bounds");
    auto& s=str.str();
    for(u32 i=bit;i<bit+len;++i)
    {
        if(val&(1<<(i-bit)))
            s[i>>3]|=(1<<(7-(i&7)));
        else
            s[i>>3]&=~(1<<(7-(i&7)));
    }
    return nil;
}
nas_ref builtin_buf(nas_ref* local,nasal_gc& gc)
{
    nas_ref length=local[1];
    if(length.type!=vm_num || length.num()<=0)
        return nas_err("buf","\"len\" must be number greater than 0");
    nas_ref str=gc.alloc(vm_str);
    auto& s=str.str();
    s.resize(length.num(),'\0');
    return str;
}
nas_ref builtin_sleep(nas_ref* local,nasal_gc& gc)
{
    nas_ref val=local[1];
    if(val.type!=vm_num)
        return nas_err("sleep","\"duration\" must be number");
    std::this_thread::sleep_for(std::chrono::microseconds(i64(val.num()*1e6)));
    return nil;
}
nas_ref builtin_pipe(nas_ref* local,nasal_gc& gc)
{
#ifndef _WIN32
    i32 fd[2];
    nas_ref res=gc.alloc(vm_vec);
    if(pipe(fd)==-1)
        return nas_err("pipe","failed to create pipe");
    res.vec().elems.push_back({vm_num,(f64)fd[0]});
    res.vec().elems.push_back({vm_num,(f64)fd[1]});
    return res;
#endif
    return nas_err("pipe","not supported for windows");
}
nas_ref builtin_fork(nas_ref* local,nasal_gc& gc)
{
#ifndef _WIN32
    f64 res=fork();
    if(res<0)
        return nas_err("fork","failed to fork a process");
    return {vm_num,(f64)res};
#endif
    return nas_err("fork","not supported for windows");
}
nas_ref builtin_waitpid(nas_ref* local,nasal_gc& gc)
{
    nas_ref pid=local[1];
    nas_ref nohang=local[2];
    if(pid.type!=vm_num || nohang.type!=vm_num)
        return nas_err("waitpid","pid and nohang must be number");
#ifndef _WIN32
    i32 ret_pid,status;
    ret_pid=waitpid(pid.num(),&status,nohang.num()==0?0:WNOHANG);
    nas_ref vec=gc.alloc(vm_vec);
    vec.vec().elems.push_back({vm_num,(f64)ret_pid});
    vec.vec().elems.push_back({vm_num,(f64)status});
    return vec;
#endif
    return nas_err("waitpid","not supported for windows");
}
void obj_dir_dtor(void* ptr)
{
#ifndef _MSC_VER
    closedir((DIR*)ptr);
#else
    FindClose(ptr);
#endif
}
nas_ref builtin_opendir(nas_ref* local,nasal_gc& gc)
{
    nas_ref path=local[1];
    if(path.type!=vm_str)
        return nas_err("opendir","\"path\" must be string");
#ifdef _MSC_VER
    WIN32_FIND_DATA data;
    HANDLE p;
    p=FindFirstFile((path.str()+"\\*.*").c_str(),&data);
    if(p==INVALID_HANDLE_VALUE)
        return nas_err("opendir","cannot open dir <"+path.str()+">");
#else
    DIR* p=opendir(path.str().c_str());
    if(!p)
        return nas_err("opendir","cannot open dir <"+path.str()+">");
#endif
    nas_ref ret=gc.alloc(vm_obj);
    ret.obj().set(nas_obj::dir,p,obj_dir_dtor);
    return ret;
}
nas_ref builtin_readdir(nas_ref* local,nasal_gc& gc)
{
    nas_ref handle=local[1];
    if(!handle.objchk(nas_obj::dir))
        return nas_err("readdir","not a valid dir handle");
#ifdef _MSC_VER
    WIN32_FIND_DATA data;
    if(!FindNextFile(handle.obj().ptr,&data))
        return nil;
    return gc.newstr(data.cFileName);
#else
    dirent* p=readdir((DIR*)handle.obj().ptr);
    return p?gc.newstr(p->d_name):nil;
#endif
}
nas_ref builtin_closedir(nas_ref* local,nasal_gc& gc)
{
    nas_ref handle=local[1];
    if(!handle.objchk(nas_obj::dir))
        return nas_err("closedir","not a valid dir handle");
#ifndef _MSC_VER
    closedir((DIR*)handle.obj().ptr);
#else
    FindClose(handle.obj().ptr);
#endif
    handle.obj().ptr=nullptr;
    return nil;
}
nas_ref builtin_chdir(nas_ref* local,nasal_gc& gc)
{
    nas_ref path=local[1];
    if(path.type!=vm_str)
        return {vm_num,(f64)-1};
    return {vm_num,(f64)chdir(path.str().c_str())};
}
nas_ref builtin_environ(nas_ref* local,nasal_gc& gc)
{
    nas_ref res=gc.temp=gc.alloc(vm_vec);
    auto& vec=res.vec().elems;
    for(char** env=environ;*env;++env)
        vec.push_back(gc.newstr(*env));
    gc.temp=nil;
    return res;
}
nas_ref builtin_getcwd(nas_ref* local,nasal_gc& gc)
{
    char buf[1024];
    if(!getcwd(buf,sizeof(buf)))
        return nil;
    return gc.newstr(buf);
}
nas_ref builtin_getenv(nas_ref* local,nasal_gc& gc)
{
    nas_ref envvar=local[1];
    if(envvar.type!=vm_str)
        return nas_err("getenv","\"envvar\" must be string");
    char* res=getenv(envvar.str().c_str());
    return res?gc.newstr(res):nil;
}
void obj_dylib_dtor(void* ptr)
{
#ifdef _WIN32
    FreeLibrary((HMODULE)ptr);
#else
    dlclose(ptr);
#endif
}
nas_ref builtin_dlopen(nas_ref* local,nasal_gc& gc)
{
    nas_ref dlname=local[1];
    if(dlname.type!=vm_str)
        return nas_err("dlopen","\"libname\" must be string");
#ifdef _WIN32
    wchar_t* str=new wchar_t[dlname.str().size()+1];
    if(!str)
        return nas_err("dlopen","malloc failed");
    memset(str,0,sizeof(wchar_t)*dlname.str().size()+1);
    mbstowcs(str,dlname.str().c_str(),dlname.str().size()+1);
    void* ptr=LoadLibraryW(str);
    delete []str;
#else
    void* ptr=dlopen(dlname.str().c_str(),RTLD_LOCAL|RTLD_LAZY);
#endif
    if(!ptr)
        return nas_err("dlopen","cannot open dynamic lib <"+dlname.str()+">");
    nas_ref ret=gc.alloc(vm_obj);
    ret.obj().set(nas_obj::dylib,ptr,obj_dylib_dtor);
    return ret;
}
nas_ref builtin_dlsym(nas_ref* local,nasal_gc& gc)
{
    nas_ref lib=local[1];
    nas_ref sym=local[2];
    if(!lib.objchk(nas_obj::dylib))
        return nas_err("dlsym","\"lib\" is not a valid dynamic lib");
    if(sym.type!=vm_str)
        return nas_err("dlsym","\"sym\" must be string");
#ifdef _WIN32
    void* func=(void*)GetProcAddress((HMODULE)lib.obj().ptr,sym.str().c_str());
#else
    void* func=dlsym(lib.obj().ptr,sym.str().c_str());
#endif
    if(!func)
        return nas_err("dlsym","cannot find symbol \""+sym.str()+"\"");
    nas_ref ret=gc.alloc(vm_obj);
    ret.obj().set(nas_obj::faddr,func);
    return ret;
}
nas_ref builtin_dlclose(nas_ref* local,nasal_gc& gc)
{
    nas_ref libptr=local[1];
    if(!libptr.objchk(nas_obj::dylib))
        return nas_err("dlclose","\"lib\" is not a valid dynamic lib");
#ifdef _WIN32
    FreeLibrary((HMODULE)libptr.obj().ptr);
#else
    dlclose(libptr.obj().ptr);
#endif
    libptr.obj().ptr=nullptr;
    return nil;
}
nas_ref builtin_dlcall(nas_ref* local,nasal_gc& gc)
{
    nas_ref fp=local[1];
    nas_ref args=local[2];
    if(!fp.objchk(nas_obj::faddr))
        return nas_err("dlcall","\"funcptr\" is not a valid function pointer");
    typedef nas_ref (*externs)(std::vector<nas_ref>&,nasal_gc&);
    externs func=(externs)fp.obj().ptr;
    return func(args.vec().elems,gc);
}

nas_ref builtin_platform(nas_ref* local,nasal_gc& gc)
{
#if defined _WIN32 || defined _WIN64
    return gc.newstr("windows");
#elif defined __linux__
    return gc.newstr("linux");
#elif defined __APPLE__
    return gc.newstr("macOS");
#endif
    return gc.newstr("unknown");
}

// md5 related functions
string tohex(u32 num)
{
    const char str16[]="0123456789abcdef";
    string str="";
    for(u32 i=0;i<4;i++,num>>=8)
    {
        string tmp="";
        for(u32 j=0,b=num&0xff;j<2;j++,b>>=4)
            tmp.insert(0,1,str16[b&0xf]);
        str+=tmp;
    }
    return str;
}
string md5(const string& src)
{
    std::vector<u32> buff;
    usize num=((src.length()+8)>>6)+1;
    usize buffsize=num<<4;
    buff.resize(buffsize,0);
    for(usize i=0;i<src.length();i++)
        buff[i>>2]|=((u8)src[i])<<((i&0x3)<<3);
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
    for(u32 i=0;i<buffsize;i+=16)
    {
        u32 f,a=atmp,b=btmp,c=ctmp,d=dtmp;
        for(u32 j=0;j<64;j++)
        {
            if(j<16)      f=md5f(b,c,d);
            else if(j<32) f=md5g(b,c,d);
            else if(j<48) f=md5h(b,c,d);
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
nas_ref builtin_md5(nas_ref* local,nasal_gc& gc)
{
    nas_ref str=local[1];
    if(str.type!=vm_str)
        return nas_err("md5","\"str\" must be string");
    return gc.newstr(md5(str.str()));
}

nas_ref builtin_cocreate(nas_ref* local,nasal_gc& gc)
{
    // +-----------------+
    // | old pc          | <- top[0]
    // +-----------------+
    // | old localr      | <- top[-1]
    // +-----------------+
    // | old upvalr      | <- top[-2]
    // +-----------------+
    // | local scope     |
    // | ...             |
    // +-----------------+ <- local pointer stored in localr
    // | old funcr       | <- old function stored in funcr
    // +-----------------+
    nas_ref func=local[1];
    if(func.type!=vm_func)
        return nas_err("coroutine::create","must use a function to create coroutine");
    if(gc.cort)
        return nas_err("coroutine::create","cannot create another coroutine in a coroutine");
    nas_ref co=gc.alloc(vm_co);
    nas_co& coroutine=co.co();
    coroutine.pc=func.func().entry-1;

    coroutine.top[0]=nil;
    coroutine.localr=coroutine.top+1;
    coroutine.top=coroutine.localr+func.func().lsize;
    coroutine.localr[0]=func.func().local[0];
    coroutine.top[0]=nil; // old upvalr
    coroutine.top++;
    coroutine.top[0]={vm_addr,(nas_ref*)nullptr}; // old localr
    coroutine.top++;
    coroutine.top[0]={vm_ret,(u32)0}; // old pc, set to zero to make op_ret recognizing this as coroutine function

    coroutine.funcr=func; // make sure the coroutine function can use correct upvalues
    coroutine.status=nas_co::suspended;
    
    return co;
}
nas_ref builtin_coresume(nas_ref* local,nasal_gc& gc)
{
    if(gc.cort)
        return nas_err("coroutine::resume","cannot start another coroutine when one is running");
    nas_ref co=local[1];
    if(co.type!=vm_co)
        return nas_err("coroutine::resume","must use a coroutine object");
    if(co.co().status==nas_co::dead)
        return nil;
    gc.ctxchg(co.co());
    return gc.top[0];
}
nas_ref builtin_coyield(nas_ref* local,nasal_gc& gc)
{
    if(!gc.cort)
        return nas_err("coroutine::yield","no coroutine is running");
    gc.ctxreserve();
    // this will set to main stack top
    // then builtin_coresume will return it
    return local[1];
}
nas_ref builtin_costatus(nas_ref* local,nasal_gc& gc)
{
    nas_ref co=local[1];
    if(co.type!=vm_co)
        return nas_err("coroutine::status","must use a coroutine object");
    switch(co.co().status)
    {
        case nas_co::suspended: return gc.newstr("suspended");break;
        case nas_co::running:   return gc.newstr("running");  break;
        case nas_co::dead:      return gc.newstr("dead");     break;
    }
    return nil;
}
nas_ref builtin_corun(nas_ref* local,nasal_gc& gc)
{
    return gc.cort?one:zero;
}
nas_ref builtin_millisec(nas_ref* local,nasal_gc& gc)
{
    f64 res=std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return {vm_num,res};
}
nas_ref builtin_sysargv(nas_ref* local,nasal_gc& gc)
{
    nas_ref res=gc.alloc(vm_vec);
    res.vec().elems=gc.env_argv;
    return res;
}
nas_ref builtin_logtime(nas_ref* local,nasal_gc& gc)
{
    time_t t=time(nullptr);
    tm* tm_t=localtime(&t);
    char s[32];
    sprintf(s,"%d-%.2d-%.2d %.2d:%.2d:%.2d",tm_t->tm_year+1900,tm_t->tm_mon+1,tm_t->tm_mday,tm_t->tm_hour,tm_t->tm_min,tm_t->tm_sec);
    return gc.newstr(s);
}

// register builtin function's name and it's address here in this table below
// this table must end with {nullptr,nullptr}
struct
{
    const char* name;
    nas_ref (*func)(nas_ref*,nasal_gc&);
} builtin[]=
{
    {"__print",   builtin_print   },
    {"__println", builtin_println },
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
    {"__dlsym",   builtin_dlsym   },
    {"__dlclose", builtin_dlclose },
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
    {"__logtime", builtin_logtime },
    {nullptr,     nullptr         }
};
#endif