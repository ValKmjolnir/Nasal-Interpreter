#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

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
    vm_co,
    vm_tsize
};
const u32 gc_tsize=vm_tsize-vm_str;
// change parameters here to make your own efficient gc
// better set bigger number on vm_vec
const u32 ini[gc_tsize]=
{
    128, // vm_str
    128, // vm_vec
    32,  // vm_hash
    512, // vm_func
    512, // vm_upval
    0,   // vm_obj
    0    // vm_co
};
const u32 incr[gc_tsize]=
{
    256, // vm_str
    512, // vm_vec
    512, // vm_hash
    512, // vm_func
    128, // vm_upval
    128, // vm_obj
    16   // vm_co
};

struct nas_vec;  // vector
struct nas_hash; // hashmap(dict)
struct nas_func; // function(lambda)
struct nas_upval;// upvalue
struct nas_obj;  // special objects
struct nas_co;   // coroutine
struct nas_val;  // nas_val includes gc-managed types

struct nas_ref
{
    u8 type;
    union
    {
        u32 ret;
        i64 cnt;
        f64 num;
        nas_ref* addr;
        nas_val* gcobj;
    } val;

    // vm_none/vm_nil
    nas_ref(const u8 t=vm_none):type(t){}
    // vm_ret
    nas_ref(const u8 t,const u32 n):type(t){val.ret=n;}
    // vm_cnt
    nas_ref(const u8 t,const i64 n):type(t){val.cnt=n;}
    // vm_num
    nas_ref(const u8 t,const f64 n):type(t){val.num=n;}
    // vm_str/vm_func/vm_vec/vm_hash/vm_upval/vm_obj
    nas_ref(const u8 t,nas_val* n):type(t){val.gcobj=n;}
    // vm_addr
    nas_ref(const u8 t,nas_ref* n):type(t){val.addr=n;}
    nas_ref(const nas_ref& nr):type(nr.type),val(nr.val){}
    bool operator==(const nas_ref& nr){return type==nr.type && val.gcobj==nr.val.gcobj;}
    bool operator!=(const nas_ref& nr){return type!=nr.type || val.gcobj!=nr.val.gcobj;}
    // number and string can be translated to each other
    f64    tonum();
    string tostr();
    void   print();
    bool   objchk(u32);
    inline nas_ref*   addr();
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
    std::vector<nas_ref> elems;

    nas_vec():printed(false){}
    void     print();
    usize    size(){return elems.size();}
    nas_ref  get_val(const i32);
    nas_ref* get_mem(const i32);
};

struct nas_hash
{
    bool printed;
    std::unordered_map<string,nas_ref> elems;

    nas_hash():printed(false){}
    void     print();
    usize    size(){return elems.size();}
    nas_ref  get_val(const string&);
    nas_ref* get_mem(const string&);
};

struct nas_func
{
    i32 dpara; // dynamic parameter name index in hash.
    u32 entry; // pc will set to entry-1 to call this function
    u32 psize; // used to load default parameters to a new function
    u32 lsize; // used to expand memory space for local values on stack
    std::vector<nas_ref> local;   // local scope with default value(nas_ref)
    std::vector<nas_ref> upval; // closure
    std::unordered_map<u32,u32> keys; // parameter table, u32 begins from 1

    nas_func():dpara(-1),entry(0),psize(0),lsize(0){}
    void clear();
};

struct nas_upval
{
    bool onstk;
    u32 size;
    nas_ref* stk;
    std::vector<nas_ref> elems;

    nas_upval(){onstk=true;stk=nullptr;size=0;}
    nas_ref& operator[](usize n){return onstk?stk[n]:elems[n];}
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
    nas_ref  stack[STACK_DEPTH];
    
    u32      pc;
    nas_ref* top;
    nas_ref* canary;
    nas_ref* localr;
    nas_ref* memr;
    nas_ref  funcr;
    nas_ref  upvalr;

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
    }ptr;

    nas_val(u8);
    ~nas_val();
};

nas_ref nas_vec::get_val(const i32 n)
{
    i32 size=elems.size();
    if(n<-size || n>=size)
        return {vm_none};
    return elems[n>=0?n:n+size];
}
nas_ref* nas_vec::get_mem(const i32 n)
{
    i32 size=elems.size();
    if(n<-size || n>=size)
        return nullptr;
    return &elems[n>=0?n:n+size];
}
void nas_vec::print()
{
    if(!elems.size() || printed)
    {
        std::cout<<(elems.size()?"[..]":"[]");
        return;
    }
    printed=true;
    usize iter=0;
    usize size=elems.size();
    std::cout<<'[';
    for(auto& i:elems)
    {
        i.print();
        std::cout<<",]"[(++iter)==size];
    }
    printed=false;
}

