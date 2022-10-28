#pragma once

#include <vector>
#include <queue>
#include <unordered_map>

#include "nasal_err.h"

enum vm_type:u8{
    /* none-gc object */
    vm_none=0,
    vm_cnt,
    vm_addr,
    vm_ret,
    vm_nil,
    vm_num,
    /* gc object */
    vm_str,
    vm_vec,
    vm_hash,
    vm_func,
    vm_upval,
    vm_obj,
    vm_co
};
const u32 gc_tsize=vm_co-vm_str+1;
// change parameters here to make your own efficient gc
// better set bigger number on vm_vec
const u32 ini[gc_tsize]=
{
    128, // vm_str
    128, // vm_vec
    32,  // vm_hash
    128, // vm_func
    0,   // vm_upval
    0,   // vm_obj
    0    // vm_co
};
const u32 incr[gc_tsize]=
{
    1024,// vm_str
    512, // vm_vec
    512, // vm_hash
    512, // vm_func
    512, // vm_upval
    128, // vm_obj
    32   // vm_co
};

struct nas_vec;  // vector
struct nas_hash; // hashmap(dict)
struct nas_func; // function(lambda)
struct nas_upval;// upvalue
struct nas_obj;  // special objects
struct nas_co;   // coroutine
struct nas_val;  // nas_val includes gc-managed types

struct var
{
    u8 type;
    union
    {
        u32 ret;
        i64 cnt;
        f64 num;
        var* addr;
        nas_val* gcobj;
    } val;

    // vm_none/vm_nil
    var(const u8 t=vm_none):type(t){}
    // vm_ret
    var(const u8 t,const u32 n):type(t){val.ret=n;}
    // vm_cnt
    var(const u8 t,const i64 n):type(t){val.cnt=n;}
    // vm_num
    var(const u8 t,const f64 n):type(t){val.num=n;}
    // nas_val
    var(const u8 t,nas_val* n):type(t){val.gcobj=n;}
    // vm_addr
    var(const u8 t,var* n):type(t){val.addr=n;}
    // copy
    var(const var& nr):type(nr.type),val(nr.val){}
    bool operator==(const var& nr){return type==nr.type && val.gcobj==nr.val.gcobj;}
    bool operator!=(const var& nr){return type!=nr.type || val.gcobj!=nr.val.gcobj;}
    // number and string can be translated to each other
    f64    tonum();
    string tostr();
    friend std::ostream& operator<<(std::ostream&,var&);
    bool   objchk(u32);
    inline var*       addr();
    inline u32        ret ();
    inline i64&       cnt ();
    inline f64        num ();
    inline string&    str ();
    inline nas_vec&   vec ();
    inline nas_hash&  hash();
    inline nas_func&  func();
    inline nas_upval& upval();
    inline nas_obj&   obj ();
    inline nas_co&    co  ();
};

struct nas_vec
{
    bool printed;
    std::vector<var> elems;

    nas_vec():printed(false){}
    friend std::ostream& operator<<(std::ostream&,nas_vec&);
    usize  size(){return elems.size();}
    var    get_val(const i32);
    var*   get_mem(const i32);
};

struct nas_hash
{
    bool printed;
    std::unordered_map<string,var> elems;

    nas_hash():printed(false){}
    friend std::ostream& operator<<(std::ostream&,nas_hash&);
    usize  size(){return elems.size();}
    var    get_val(const string&);
    var*   get_mem(const string&);
};

struct nas_func
{
    i32 dpara; // dynamic parameter name index in hash.
    u32 entry; // pc will set to entry-1 to call this function
    u32 psize; // used to load default parameters to a new function
    u32 lsize; // used to expand memory space for local values on stack
    std::vector<var> local; // local scope with default value(var)
    std::vector<var> upval; // closure
    std::unordered_map<u32,u32> keys; // parameter table, u32 begins from 1

    nas_func():dpara(-1),entry(0),psize(0),lsize(0){}
    void clear();
};

struct nas_upval
{
    bool onstk;
    u32  size;
    var* stk;
    std::vector<var> elems;

    nas_upval(){onstk=true;stk=nullptr;size=0;}
    var& operator[](usize n){return onstk?stk[n]:elems[n];}
    void clear(){onstk=true;elems.clear();size=0;}
};

