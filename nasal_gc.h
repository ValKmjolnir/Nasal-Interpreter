#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum nasal_type
{
    // none-gc object
    vm_none=0,
    vm_nil,
    vm_num,
    // gc object
    vm_str,
    vm_func,
    vm_vec,
    vm_hash,
    vm_obj,
    vm_type_size
};

// change parameters here to make your own efficient gc
// better set bigger number on vm_num and vm_vec
const uint32_t increment[vm_type_size]=
{
    // none-gc object
    0,    // vm_none, error type
    0,    // vm_nil
    0,    // vm_num
    // gc object
    2048, // vm_str
    1024, // vm_func
    8192, // vm_vec
    512,  // vm_hash
    0     // vm_obj
};

// declaration of nasal value type
struct nasal_vec;
struct nasal_hash;
struct nasal_func;
struct nasal_val;
// declaration of nasal_ref
struct nasal_ref// 16 bytes
{
    uint8_t type;
    union
    {
        double num;
        nasal_val* gcobj;
    }value;

    nasal_ref(const uint8_t t=vm_none):type(t){}
    nasal_ref(const uint8_t t,const double n):type(t){value.num=n;}
    nasal_ref(const uint8_t t,nasal_val* n):type(t){value.gcobj=n;}
    nasal_ref(const nasal_ref& nr)
    {
        type=nr.type;
        value=nr.value;
    }
    nasal_ref(const nasal_ref&& nr)
    {
        type=nr.type;
        value=nr.value;
    }
    nasal_ref& operator=(const nasal_ref& nr)
    {
        type=nr.type;
        value=nr.value;
        return *this;
    }
    nasal_ref& operator=(const nasal_ref&& nr)
    {
        type=nr.type;
        value=nr.value;
        return *this;
    }
    bool operator==(const nasal_ref& nr){return type==nr.type && value.gcobj==nr.value.gcobj;}
    bool operator!=(const nasal_ref& nr){return type!=nr.type || value.gcobj!=nr.value.gcobj;}
    // nasal is a weak-type programming language because number and string can be translated to each other
    double      to_number();
    std::string to_string();
    // inline function to get number and pointers, make it easier to read the code
    inline double&      num ();
    inline std::string* str ();
    inline nasal_vec*   vec ();
    inline nasal_hash*  hash();
    inline nasal_func*  func();
    inline void*        obj ();
};

struct nasal_vec// 24 bytes
{
    std::vector<nasal_ref> elems;

    void       print();
    nasal_ref  get_val(int);
    nasal_ref* get_mem(int);
};

struct nasal_hash// 56 bytes
{
    std::unordered_map<std::string,nasal_ref> elems;

    void       print();
    nasal_ref  get_val(std::string&);
    nasal_ref* get_mem(std::string&);
};

struct nasal_func// 112 bytes
{
    int32_t  dynpara;                              // dynamic parameter name index in hash
    uint32_t entry;                                // pc will set to entry-1 to call this function
    std::vector<nasal_ref> local;                  // local scope with default value(nasal_ref)
    std::vector<nasal_ref> upvalue;                // closure
    std::unordered_map<std::string,int> key_table; // parameter name table

    nasal_func():dynpara(-1){}
    void clear();
};

struct nasal_val// 16 bytes
{
#define GC_UNCOLLECTED 0   
#define GC_COLLECTED   1
#define GC_FOUND       2
    uint8_t mark;
    uint8_t type;
    union 
    {
        std::string* str;
        nasal_vec*   vec;
        nasal_hash*  hash;
        nasal_func*  func;
        void*        obj;
    }ptr;

    nasal_val(uint8_t);
    ~nasal_val();
};