nas_ref nas_hash::get_val(const string& key)
{
    if(elems.count(key))
        return elems[key];
    else if(elems.count("parents"))
    {
        nas_ref ret(vm_none);
        nas_ref val=elems["parents"];
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
nas_ref* nas_hash::get_mem(const string& key)
{
    if(elems.count(key))
        return &elems[key];
    else if(elems.count("parents"))
    {
        nas_ref* addr=nullptr;
        nas_ref val=elems["parents"];
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
void nas_hash::print()
{
    if(!elems.size() || printed)
    {
        std::cout<<(elems.size()?"{..}":"{}");
        return;
    }
    printed=true;
    usize iter=0;
    usize size=elems.size();
    std::cout<<'{';
    for(auto& i:elems)
    {
        std::cout<<i.first<<':';
        i.second.print();
        std::cout<<",}"[(++iter)==size];
    }
    printed=false;
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
f64 nas_ref::tonum()
{
    return type!=vm_str?val.num:str2num(str().c_str());
}
string nas_ref::tostr()
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
void nas_ref::print()
{
    switch(type)
    {
        case vm_none: std::cout<<"undefined";   break;
        case vm_nil:  std::cout<<"nil";         break;
        case vm_num:  std::cout<<val.num;       break;
        case vm_str:  std::cout<<str();         break;
        case vm_vec:  vec().print();            break;
        case vm_hash: hash().print();           break;
        case vm_func: std::cout<<"func(..){..}";break;
        case vm_obj:  std::cout<<"<object>";    break;
        case vm_co:   std::cout<<"<coroutine>"; break;
    }
}
bool nas_ref::objchk(u32 objtype)
{
    return type==vm_obj && obj().type==objtype && obj().ptr;
}
inline nas_ref*   nas_ref::addr (){return val.addr;             }
inline u32        nas_ref::ret  (){return val.ret;              }
inline i64&       nas_ref::cnt  (){return val.cnt;              }
inline f64        nas_ref::num  (){return val.num;              }
inline string&    nas_ref::str  (){return *val.gcobj->ptr.str;  }
inline nas_vec&   nas_ref::vec  (){return *val.gcobj->ptr.vec;  }
inline nas_hash&  nas_ref::hash (){return *val.gcobj->ptr.hash; }
inline nas_func&  nas_ref::func (){return *val.gcobj->ptr.func; }
inline nas_upval& nas_ref::upval(){return *val.gcobj->ptr.upval;}
inline nas_obj&   nas_ref::obj  (){return *val.gcobj->ptr.obj;  }
inline nas_co&    nas_ref::co   (){return *val.gcobj->ptr.co;   }

const nas_ref zero={vm_num,(f64)0};
const nas_ref one ={vm_num,(f64)1};
const nas_ref nil ={vm_nil,(f64)0};

struct nasal_gc
{
    /* main context */
    struct
    {
        u32      pc;
        nas_ref* top;
        nas_ref* localr;
        nas_ref* memr;
        nas_ref  funcr;
        nas_ref  upvalr;
        nas_ref* canary;
        nas_ref* stack;
    } mctx;
    
    /* runtime context */
    u32&      pc;    // program counter
    nas_ref*& localr;// local scope register
    nas_ref*& memr;  // used for mem_call
    nas_ref&  funcr; // function register
    nas_ref&  upvalr;// upvalue register
    nas_ref*& canary;// avoid stackoverflow
    nas_ref*& top;   // stack top
    nas_ref*  stack; // stack pointer
    nas_co*   cort;  // running coroutine
    nas_ref   temp;  // temporary place used in builtin/module functions

    /* constants and memory pool */
    std::vector<nas_ref>  strs;             // reserved address for const vm_str
    std::vector<nas_ref>  env_argv;         // command line arguments
    std::vector<nas_val*> memory;           // gc memory
    std::queue<nas_val*>  unused[gc_tsize]; // gc free list

    /* values for analysis */
    u64 size[gc_tsize];
    u64 count[gc_tsize];
    u64 allocc[gc_tsize];
    nasal_gc(
        u32& _pc,
        nas_ref*& _localr,
        nas_ref*& _memr,
        nas_ref& _funcr,
        nas_ref& _upvalr,
        nas_ref*& _canary,
        nas_ref*& _top,
        nas_ref* _stk):
        pc(_pc),localr(_localr),memr(_memr),funcr(_funcr),upvalr(_upvalr),
        canary(_canary),top(_top),stack(_stk),cort(nullptr),temp(nil){}
    void    mark();
    void    sweep();
    void    init(const std::vector<string>&,const std::vector<string>&);
    void    clear();
    void    info();
    nas_ref alloc(const u8);
    nas_ref newstr(char);
    nas_ref newstr(const char*);
    nas_ref newstr(const string&);
    void    ctxchg(nas_co&);
    void    ctxreserve();
};

/* gc functions */
void nasal_gc::mark()
{
    std::queue<nas_ref> bfs;
    // scan coroutine process stack when coroutine ptr is not null
    // scan main process stack when coroutine ptr is null
    // this scan process must execute because when running coroutine,
    // the nas_co related to it will not update it's context(like `top`) until the coroutine suspends or exits.
    for(nas_ref* i=stack;i<=top;++i)
        bfs.push(*i);
    bfs.push(funcr);
    bfs.push(upvalr);
    bfs.push(temp);
    if(cort) // scan main process stack
    {
        for(nas_ref* i=mctx.stack;i<=mctx.top;++i)
            bfs.push(*i);
        bfs.push(mctx.funcr);
        bfs.push(mctx.upvalr);
    }
    
    while(!bfs.empty())
    {
        nas_ref tmp=bfs.front();
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
                for(nas_ref* i=tmp.co().stack;i<=tmp.co().top;++i)
                    bfs.push(*i);
                break;
        }
    }
}
void nasal_gc::sweep()
{
    for(auto i:memory)
    {
        if(i->mark==GC_UNCOLLECTED)
        {
            switch(i->type)
            {
                case vm_str:  i->ptr.str->clear();       break;
                case vm_vec:  i->ptr.vec->elems.clear(); break;
                case vm_hash: i->ptr.hash->elems.clear();break;
                case vm_func: i->ptr.func->clear();      break;
                case vm_upval:i->ptr.upval->clear();     break;
                case vm_obj:  i->ptr.obj->clear();       break;
                case vm_co:   i->ptr.co->clear();        break;
            }
            unused[i->type-vm_str].push(i);
            i->mark=GC_COLLECTED;
        }
        else if(i->mark==GC_FOUND)
            i->mark=GC_UNCOLLECTED;
    }
}
void nasal_gc::init(const std::vector<string>& s,const std::vector<string>& argv)
{
    // initiaize function register
    funcr=nil;

    for(u8 i=0;i<gc_tsize;++i)
        size[i]=count[i]=allocc[i]=0;
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
void nasal_gc::clear()
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
void nasal_gc::info()
{
    const char* name[]={"str  ","vec  ","hash ","func ","upval","obj  ","co   "};
    std::cout<<"\ngarbage collector info(gc/alloc)\n";
    for(u8 i=0;i<gc_tsize;++i)
        if(count[i] || allocc[i])
            std::cout<<" "<<name[i]<<" | "<<count[i]<<","<<allocc[i]<<"\n";
    std::cout<<"\nmemory allocator info(max size)\n";
    for(u8 i=0;i<gc_tsize;++i)
        std::cout<<" "<<name[i]<<" | "<<ini[i]+size[i]*incr[i]<<" (+"<<size[i]<<")\n";
}
nas_ref nasal_gc::alloc(u8 type)
{
    const u8 index=type-vm_str;
    ++allocc[index];
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
    nas_ref ret={type,unused[index].front()};
    ret.val.gcobj->mark=GC_UNCOLLECTED;
    unused[index].pop();
    return ret;
}
nas_ref nasal_gc::newstr(char c)
{
    nas_ref s=alloc(vm_str);
    s.str()=c;
    return s;
}
nas_ref nasal_gc::newstr(const char* buff)
{
    nas_ref s=alloc(vm_str);
    s.str()=buff;
    return s;
}
nas_ref nasal_gc::newstr(const string& buff)
{
    nas_ref s=alloc(vm_str);
    s.str()=buff;
    return s;
}
void nasal_gc::ctxchg(nas_co& ctx)
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
void nasal_gc::ctxreserve()
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
nas_ref nas_err(const string& err_f,const string& info)
{
    std::cerr<<"[vm] "<<err_f<<": "<<info<<"\n";
    return {vm_none};
}
#endif