struct nas_obj
{
    enum obj_t:u32
    {
        null,
        file,
        dir,
        dylib,
        faddr
    };
    /* RAII constructor */
    /* new object is initialized when creating */
    u32 type;
    void* ptr;
    /* RAII destroyer */
    /* default destroyer does nothing */
    typedef void (*dest)(void*);
    dest dtor;

    nas_obj():type(obj_t::null),ptr(nullptr),dtor(nullptr){}
    ~nas_obj(){clear();}
    void set(u32 t=obj_t::null,void* p=nullptr,dest d=nullptr)
    {
        type=t;
        ptr=p;
        dtor=d;
    }
    void clear()
    {
        if(dtor && ptr)
            dtor(ptr);
        ptr=nullptr;
        dtor=nullptr;
    }
};

struct nas_co
{
    enum costat:u32
    {
        suspended,
        running,
        dead
    };
    var  stack[STACK_DEPTH];
    
    u32  pc;
    var* top;
    var* canary;
    var* localr;
    var* memr;
    var  funcr;
    var  upvalr;

    u32 status;
    nas_co():
        pc(0),top(stack),
        canary(stack+STACK_DEPTH-1),
        localr(nullptr),
        memr(nullptr),
        funcr({vm_nil,(f64)0}),
        upvalr({vm_nil,(f64)0}),
        status(nas_co::suspended)
    {
        for(u32 i=0;i<STACK_DEPTH;++i)
            stack[i]={vm_nil,(f64)0};
    }
    void clear()
    {
        for(u32 i=0;i<STACK_DEPTH;++i)
            stack[i]={vm_nil,(f64)0};
        pc=0;
        localr=nullptr;
        memr=nullptr;
        top=stack;
        status=nas_co::suspended;
        funcr={vm_nil,(f64)0};
    }
};

const u8 GC_UNCOLLECTED=0;   
const u8 GC_COLLECTED  =1;
const u8 GC_FOUND      =2;
struct nas_val
{
    u8 mark;
    u8 type;
    u8 unmut; // used to mark if a string is unmutable
    union 
    {
        string*    str;
        nas_vec*   vec;
        nas_hash*  hash;
        nas_func*  func;
        nas_upval* upval;
        nas_obj*   obj;
        nas_co*    co;
    } ptr;

    nas_val(u8);
    ~nas_val();
    void clear();
};

var nas_vec::get_val(const i32 n)
{
    i32 size=elems.size();
    if(n<-size || n>=size)
        return {vm_none};
    return elems[n>=0?n:n+size];
}
var* nas_vec::get_mem(const i32 n)
{
    i32 size=elems.size();
    if(n<-size || n>=size)
        return nullptr;
    return &elems[n>=0?n:n+size];
}
std::ostream& operator<<(std::ostream& out,nas_vec& vec)
{
    if(!vec.elems.size() || vec.printed)
    {
        out<<(vec.elems.size()?"[..]":"[]");
        return out;
    }
    vec.printed=true;
    usize iter=0,size=vec.elems.size();
    out<<'[';
    for(auto& i:vec.elems)
        out<<i<<",]"[(++iter)==size];
    vec.printed=false;
    return out;
}

var nas_hash::get_val(const string& key)
{
    if(elems.count(key))
        return elems[key];
    else if(elems.count("parents"))
    {
        var ret(vm_none);
        var val=elems["parents"];
        if(val.type==vm_vec)
            for(auto& i:val.vec().elems)
            {
                if(i.type==vm_hash)
                    ret=i.hash().get_val(key);
                if(ret.type!=vm_none)
                    return ret;
            }
    }
    return {vm_none};
}
var* nas_hash::get_mem(const string& key)
{
    if(elems.count(key))
        return &elems[key];
    else if(elems.count("parents"))
    {
        var* addr=nullptr;
        var val=elems["parents"];
        if(val.type==vm_vec)
            for(auto& i:val.vec().elems)
            {
                if(i.type==vm_hash)
                    addr=i.hash().get_mem(key);
                if(addr)
                    return addr;
            }
    }
    return nullptr;
}
std::ostream& operator<<(std::ostream& out,nas_hash& hash)
{
    if(!hash.elems.size() || hash.printed)
    {
        out<<(hash.elems.size()?"{..}":"{}");
        return out;
    }
    hash.printed=true;
    usize iter=0,size=hash.elems.size();
    out<<'{';
    for(auto& i:hash.elems)
        out<<i.first<<':'<<i.second<<",}"[(++iter)==size];
    hash.printed=false;
    return out;
}

