#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum vm_type:std::uint8_t{
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
const uint32_t gc_tsize=vm_tsize-vm_str;
// change parameters here to make your own efficient gc
// better set bigger number on vm_vec
const uint32_t ini[gc_tsize]=
{
    128, // vm_str
    128, // vm_vec
    32,  // vm_hash
    512, // vm_func
    512, // vm_upval
    0,   // vm_obj
    0    // vm_co
};
const uint32_t incr[gc_tsize]=
{
    256, // vm_str
    512, // vm_vec
    512, // vm_hash
    512, // vm_func
    128, // vm_upval
    128, // vm_obj
    16   // vm_co
};

struct nasal_vec;  // vector
struct nasal_hash; // hashmap(dict)
struct nasal_func; // function(lambda)
struct nasal_upval;// upvalue
struct nasal_obj;  // special objects
struct nasal_co;   // coroutine
struct nasal_val;  // nasal_val includes gc-managed types

struct nasal_ref
{
    uint8_t type;
    union
    {
        uint32_t ret;
        int64_t  cnt;
        double   num;
        nasal_ref* addr;
        nasal_val* gcobj;
    } val;

    // vm_none/vm_nil
    nasal_ref(const uint8_t t=vm_none):type(t){}
    // vm_ret
    nasal_ref(const uint8_t t,const uint32_t n):type(t){val.ret=n;}
    // vm_cnt
    nasal_ref(const uint8_t t,const int64_t n):type(t){val.cnt=n;}
    // vm_num
    nasal_ref(const uint8_t t,const double n):type(t){val.num=n;}
    // vm_str/vm_func/vm_vec/vm_hash/vm_upval/vm_obj
    nasal_ref(const uint8_t t,nasal_val* n):type(t){val.gcobj=n;}
    // vm_addr
    nasal_ref(const uint8_t t,nasal_ref* n):type(t){val.addr=n;}
    nasal_ref(const nasal_ref& nr):type(nr.type),val(nr.val){}
    bool operator==(const nasal_ref& nr){return type==nr.type && val.gcobj==nr.val.gcobj;}
    bool operator!=(const nasal_ref& nr){return type!=nr.type || val.gcobj!=nr.val.gcobj;}
    // number and string can be translated to each other
    double      tonum();
    std::string tostr();
    void        print();
    bool        objchk(uint32_t);
    inline nasal_ref*   addr();
    inline uint32_t     ret ();
    inline int64_t&     cnt ();
    inline double       num ();
    inline std::string& str ();
    inline nasal_vec&   vec ();
    inline nasal_hash&  hash();
    inline nasal_func&  func();
    inline nasal_upval& upval();
    inline nasal_obj&   obj ();
    inline nasal_co&    co  ();
};

struct nasal_vec
{
    bool printed;
    std::vector<nasal_ref> elems;

    nasal_vec():printed(false){}
    void       print();
    size_t     size(){return elems.size();}
    nasal_ref  get_val(const int);
    nasal_ref* get_mem(const int);
};

struct nasal_hash
{
    bool printed;
    std::unordered_map<std::string,nasal_ref> elems;

    nasal_hash():printed(false){}
    void       print();
    size_t     size(){return elems.size();}
    nasal_ref  get_val(const std::string&);
    nasal_ref* get_mem(const std::string&);
};

struct nasal_func
{
    int32_t  dynpara;                           // dynamic parameter name index in hash.
    uint32_t entry;                             // pc will set to entry-1 to call this function
    uint32_t psize;                             // used to load default parameters to a new function
    uint32_t lsize;                             // used to expand memory space for local values on stack
    std::vector<nasal_ref> local;               // local scope with default value(nasal_ref)
    std::vector<nasal_ref> upvalue;             // closure
    std::unordered_map<uint32_t,uint32_t> keys; // parameter name table, size_t begins from 1

    nasal_func():dynpara(-1),entry(0),psize(0),lsize(0){}
    void clear();
};

struct nasal_upval
{
    bool onstk;
    uint32_t size;
    nasal_ref* stk;
    std::vector<nasal_ref> elems;

    nasal_upval(){onstk=true;stk=nullptr;size=0;}
    nasal_ref& operator[](const int i){return onstk?stk[i]:elems[i];}
    void clear(){onstk=true;elems.clear();size=0;}
};

struct nasal_obj
{
    enum obj_t:std::uint32_t
    {
        null,
        file,
        dir,
        dylib,
        faddr
    };
    /* RAII constructor */
    /* new object is initialized when creating */
    uint32_t type;
    void* ptr;
    /* RAII destroyer */
    /* default destroyer does nothing */
    typedef void (*dest)(void*);
    dest dtor;

