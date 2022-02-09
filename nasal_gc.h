#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum nasal_type
{
    /* none-gc object */
    vm_none=0,
    vm_cnt,
    vm_ret,
    vm_nil,
    vm_num,
    /* gc object */
    vm_str,
    vm_func,
    vm_vec,
    vm_hash,
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
    0,   // vm_ret, used to store call-return address
    0,   // vm_nil
    0,   // vm_num
    /* gc object */
    256, // vm_str
    512, // vm_func
    512, // vm_vec
    128, // vm_hash
    16   // vm_obj
};

struct nasal_vec; // vector
struct nasal_hash;// hashmap(dict)
struct nasal_func;// function(lambda)
struct nasal_obj; // special objects
struct nasal_val; // nasal_val includes gc-managed types

struct nasal_ref
{
    uint8_t type;
    union
    {
        uint32_t ret;
        int64_t  cnt;
        double   num;
        nasal_val* gcobj;
    }value;

    nasal_ref(const uint8_t t=vm_none):type(t){}
    nasal_ref(const uint8_t t,const uint32_t n):type(t){value.ret=n;}
    nasal_ref(const uint8_t t,const int64_t n):type(t){value.cnt=n;}
    nasal_ref(const uint8_t t,const double n):type(t){value.num=n;}
    nasal_ref(const uint8_t t,nasal_val* n):type(t){value.gcobj=n;}
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
    inline uint32_t     ret ();
    inline int64_t&     cnt ();
    inline double&      num ();
    inline std::string* str ();
    inline nasal_vec*   vec ();
    inline nasal_hash*  hash();
    inline nasal_func*  func();
    inline nasal_obj*   obj ();
};

struct nasal_vec
{
    std::vector<nasal_ref> elems;

    void       print();
    nasal_ref  get_val(const int);
    nasal_ref* get_mem(const int);
};

struct nasal_hash
{
    std::unordered_map<std::string,nasal_ref> elems;

    void       print();
    nasal_ref  get_val(const std::string&);
    nasal_ref* get_mem(const std::string&);
};

struct nasal_func
{
    int32_t  dynpara;                            // dynamic parameter name index in hash.
    uint32_t entry;                              // pc will set to entry-1 to call this function
    std::vector<nasal_ref> local;                // local scope with default value(nasal_ref)
    std::vector<nasal_ref> upvalue;              // closure
    std::unordered_map<std::string,size_t> keys; // parameter name table

    nasal_func():dynpara(-1){}
    void clear();
};

struct nasal_obj
{
    uint32_t type;
    void* ptr;
    nasal_obj():ptr(nullptr){}
    void clear(){ptr=nullptr;}
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
    static int depth=0;
    if(!elems.size())
    {
        std::cout<<"[]";
        return;
    }
    if(++depth>16)
    {
        std::cout<<"[..]";
        --depth;
        return;
    }
    size_t iter=0;
    std::cout<<'[';
    for(auto& i:elems)
    {
        i.print();
        std::cout<<",]"[(++iter)==elems.size()];
    }
    --depth;
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
            for(auto& i:val.vec()->elems)
            {
                if(i.type==vm_hash)
                    ret=i.hash()->get_val(key);
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
            for(auto& i:val.vec()->elems)
            {
                if(i.type==vm_hash)
                    addr=i.hash()->get_mem(key);
                if(addr)
                    return addr;
            }
    }
    return nullptr;
}
void nasal_hash::print()
{
    static int depth=0;
    if(!elems.size())
    {
        std::cout<<"{}";
        return;
    }
    if(++depth>16)
    {
        std::cout<<"{..}";
        --depth;
        return;
    }
    size_t iter=0;
    std::cout<<'{';
    for(auto& i:elems)
    {
        std::cout<<i.first<<':';
        i.second.print();
        std::cout<<",}"[(++iter)==elems.size()];
    }
    --depth;
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
        case vm_str:  ptr.str=new std::string; break;
        case vm_vec:  ptr.vec=new nasal_vec;   break;
        case vm_hash: ptr.hash=new nasal_hash; break;
        case vm_func: ptr.func=new nasal_func; break;
        case vm_obj:  ptr.obj=new nasal_obj;   break;
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
        case vm_obj:  delete ptr.obj;  break;
    }
    type=vm_nil;
}
double nasal_ref::to_number()
{
    return type!=vm_str?value.num:str2num(str()->c_str());
}
std::string nasal_ref::to_string()
{
    if(type==vm_str)
        return *str();
    else if(type==vm_num)
        return std::to_string(num());
    return "";
}
void nasal_ref::print()
{
    switch(type)
    {
        case vm_none: std::cout<<"undefined";   break;
        case vm_nil:  std::cout<<"nil";         break;
        case vm_num:  std::cout<<value.num;     break;
        case vm_str:  std::cout<<*(this->str());break;
        case vm_vec:  this->vec()->print();     break;
        case vm_hash: this->hash()->print();    break;
        case vm_func: std::cout<<"func(..){..}";break;
        case vm_obj:  std::cout<<"<object>";    break;
    }
}
inline uint32_t     nasal_ref::ret (){return value.ret;            }
inline int64_t&     nasal_ref::cnt (){return value.cnt;            }
inline double&      nasal_ref::num (){return value.num;            }
inline std::string* nasal_ref::str (){return value.gcobj->ptr.str; }
inline nasal_vec*   nasal_ref::vec (){return value.gcobj->ptr.vec; }
inline nasal_hash*  nasal_ref::hash(){return value.gcobj->ptr.hash;}
inline nasal_func*  nasal_ref::func(){return value.gcobj->ptr.func;}
inline nasal_obj*   nasal_ref::obj (){return value.gcobj->ptr.obj; }