void nas_func::clear()
{
    dpara=-1;
    local.clear();
    upval.clear();
    keys.clear();
}

nas_val::nas_val(u8 val_type)
{
    mark=GC_COLLECTED;
    type=val_type;
    unmut=0;
    switch(val_type)
    {
        case vm_str:  ptr.str=new string;     break;
        case vm_vec:  ptr.vec=new nas_vec;    break;
        case vm_hash: ptr.hash=new nas_hash;  break;
        case vm_func: ptr.func=new nas_func;  break;
        case vm_upval:ptr.upval=new nas_upval;break;
        case vm_obj:  ptr.obj=new nas_obj;    break;
        case vm_co:   ptr.co=new nas_co;      break;
    }
}
nas_val::~nas_val()
{
    switch(type)
    {
        case vm_str:  delete ptr.str;  break;
        case vm_vec:  delete ptr.vec;  break;
        case vm_hash: delete ptr.hash; break;
        case vm_func: delete ptr.func; break;
        case vm_upval:delete ptr.upval;break;
        case vm_obj:  delete ptr.obj;  break;
        case vm_co:   delete ptr.co;   break;
    }
    type=vm_nil;
}
void nas_val::clear()
{
    switch(type)
    {
        case vm_str:  ptr.str->clear();       break;
        case vm_vec:  ptr.vec->elems.clear(); break;
        case vm_hash: ptr.hash->elems.clear();break;
        case vm_func: ptr.func->clear();      break;
        case vm_upval:ptr.upval->clear();     break;
        case vm_obj:  ptr.obj->clear();       break;
        case vm_co:   ptr.co->clear();        break;
    }
}
f64 var::tonum()
{
    return type!=vm_str?val.num:str2num(str().c_str());
}
string var::tostr()
{
    if(type==vm_str)
        return str();
    else if(type==vm_num)
    {
        string tmp=std::to_string(num());
        tmp.erase(tmp.find_last_not_of('0')+1,string::npos);
        tmp.erase(tmp.find_last_not_of('.')+1,string::npos);
        return tmp;
    }
    return "";
}
std::ostream& operator<<(std::ostream& out,var& ref)
{
    switch(ref.type)
    {
        case vm_none: out<<"undefined";   break;
        case vm_nil:  out<<"nil";         break;
        case vm_num:  out<<ref.val.num;   break;
        case vm_str:  out<<ref.str();     break;
        case vm_vec:  out<<ref.vec();     break;
        case vm_hash: out<<ref.hash();    break;
        case vm_func: out<<"func(..){..}";break;
        case vm_obj:  out<<"<object>";    break;
        case vm_co:   out<<"<coroutine>"; break;
    }
    return out;
}
bool var::objchk(u32 objtype)
{
    return type==vm_obj && obj().type==objtype && obj().ptr;
}
inline var*       var::addr (){return val.addr;             }
inline u32        var::ret  (){return val.ret;              }
inline i64&       var::cnt  (){return val.cnt;              }
inline f64        var::num  (){return val.num;              }
inline string&    var::str  (){return *val.gcobj->ptr.str;  }
inline nas_vec&   var::vec  (){return *val.gcobj->ptr.vec;  }
inline nas_hash&  var::hash (){return *val.gcobj->ptr.hash; }
inline nas_func&  var::func (){return *val.gcobj->ptr.func; }
inline nas_upval& var::upval(){return *val.gcobj->ptr.upval;}
inline nas_obj&   var::obj  (){return *val.gcobj->ptr.obj;  }
inline nas_co&    var::co   (){return *val.gcobj->ptr.co;   }

const var zero={vm_num,(f64)0};
const var one ={vm_num,(f64)1};
const var nil ={vm_nil,(f64)0};