    nasal_obj():type(obj_t::null),ptr(nullptr),dtor(nullptr){}
    ~nasal_obj(){clear();}
    void set(uint32_t t=obj_t::null,void* p=nullptr,dest d=nullptr)
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

struct nasal_co
{
    enum coroutine_stat
    {
        suspended,
        running,
        dead
    };
    nasal_ref  stack[STACK_DEPTH];
    
    uint32_t   pc;
    nasal_ref* top;
    nasal_ref* canary;
    nasal_ref* localr;
    nasal_ref* memr;
    nasal_ref  funcr;
    nasal_ref  upvalr;

    uint32_t   status;
    nasal_co():
        pc(0),
        top(stack),
        canary(stack+STACK_DEPTH-1),
        localr(nullptr),
        memr(nullptr),
        funcr({vm_nil,(double)0}),
        upvalr({vm_nil,(double)0}),
        status(nasal_co::suspended)
    {
        for(uint32_t i=0;i<STACK_DEPTH;++i)
            stack[i]={vm_nil,(double)0};
    }
    void clear()
    {
        for(uint32_t i=0;i<STACK_DEPTH;++i)
            stack[i]={vm_nil,(double)0};
        pc=0;
        localr=nullptr;
        memr=nullptr;
        top=stack;
        status=nasal_co::suspended;
        funcr={vm_nil,(double)0};
    }
};

const uint8_t GC_UNCOLLECTED=0;   
const uint8_t GC_COLLECTED  =1;
const uint8_t GC_FOUND      =2;
struct nasal_val
{
    uint8_t mark;
    uint8_t type;
    uint8_t unmut; // used to mark if a string is unmutable
    union 
    {
        std::string* str;
        nasal_vec*   vec;
        nasal_hash*  hash;
        nasal_func*  func;
        nasal_upval* upval;
        nasal_obj*   obj;
        nasal_co*    co;
    }ptr;