/*functions of nasal_vec*/
nasal_ref nasal_vec::get_val(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
        return nasal_ref(vm_none);
    return elems[index>=0?index:index+vec_size];
}
nasal_ref* nasal_vec::get_mem(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
        return nullptr;
    return &elems[index>=0?index:index+vec_size];
}
void nasal_vec::print()
{
    static int depth=0;
    if(++depth>32)
    {
        std::cout<<"[..]";
        --depth;
        return;
    }
    if(!elems.size())
    {
        std::cout<<"[]";
        return;
    }
    ssize_t iter=0;
    std::cout<<'[';
    for(auto& i:elems)
    {
        switch(i.type)
        {
            case vm_none: std::cout<<"undefined";   break;
            case vm_nil:  std::cout<<"nil";         break;
            case vm_num:  std::cout<<i.num();       break;
            case vm_str:  std::cout<<*i.str();      break;
            case vm_vec:  i.vec()->print();         break;
            case vm_hash: i.hash()->print();        break;
            case vm_func: std::cout<<"func(..){..}";break;
            case vm_obj:  std::cout<<"<object>";    break;
        }
        std::cout<<",]"[(++iter)==elems.size()];
    }
    --depth;
    return;
}

/*functions of nasal_hash*/
nasal_ref nasal_hash::get_val(std::string& key)
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
    return nasal_ref(vm_none);
}
nasal_ref* nasal_hash::get_mem(std::string& key)
{
    if(elems.count(key))
        return &elems[key];
    else if(elems.count("parents"))
    {
        nasal_ref* mem_addr=nullptr;
        nasal_ref val=elems["parents"];
        if(val.type==vm_vec)
            for(auto& i:val.vec()->elems)
            {
                if(i.type==vm_hash)
                    mem_addr=i.hash()->get_mem(key);
                if(mem_addr)
                    return mem_addr;
            }
    }
    return nullptr;
}
void nasal_hash::print()
{
    static int depth=0;
    if(++depth>32)
    {
        std::cout<<"{..}";
        --depth;
        return;
    }
    if(!elems.size())
    {
        std::cout<<"{}";
        return;
    }
    size_t iter=0;
    std::cout<<'{';
    for(auto& i:elems)
    {
        std::cout<<i.first<<':';
        nasal_ref tmp=i.second;
        switch(tmp.type)
        {
            case vm_none: std::cout<<"undefined";   break;
            case vm_nil:  std::cout<<"nil";         break;
            case vm_num:  std::cout<<tmp.num();     break;
            case vm_str:  std::cout<<*tmp.str();    break;
            case vm_vec:  tmp.vec()->print();       break;
            case vm_hash: tmp.hash()->print();      break;
            case vm_func: std::cout<<"func(..){..}";break;
            case vm_obj:  std::cout<<"<object>";    break;
        }
        std::cout<<",}"[(++iter)==elems.size()];
    }
    --depth;
    return;
}

/*functions of nasal_func*/
void nasal_func::clear()
{
    dynpara=-1;
    local.clear();
    key_table.clear();
    return;
}

/*functions of nasal_val*/
nasal_val::nasal_val(uint8_t val_type)
{
    mark=GC_COLLECTED;
    type=val_type;
    switch(type)
    {
        case vm_str:  ptr.str=new std::string; break;
        case vm_vec:  ptr.vec=new nasal_vec;   break;
        case vm_hash: ptr.hash=new nasal_hash; break;
        case vm_func: ptr.func=new nasal_func; break;
    }
    return;
}
nasal_val::~nasal_val()
{
    switch(type)
    {
        case vm_str:  delete ptr.str;  break;
        case vm_vec:  delete ptr.vec;  break;
        case vm_hash: delete ptr.hash; break;
        case vm_func: delete ptr.func; break;
    }
    type=vm_nil;
    return;
}

/* functions of nasal_ref */
double nasal_ref::to_number()
{
    if(type==vm_str)
        return str2num(str()->c_str());
    return num();
}
std::string nasal_ref::to_string()
{
    if(type==vm_str)
        return *str();
    else if(type==vm_num)
        return std::to_string(num());
    return "";
}
inline double&      nasal_ref::num (){return value.num;}
inline std::string* nasal_ref::str (){return value.gcobj->ptr.str;}
inline nasal_vec*   nasal_ref::vec (){return value.gcobj->ptr.vec;}
inline nasal_hash*  nasal_ref::hash(){return value.gcobj->ptr.hash;}
inline nasal_func*  nasal_ref::func(){return value.gcobj->ptr.func;}
inline void*        nasal_ref::obj (){return value.gcobj->ptr.obj;}