struct gc
{
    /* main context */
    struct
    {
        u32  pc;
        var* top;
        var* localr;
        var* memr;
        var  funcr;
        var  upvalr;
        var* canary;
        var* stack;
    } mctx;
    
    /* runtime context */
    u32&  pc;     // program counter
    var*& localr; // local scope register
    var*& memr;   // used for mem_call
    var&  funcr;  // function register
    var&  upvalr; // upvalue register
    var*& canary; // avoid stackoverflow
    var*& top;    // stack top
    var*  stack;  // stack pointer
    nas_co* cort; // running coroutine

    /* native function used */
    var temp; // temporary place used in builtin/module functions

    /* constants and memory pool */
    std::vector<var> strs;        // reserved address for const vm_str
    std::vector<var> env_argv;    // command line arguments
    std::vector<nas_val*> memory; // gc memory
    std::queue<nas_val*> unused[gc_tsize]; // gc free list

    /* values for analysis */
    u64 size[gc_tsize];
    u64 count[gc_tsize];
    u64 acnt[gc_tsize];
    gc(u32& _pc, var*& _localr, var*& _memr, var& _funcr,
       var& _upvalr, var*& _canary, var*& _top, var* _stk):
        pc(_pc),localr(_localr),memr(_memr),funcr(_funcr),upvalr(_upvalr),
        canary(_canary),top(_top),stack(_stk),cort(nullptr),temp(nil){}
    void mark();
    void sweep();
    void init(const std::vector<string>&,const std::vector<string>&);
    void clear();
    void info();
    var  alloc(const u8);
    var  newstr(char);
    var  newstr(const char*);
    var  newstr(const string&);
    void ctxchg(nas_co&);
    void ctxreserve();
};