const uint32_t STACK_MAX_DEPTH=4095;
const nasal_ref zero={vm_num,(double)0};
const nasal_ref one ={vm_num,(double)1};
const nasal_ref nil ={vm_nil,nullptr};
struct nasal_gc
{
    nasal_ref               stack[STACK_MAX_DEPTH+1];// 1 reserved to avoid stack overflow
    nasal_ref*              top;                     // stack top
    std::vector<nasal_ref>  strs;                    // reserved address for const vm_str
    std::vector<nasal_val*> memory;                  // gc memory
    std::queue<nasal_val*>  free_list[vm_type_size]; // gc free list
    std::vector<nasal_ref>  local;
    size_t                  size[vm_type_size];
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
    for(auto& i:local)
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
                for(auto& i:tmp.vec()->elems)
                    bfs.push(i);
                break;
            case vm_hash:
                for(auto& i:tmp.hash()->elems)
                    bfs.push(i.second);
                break;
            case vm_func:
                for(auto& i:tmp.func()->local)
                    bfs.push(i);
                for(auto& i:tmp.func()->upvalue)
                    bfs.push(i);
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
                case vm_str: i->ptr.str->clear();       break;
                case vm_vec: i->ptr.vec->elems.clear(); break;
                case vm_hash:i->ptr.hash->elems.clear();break;
                case vm_func:i->ptr.func->clear();      break;
                case vm_obj: i->ptr.obj->clear();       break;
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
    for(uint8_t i=0;i<vm_type_size;++i)
    {
        size[i]=increment[i];
        count[i]=0;
    }
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
        *strs[i].str()=s[i];
    }
}
void nasal_gc::clear()
{
    for(auto i:memory)
        delete i;
    memory.clear();
    for(uint8_t i=0;i<vm_type_size;++i)
        while(!free_list[i].empty())
            free_list[i].pop();
    local.clear();
    for(auto& i:strs)
        delete i.value.gcobj;
    strs.clear();
}
void nasal_gc::info()
{
    const char* name[]={
        "null","cnt ","ret ",
        "nil ","num ","str ",
        "func","vec ","hash","obj "
    };
    std::cout<<"\ngarbage collector info:\n";
    for(uint8_t i=vm_str;i<vm_type_size;++i)
        std::cout<<name[i]<<" | "<<count[i]<<"\n";
    std::cout<<"\nmemory allocator info(max size):\n";
    for(uint8_t i=vm_str;i<vm_type_size;++i)
        std::cout<<name[i]<<" | "<<size[i]<<"(+"<<size[i]/increment[i]-1<<")\n";
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
        size[type]+=increment[type];
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
        size[type]+=increment[type];
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