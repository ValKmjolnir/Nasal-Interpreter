#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum nasal_type
{
    /* none-gc object */
    vm_none=0,
    vm_cnt,
    vm_addr,
    vm_ret,
    vm_nil,
    vm_num,
    /* gc object */
    vm_str,
    vm_func,
    vm_vec,
    vm_hash,
    vm_upval,
    vm_obj,
    vm_type_size
};

// change parameters here to make your own efficient gc
// better set bigger number on vm_vec
const uint32_t increment[vm_type_size]=
{
    /* none-gc object */
    0,   // vm_none, error type
    0,   // vm_count, used in foreach/forindex
    0,   // vm_addr, used to store local address pointers
    0,   // vm_ret, used to store call-return address
    0,   // vm_nil
    0,   // vm_num
    /* gc object */
    128, // vm_str
    512, // vm_func
    128, // vm_vec
    64,  // vm_hash
    512, // vm_upval
    16   // vm_obj
};

struct nasal_vec;  // vector
struct nasal_hash; // hashmap(dict)
struct nasal_func; // function(lambda)
struct nasal_upval;// upvalue
struct nasal_obj;  // special objects
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
    } value;

    // vm_none/vm_nil
    nasal_ref(const uint8_t t=vm_none):type(t){}
    // vm_ret
    nasal_ref(const uint8_t t,const uint32_t n):type(t){value.ret=n;}
    // vm_cnt
    nasal_ref(const uint8_t t,const int64_t n):type(t){value.cnt=n;}
    // vm_num
    nasal_ref(const uint8_t t,const double n):type(t){value.num=n;}
    // vm_str/vm_func/vm_vec/vm_hash/vm_upval/vm_obj
    nasal_ref(const uint8_t t,nasal_val* n):type(t){value.gcobj=n;}
    // vm_addr
    nasal_ref(const uint8_t t,nasal_ref* n):type(t){value.addr=n;}
    nasal_ref(const nasal_ref& nr):type(nr.type),value(nr.value){}
    nasal_ref& operator=(const nasal_ref& nr)
    {
        type=nr.type;
        value=nr.value;
        return *this;
    }
    bool operator==(const nasal_ref& nr){return type==nr.type && value.gcobj==nr.value.gcobj;}
    bool operator!=(const nasal_ref& nr){return type!=nr.type || value.gcobj!=nr.value.gcobj;}
    // number and string can be translated to each other
    double      to_number();
    std::string to_string();
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
    int32_t  dynpara;                            // dynamic parameter name index in hash.
    uint32_t entry;                              // pc will set to entry-1 to call this function
    uint32_t psize;                              // used to load default parameters to a new function
    uint32_t lsize;                              // used to expand memory space for local values on stack
    std::vector<nasal_ref> local;                // local scope with default value(nasal_ref)
    std::vector<nasal_ref> upvalue;              // closure
    std::unordered_map<std::string,size_t> keys; // parameter name table, size_t begins from 1

    nasal_func():dynpara(-1){}
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
    enum obj_type
    {
        file=1,
        dir,
        dylib,
        externfunc
    };
    /* RAII constructor */
    /* new object is initialized when creating */
    uint32_t type;
    void* ptr;

    /* RAII destroyer */
    /* default destroyer does nothing */
    typedef void (*dest)(void*);
    dest destructor;

    nasal_obj():ptr(nullptr),destructor(nullptr){}
    ~nasal_obj(){clear();}
    void clear()
    {
        if(destructor && ptr)
            destructor(ptr);
        ptr=nullptr;
        destructor=nullptr;
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
    std::cout<<'[';
    for(auto& i:elems)
    {
        i.print();
        std::cout<<",]"[(++iter)==elems.size()];
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
    std::cout<<'{';
    for(auto& i:elems)
    {
        std::cout<<i.first<<':';
        i.second.print();
        std::cout<<",}"[(++iter)==elems.size()];
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
    }
    type=vm_nil;
}
double nasal_ref::to_number()
{
    return type!=vm_str?value.num:str2num(str().c_str());
}
std::string nasal_ref::to_string()
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
        case vm_num:  std::cout<<value.num;     break;
        case vm_str:  std::cout<<str();         break;
        case vm_vec:  vec().print();            break;
        case vm_hash: hash().print();           break;
        case vm_func: std::cout<<"func(..){..}";break;
        case vm_obj:  std::cout<<"<object>";    break;
    }
}
bool nasal_ref::objchk(uint32_t objtype)
{
    return type==vm_obj && obj().type==objtype && obj().ptr;
}
inline nasal_ref*   nasal_ref::addr (){return value.addr;             }
inline uint32_t     nasal_ref::ret  (){return value.ret;              }
inline int64_t&     nasal_ref::cnt  (){return value.cnt;              }
inline double       nasal_ref::num  (){return value.num;              }
inline std::string& nasal_ref::str  (){return *value.gcobj->ptr.str;  }
inline nasal_vec&   nasal_ref::vec  (){return *value.gcobj->ptr.vec;  }
inline nasal_hash&  nasal_ref::hash (){return *value.gcobj->ptr.hash; }
inline nasal_func&  nasal_ref::func (){return *value.gcobj->ptr.func; }
inline nasal_upval& nasal_ref::upval(){return *value.gcobj->ptr.upval;}
inline nasal_obj&   nasal_ref::obj  (){return *value.gcobj->ptr.obj;  }