/* gc functions */
void gc::mark()
{
    std::queue<var> bfs;
    // scan coroutine process stack when coroutine ptr is not null
    // scan main process stack when coroutine ptr is null
    // this scan process must execute because when running coroutine,
    // the nas_co related to it will not update it's context(like `top`) until the coroutine suspends or exits.
    for(var* i=stack;i<=top;++i)
        bfs.push(*i);
    bfs.push(funcr);
    bfs.push(upvalr);
    bfs.push(temp);
    if(cort) // scan main process stack
    {
        for(var* i=mctx.stack;i<=mctx.top;++i)
            bfs.push(*i);
        bfs.push(mctx.funcr);
        bfs.push(mctx.upvalr);
    }
    
    while(!bfs.empty())
    {
        var tmp=bfs.front();
        bfs.pop();
        if(tmp.type<=vm_num || tmp.val.gcobj->mark) continue;
        tmp.val.gcobj->mark=GC_FOUND;
        switch(tmp.type)
        {
            case vm_vec:
                for(auto& i:tmp.vec().elems)
                    bfs.push(i);
                break;
            case vm_hash:
                for(auto& i:tmp.hash().elems)
                    bfs.push(i.second);
                break;
            case vm_func:
                for(auto& i:tmp.func().local)
                    bfs.push(i);
                for(auto& i:tmp.func().upval)
                    bfs.push(i);
                break;
            case vm_upval:
                for(auto& i:tmp.upval().elems)
                    bfs.push(i);
                break;
            case vm_co:
                bfs.push(tmp.co().funcr);
                bfs.push(tmp.co().upvalr);
                for(var* i=tmp.co().stack;i<=tmp.co().top;++i)
                    bfs.push(*i);
                break;
        }
    }
}
void gc::sweep()
{
    for(auto i:memory)
    {
        if(i->mark==GC_UNCOLLECTED)
        {
            i->clear();
            unused[i->type-vm_str].push(i);
            i->mark=GC_COLLECTED;
        }
        else if(i->mark==GC_FOUND)
            i->mark=GC_UNCOLLECTED;
    }
}
void gc::init(const std::vector<string>& s,const std::vector<string>& argv)
{
    // initiaize function register
    funcr=nil;

    for(u8 i=0;i<gc_tsize;++i)
        size[i]=count[i]=acnt[i]=0;
    for(u8 i=0;i<gc_tsize;++i)
        for(u32 j=0;j<ini[i];++j)
        {
            nas_val* tmp=new nas_val(i+vm_str);
            memory.push_back(tmp);
            unused[i].push(tmp);
        }
    cort=nullptr;
    // init constant strings
    strs.resize(s.size());
    for(u32 i=0;i<strs.size();++i)
    {
        strs[i]={vm_str,new nas_val(vm_str)};
        strs[i].val.gcobj->unmut=1;
        strs[i].str()=s[i];
    }
    // record arguments
    env_argv.resize(argv.size());
    for(usize i=0;i<argv.size();++i)
    {
        env_argv[i]={vm_str,new nas_val(vm_str)};
        env_argv[i].val.gcobj->unmut=1;
        env_argv[i].str()=argv[i];
    }
}
void gc::clear()
{
    for(auto i:memory)
        delete i;
    memory.clear();
    for(u8 i=0;i<gc_tsize;++i)
        while(!unused[i].empty())
            unused[i].pop();
    for(auto& i:strs)
        delete i.val.gcobj;
    strs.clear();
    env_argv.clear();
}
void gc::info()
{
    const char* name[]={"str  ","vec  ","hash ","func ","upval","obj  ","co   "};
    std::cout<<"\ngarbage collector info(gc/alloc)\n";
    for(u8 i=0;i<gc_tsize;++i)
        if(count[i] || acnt[i])
            std::cout<<" "<<name[i]<<" | "<<count[i]<<","<<acnt[i]<<"\n";
    std::cout<<"\nmemory allocator info(max size)\n";
    for(u8 i=0;i<gc_tsize;++i)
        if(ini[i] || size[i])
            std::cout<<" "<<name[i]<<" | "<<ini[i]+size[i]*incr[i]<<" (+"<<size[i]<<")\n";
}
var gc::alloc(u8 type)
{
    const u8 index=type-vm_str;
    ++acnt[index];
    if(unused[index].empty())
    {
        ++count[index];
        mark();
        sweep();
    }
    if(unused[index].empty())
    {
        ++size[index];
        for(u32 i=0;i<incr[index];++i)
        {
            nas_val* tmp=new nas_val(type);
            memory.push_back(tmp);
            unused[index].push(tmp);
        }
    }
    var ret={type,unused[index].front()};
    ret.val.gcobj->mark=GC_UNCOLLECTED;
    unused[index].pop();
    return ret;
}
var gc::newstr(char c)
{
    var s=alloc(vm_str);
    s.str()=c;
    return s;
}
var gc::newstr(const char* buff)
{
    var s=alloc(vm_str);
    s.str()=buff;
    return s;
}
var gc::newstr(const string& buff)
{
    var s=alloc(vm_str);
    s.str()=buff;
    return s;
}
void gc::ctxchg(nas_co& ctx)
{
    mctx.pc=pc;
    mctx.top=top;
    mctx.localr=localr;
    mctx.memr=memr;
    mctx.funcr=funcr;
    mctx.upvalr=upvalr;
    mctx.canary=canary;
    mctx.stack=stack;

    pc=ctx.pc;
    top=ctx.top;
    localr=ctx.localr;
    memr=ctx.memr;
    funcr=ctx.funcr;
    upvalr=ctx.upvalr;
    canary=ctx.canary;
    stack=ctx.stack;
    cort=&ctx;

    cort->status=nas_co::running;
}
void gc::ctxreserve()
{
    // pc=0 means this coroutine is finished
    cort->status=pc?nas_co::suspended:nas_co::dead;
    cort->pc=pc;
    cort->localr=localr;
    cort->memr=memr;
    cort->funcr=funcr;
    cort->upvalr=upvalr;
    cort->canary=canary;
    cort->top=top;

    pc=mctx.pc;
    localr=mctx.localr;
    memr=mctx.memr;
    funcr=mctx.funcr;
    upvalr=mctx.upvalr;
    canary=mctx.canary;
    top=mctx.top;
    stack=mctx.stack;
    cort=nullptr;
}

// use to print error log and return error value
var nas_err(const string& err_f,const string& info)
{
    std::cerr<<"[vm] "<<err_f<<": "<<info<<"\n";
    return {vm_none};
}

typedef var (*mod)(var*,usize,gc*); // module function type
typedef mod (*getptr)(const char*); // module function "get" type