    nasal_val(uint8_t);
    ~nasal_val();
};

nasal_ref nasal_vec::get_val(const int index)
{
    int size=elems.size();
    if(index<-size || index>=size)
        return {vm_none};
    return elems[index>=0?index:index+size];
}
nasal_ref* nasal_vec::get_mem(const int index)
{
    int size=elems.size();
    if(index<-size || index>=size)
        return nullptr;
    return &elems[index>=0?index:index+size];
}
void nasal_vec::print()
{
    if(!elems.size() || printed)
    {
        std::cout<<(elems.size()?"[..]":"[]");
        return;
    }
    printed=true;
    size_t iter=0;
    size_t size=elems.size();
    std::cout<<'[';
    for(auto& i:elems)
    {
        i.print();
        std::cout<<",]"[(++iter)==size];
    }
    printed=false;
}

nasal_ref nasal_hash::get_val(const std::string& key)
{
    if(elems.count(key))
        return elems[key];
    else if(elems.count("parents"))
    {
        nasal_ref ret(vm_none);
        nasal_ref val=elems["parents"];
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
nasal_ref* nasal_hash::get_mem(const std::string& key)
{
    if(elems.count(key))
        return &elems[key];
    else if(elems.count("parents"))
    {
        nasal_ref* addr=nullptr;
        nasal_ref val=elems["parents"];
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
void nasal_hash::print()
{
    if(!elems.size() || printed)
    {
        std::cout<<(elems.size()?"{..}":"{}");
        return;
    }
    printed=true;
    size_t iter=0;
    size_t size=elems.size();
    std::cout<<'{';
    for(auto& i:elems)
    {
        std::cout<<i.first<<':';
        i.second.print();
        std::cout<<",}"[(++iter)==size];
    }
    printed=false;
}

void nasal_func::clear()
{
    dynpara=-1;
    local.clear();
    upvalue.clear();
    keys.clear();
}

nasal_val::nasal_val(uint8_t val_type)
{
    mark=GC_COLLECTED;
    type=val_type;
    unmut=0;
    switch(val_type)
    {
        case vm_str:  ptr.str=new std::string;  break;
        case vm_vec:  ptr.vec=new nasal_vec;    break;
        case vm_hash: ptr.hash=new nasal_hash;  break;
        case vm_func: ptr.func=new nasal_func;  break;
        case vm_upval:ptr.upval=new nasal_upval;break;
        case vm_obj:  ptr.obj=new nasal_obj;    break;
        case vm_co:   ptr.co=new nasal_co;      break;
    }
}
nasal_val::~nasal_val()
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
double nasal_ref::tonum()
{
    return type!=vm_str?val.num:str2num(str().c_str());
}
std::string nasal_ref::tostr()
{
    if(type==vm_str)
        return str();
    else if(type==vm_num)
    {
        std::string tmp=std::to_string(num());
        tmp.erase(tmp.find_last_not_of('0')+1,std::string::npos);
        tmp.erase(tmp.find_last_not_of('.')+1,std::string::npos);
        return tmp;
    }
    return "";
}
void nasal_ref::print()
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
bool nasal_ref::objchk(uint32_t objtype)
{
    return type==vm_obj && obj().type==objtype && obj().ptr;
}
inline nasal_ref*   nasal_ref::addr (){return val.addr;             }
inline uint32_t     nasal_ref::ret  (){return val.ret;              }
inline int64_t&     nasal_ref::cnt  (){return val.cnt;              }
inline double       nasal_ref::num  (){return val.num;              }
inline std::string& nasal_ref::str  (){return *val.gcobj->ptr.str;  }
inline nasal_vec&   nasal_ref::vec  (){return *val.gcobj->ptr.vec;  }
inline nasal_hash&  nasal_ref::hash (){return *val.gcobj->ptr.hash; }
inline nasal_func&  nasal_ref::func (){return *val.gcobj->ptr.func; }
inline nasal_upval& nasal_ref::upval(){return *val.gcobj->ptr.upval;}
inline nasal_obj&   nasal_ref::obj  (){return *val.gcobj->ptr.obj;  }
inline nasal_co&    nasal_ref::co   (){return *val.gcobj->ptr.co;   }

const nasal_ref zero={vm_num,(double)0};
const nasal_ref one ={vm_num,(double)1};
const nasal_ref nil ={vm_nil,(double)0};

struct nasal_gc
{
    struct
    {
        uint32_t   pc;
        nasal_ref* top;
        nasal_ref* localr;
        nasal_ref* memr;
        nasal_ref  funcr;
        nasal_ref  upvalr;
        nasal_ref* canary;
        nasal_ref* stack;
    } main_ctx;
    
    /* runtime context */
    uint32_t&   pc;        // program counter
    nasal_ref*& localr;    // local scope register
    nasal_ref*& memr;      // used for mem_call
    nasal_ref&  funcr;     // function register
    nasal_ref&  upvalr;    // upvalue register
    nasal_ref*& canary;    // avoid stackoverflow
    nasal_ref*& top;       // stack top
    nasal_ref*  stack;     // stack pointer
    nasal_co*   coroutine; // running coroutine
    nasal_ref   temp;      // temporary place used in builtin/module functions

    /* constants and memory pool */
    std::vector<nasal_ref>  strs;             // reserved address for const vm_str
    std::vector<nasal_ref>  env_argv;         // command line arguments
    std::vector<nasal_val*> memory;           // gc memory
    std::queue<nasal_val*>  unused[gc_tsize]; // gc free list

    /* values for analysis */
    uint64_t size[gc_tsize];
    uint64_t count[gc_tsize];
    uint64_t allocc[gc_tsize];
    nasal_gc(
        uint32_t& _pc,
        nasal_ref*& _localr,
        nasal_ref*& _memr,
        nasal_ref& _funcr,
        nasal_ref& _upvalr,
        nasal_ref*& _canary,
        nasal_ref*& _top,
        nasal_ref* _stk):
        pc(_pc),localr(_localr),memr(_memr),funcr(_funcr),upvalr(_upvalr),
        canary(_canary),top(_top),stack(_stk),coroutine(nullptr),temp(nil){}
    void                    mark();
    void                    sweep();
    void                    init(const std::vector<std::string>&,const std::vector<std::string>&);
    void                    clear();
    void                    info();
    nasal_ref               alloc(const uint8_t);
    nasal_ref               newstr(char);
    nasal_ref               newstr(const char*);
    nasal_ref               newstr(const std::string&);
    void                    ctxchg(nasal_co&);
    void                    ctxreserve();
};

/* gc functions */
void nasal_gc::mark()
{
    std::queue<nasal_ref> bfs;
    // scan coroutine process stack when coroutine ptr is not null
    // scan main process stack when coroutine ptr is null
    // this scan process must execute because when running coroutine,
    // the nasal_co related to it will not update it's context(like `top`) until the coroutine suspends or exits.
    for(nasal_ref* i=stack;i<=top;++i)
        bfs.push(*i);
    bfs.push(funcr);
    bfs.push(upvalr);
    bfs.push(temp);
    if(coroutine) // scan main process stack
    {
        for(nasal_ref* i=main_ctx.stack;i<=main_ctx.top;++i)
            bfs.push(*i);
        bfs.push(main_ctx.funcr);
        bfs.push(main_ctx.upvalr);
    }
    
    while(!bfs.empty())
    {
        nasal_ref tmp=bfs.front();
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
                for(auto& i:tmp.func().upvalue)
                    bfs.push(i);
                break;
            case vm_upval:
                for(auto& i:tmp.upval().elems)
                    bfs.push(i);
                break;
            case vm_co:
                bfs.push(tmp.co().funcr);
                bfs.push(tmp.co().upvalr);
                for(nasal_ref* i=tmp.co().stack;i<=tmp.co().top;++i)
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
void nasal_gc::init(const std::vector<std::string>& s,const std::vector<std::string>& argv)
{
    // initiaize function register
    funcr=nil;

    for(uint8_t i=0;i<gc_tsize;++i)
        size[i]=count[i]=allocc[i]=0;
    for(uint8_t i=0;i<gc_tsize;++i)
        for(uint32_t j=0;j<ini[i];++j)
        {
            nasal_val* tmp=new nasal_val(i+vm_str);
            memory.push_back(tmp);
            unused[i].push(tmp);
        }
    coroutine=nullptr;
    // init constant strings
    strs.resize(s.size());
    for(uint32_t i=0;i<strs.size();++i)
    {
        strs[i]={vm_str,new nasal_val(vm_str)};
        strs[i].val.gcobj->unmut=1;
        strs[i].str()=s[i];
    }
    // record arguments
    env_argv.resize(argv.size());
    for(size_t i=0;i<argv.size();++i)
    {
        env_argv[i]={vm_str,new nasal_val(vm_str)};
        env_argv[i].val.gcobj->unmut=1;
        env_argv[i].str()=argv[i];
    }
}
void nasal_gc::clear()
{
    for(auto i:memory)
        delete i;
    memory.clear();
    for(uint8_t i=0;i<gc_tsize;++i)
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
    for(uint8_t i=0;i<gc_tsize;++i)
        if(count[i] || allocc[i])
            std::cout<<" "<<name[i]<<" | "<<count[i]<<","<<allocc[i]<<"\n";
    std::cout<<"\nmemory allocator info(max size)\n";
    for(uint8_t i=0;i<gc_tsize;++i)
        std::cout<<" "<<name[i]<<" | "<<ini[i]+size[i]*incr[i]<<" (+"<<size[i]<<")\n";
}
nasal_ref nasal_gc::alloc(uint8_t type)
{
    const uint8_t index=type-vm_str;
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
        for(uint32_t i=0;i<incr[index];++i)
        {
            nasal_val* tmp=new nasal_val(type);
            memory.push_back(tmp);
            unused[index].push(tmp);
        }
    }
    nasal_ref ret={type,unused[index].front()};
    ret.val.gcobj->mark=GC_UNCOLLECTED;
    unused[index].pop();
    return ret;
}
nasal_ref nasal_gc::newstr(char c)
{
    nasal_ref s=alloc(vm_str);
    s.str()=c;
    return s;
}
nasal_ref nasal_gc::newstr(const char* buff)
{
    nasal_ref s=alloc(vm_str);
    s.str()=buff;
    return s;
}
nasal_ref nasal_gc::newstr(const std::string& buff)
{
    nasal_ref s=alloc(vm_str);
    s.str()=buff;
    return s;
}
void nasal_gc::ctxchg(nasal_co& context)
{
    main_ctx.pc=pc;
    main_ctx.top=top;
    main_ctx.localr=localr;
    main_ctx.memr=memr;
    main_ctx.funcr=funcr;
    main_ctx.upvalr=upvalr;
    main_ctx.canary=canary;
    main_ctx.stack=stack;

    pc=context.pc;
    top=context.top;
    localr=context.localr;
    memr=context.memr;
    funcr=context.funcr;
    upvalr=context.upvalr;
    canary=context.canary;
    stack=context.stack;
    coroutine=&context;

    coroutine->status=nasal_co::running;
}
void nasal_gc::ctxreserve()
{
    // pc=0 means this coroutine is finished
    coroutine->status=pc?nasal_co::suspended:nasal_co::dead;
    coroutine->pc=pc;
    coroutine->localr=localr;
    coroutine->memr=memr;
    coroutine->funcr=funcr;
    coroutine->upvalr=upvalr;
    coroutine->canary=canary;
    coroutine->top=top;

    pc=main_ctx.pc;
    localr=main_ctx.localr;
    memr=main_ctx.memr;
    funcr=main_ctx.funcr;
    upvalr=main_ctx.upvalr;
    canary=main_ctx.canary;
    top=main_ctx.top;
    stack=main_ctx.stack;
    coroutine=nullptr;
}
#endif