struct nasal_gc
{
#define STACK_MAX_DEPTH (4095)
    nasal_ref               zero;               // reserved address of nasal_val,type vm_num, 0
    nasal_ref               one;                // reserved address of nasal_val,type vm_num, 1
    nasal_ref               nil;                // reserved address of nasal_val,type vm_nil
    nasal_ref               val_stack[STACK_MAX_DEPTH+1];// 1 reserved to avoid stack overflow, stack grows 1 each time
    nasal_ref*              stack_top;               // stack top
    std::vector<nasal_ref>  str_addrs;               // reserved address for const vm_str
    std::vector<nasal_val*> memory;                  // gc memory
    std::queue<nasal_val*>  free_list[vm_type_size]; // gc free list
    std::vector<nasal_ref>  local;
    void                    mark();
    void                    sweep();
    void                    gc_init(const std::vector<std::string>&);
    void                    gc_clear();
    nasal_ref               gc_alloc(const uint8_t);
    nasal_ref               builtin_alloc(const uint8_t);
};

/* gc functions */
void nasal_gc::mark()
{
    std::queue<nasal_ref> bfs;
    for(auto i:local)
        bfs.push(i);
    for(nasal_ref* i=val_stack;i<=stack_top;++i)
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
    return;
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
            }
            free_list[i->type].push(i);
            i->mark=GC_COLLECTED;
        }
        else if(i->mark==GC_FOUND)
            i->mark=GC_UNCOLLECTED;
    }
    return;
}
void nasal_gc::gc_init(const std::vector<std::string>& strs)
{
    for(uint8_t i=vm_str;i<vm_type_size;++i)
        for(uint32_t j=0;j<increment[i];++j)
        {
            nasal_val* tmp=new nasal_val(i);
            memory.push_back(tmp);
            free_list[i].push(tmp);
        }

    stack_top=val_stack;     // set stack_top to val_stack

    zero={vm_num,(double)0}; // init constant 0
    one ={vm_num,(double)1}; // init constant 1
    nil ={vm_nil,(double)0}; // init constant nil

    // init constant strings
    str_addrs.resize(strs.size());
    for(uint32_t i=0;i<strs.size();++i)
    {
        str_addrs[i]={vm_str,new nasal_val(vm_str)};
        *str_addrs[i].str()=strs[i];
    }
    return;
}
void nasal_gc::gc_clear()
{
    for(auto i:memory)
        delete i;
    memory.clear();
    for(uint8_t i=0;i<vm_type_size;++i)
        while(!free_list[i].empty())
            free_list[i].pop();
    local.clear();

    for(auto& i:str_addrs)
        delete i.value.gcobj;
    str_addrs.clear();
    return;
}
nasal_ref nasal_gc::gc_alloc(uint8_t type)
{
    if(free_list[type].empty())
    {
        mark();
        sweep();
    }
    if(free_list[type].empty())
        for(uint32_t i=0;i<increment[type];++i)
        {
            nasal_val* tmp=new nasal_val(type);
            memory.push_back(tmp);
            free_list[type].push(tmp);
        }
    nasal_ref ret={type,free_list[type].front()};
    ret.value.gcobj->mark=GC_UNCOLLECTED;
    free_list[type].pop();
    return ret;
}
nasal_ref nasal_gc::builtin_alloc(uint8_t type)
{
    // when running a builtin function,alloc will run more than one time
    // this may cause mark-sweep in gc_alloc
    // and the value got before will be collected,this is a fatal error
    // so use builtin_alloc in builtin functions if this function uses alloc more then one time
    if(free_list[type].empty())
        for(uint32_t i=0;i<increment[type];++i)
        {
            nasal_val* tmp=new nasal_val(type);
            memory.push_back(tmp);
            free_list[type].push(tmp);
        }
    nasal_ref ret={type,free_list[type].front()};
    ret.value.gcobj->mark=GC_UNCOLLECTED;
    free_list[type].pop();
    return ret;
}
#endif