const nasal_ref zero={vm_num,(double)0};
const nasal_ref one ={vm_num,(double)1};
const nasal_ref nil ={vm_nil,(double)0};

struct nasal_gc
{
    static const uint32_t   stack_depth=8192;        // depth of value stack
    nasal_ref               funcr;                   // function register
    nasal_ref               stack[stack_depth];      // the last one is reserved to avoid stack overflow
    nasal_ref*              top;                     // stack top
    std::vector<nasal_ref>  strs;                    // reserved address for const vm_str
    std::vector<nasal_val*> memory;                  // gc memory
    std::queue<nasal_val*>  free_list[vm_type_size]; // gc free list
    /* upvalue is a temporary space to store upvalues */
    /* if no new functions generated in local scope   */
    /* upvalue will pushback(nil)                     */
    /* if new functions generated in local scope      */
    /* they will share the same upvalue stored here   */
    std::vector<nasal_ref>  upvalue;
    uint64_t                size[vm_type_size];
    uint64_t                count[vm_type_size];
    void                    mark();
    void                    sweep();
    void                    init(const std::vector<std::string>&);
    void                    clear();
    void                    info();
    nasal_ref               alloc(const uint8_t);
    nasal_ref               builtin_alloc(const uint8_t);
};

/* gc functions */
void nasal_gc::mark()
{
    std::queue<nasal_ref> bfs;
    bfs.push(funcr);
    for(auto& i:upvalue)
        bfs.push(i);
    for(nasal_ref* i=stack;i<=top;++i)
        bfs.push(*i);
    while(!bfs.empty())
    {
        nasal_ref tmp=bfs.front();
        bfs.pop();
        if(tmp.type<=vm_num || tmp.value.gcobj->mark) continue;
        tmp.value.gcobj->mark=GC_FOUND;
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
            }
            free_list[i->type].push(i);
            i->mark=GC_COLLECTED;
        }
        else if(i->mark==GC_FOUND)
            i->mark=GC_UNCOLLECTED;
    }
}
void nasal_gc::init(const std::vector<std::string>& s)
{
    // initiaize function register
    funcr=nil;

    for(uint8_t i=0;i<vm_type_size;++i)
        size[i]=count[i]=0;
    for(uint8_t i=vm_str;i<vm_type_size;++i)
        for(uint32_t j=0;j<increment[i];++j)
        {
            nasal_val* tmp=new nasal_val(i);
            memory.push_back(tmp);
            free_list[i].push(tmp);
        }
    top=stack;
    // init constant strings
    strs.resize(s.size());
    for(uint32_t i=0;i<strs.size();++i)
    {
        strs[i]={vm_str,new nasal_val(vm_str)};
        strs[i].value.gcobj->unmut=1;
        strs[i].str()=s[i];
    }
}
void nasal_gc::clear()
{
    for(auto i:memory)
        delete i;
    memory.clear();
    upvalue.clear();
    for(uint8_t i=0;i<vm_type_size;++i)
        while(!free_list[i].empty())
            free_list[i].pop();
    for(auto& i:strs)
        delete i.value.gcobj;
    strs.clear();
}
void nasal_gc::info()
{
    const char* name[]={
        "null ","cnt  ","addr ",
        "ret  ","nil  ","num  ",
        "str  ","func ","vec  ",
        "hash ","upval","obj  "
    };
    std::cout<<"\ngarbage collector info:\n";
    for(uint8_t i=vm_str;i<vm_type_size;++i)
        std::cout<<"  "<<name[i]<<" | "<<count[i]<<"\n";
    std::cout<<"\nmemory allocator info(max size):\n";
    for(uint8_t i=vm_str;i<vm_type_size;++i)
        std::cout<<"  "<<name[i]<<" | "<<(size[i]+1)*increment[i]<<"(+"<<size[i]<<")\n";
}
nasal_ref nasal_gc::alloc(uint8_t type)
{
    if(free_list[type].empty())
    {
        ++count[type];
        mark();
        sweep();
    }
    if(free_list[type].empty())
    {
        ++size[type];
        for(uint32_t i=0;i<increment[type];++i)
        {
            nasal_val* tmp=new nasal_val(type);
            memory.push_back(tmp);
            free_list[type].push(tmp);
        }
    }
    nasal_ref ret={type,free_list[type].front()};
    ret.value.gcobj->mark=GC_UNCOLLECTED;
    free_list[type].pop();
    return ret;
}
nasal_ref nasal_gc::builtin_alloc(uint8_t type)
{
    // when running a builtin function,alloc will run more than one time
    // this may cause mark-sweep in gc::alloc
    // and the value got before will be collected,this is a fatal error
    // so use builtin_alloc in builtin functions if this function uses alloc more then one time
    if(free_list[type].empty())
    {
        ++size[type];
        for(uint32_t i=0;i<increment[type];++i)
        {
            nasal_val* tmp=new nasal_val(type);
            memory.push_back(tmp);
            free_list[type].push(tmp);
        }
    }
    nasal_ref ret={type,free_list[type].front()};
    ret.value.gcobj->mark=GC_UNCOLLECTED;
    free_list[type].pop();
    return ret;
}
#endif