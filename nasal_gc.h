#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum nasal_type
{
    vm_nil=0,
    vm_num,
    vm_str,
    vm_func,
    vm_vec,
    vm_hash,
    vm_type_size
};

// change parameters here to make your own efficient gc
// better set bigger number on vm_num and vm_vec
const int increment[vm_type_size]=
{
    0,    // vm_nil,in fact it is not in use
    65536,// vm_num
    256,  // vm_str
    16,   // vm_func
    2048, // vm_vec
    8     // vm_hash
};

struct nasal_val;//declaration of nasal_val

struct nasal_vec// 24 bytes
{
    std::vector<nasal_val*> elems;

    void        print();
    nasal_val*  get_val(int);
    nasal_val** get_mem(int);
};

struct nasal_hash// 56 bytes
{
    std::unordered_map<std::string,nasal_val*> elems;

    void        print();
    nasal_val*  get_val(std::string&);
    nasal_val** get_mem(std::string&);
};

struct nasal_func// 120 bytes
{
    int32_t  dynpara;// dynamic parameter name index in hash
    uint32_t offset; // arguments will be loaded into local scope from this offset 
    uint32_t entry;  // pc will set to entry-1 to call this function
    std::vector<nasal_val*> default_para;// default value(nasal_val*)
    std::unordered_map<std::string,int> key_table;// parameter name hash
    std::vector<nasal_val*> closure;// closure will be loaded to gc.local.back() as the local scope

    nasal_func();
    void clear();
};

struct nasal_val// 16 bytes
{
#define GC_UNCOLLECTED 0   
#define GC_FOUND       1
#define GC_COLLECTED   2
    uint8_t mark;
    uint16_t type;
    union 
    {
        double       num;
        std::string* str;
        nasal_vec*   vec;
        nasal_hash*  hash;
        nasal_func*  func;
    }ptr;

    nasal_val(int);
    ~nasal_val();
    double to_number();
    std::string to_string();
};

/*functions of nasal_vec*/
nasal_val* nasal_vec::get_val(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
    {
        std::cout<<">> [gc] nasal_vec::get_val: index out of range: "<<index<<"\n";
        return nullptr;
    }
    int idx[2]={index+vec_size,index};
    return elems[idx[index>=0]];
}
nasal_val** nasal_vec::get_mem(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
    {
        std::cout<<">> [gc] nasal_vec::get_mem: index out of range: "<<index<<"\n";
        return nullptr;
    }
    int idx[2]={index+vec_size,index};
    return &elems[idx[index>=0]];
}
void nasal_vec::print()
{
    std::cout<<'[';  
    for(auto i:elems)
    {
        switch(i->type)
        {
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<i->ptr.num;       break;
            case vm_str:  std::cout<<*i->ptr.str;      break;
            case vm_vec:  i->ptr.vec->print();         break;
            case vm_hash: i->ptr.hash->print();        break;
            case vm_func: std::cout<<"func(...){...}"; break;
        }
        std::cout<<',';
    }
    std::cout<<']';
    return;
}

/*functions of nasal_hash*/
nasal_val* nasal_hash::get_val(std::string& key)
{
    nasal_val* ret_addr=nullptr;
    if(elems.count(key))
        return elems[key];
    else if(elems.count("parents"))
    {
        nasal_val* val_addr=elems["parents"];
        if(val_addr->type==vm_vec)
            for(auto i:val_addr->ptr.vec->elems)
            {
                if(i->type==vm_hash)
                    ret_addr=i->ptr.hash->get_val(key);
                if(ret_addr)
                    return ret_addr;
            }
    }
    return nullptr;
}
nasal_val** nasal_hash::get_mem(std::string& key)
{
    nasal_val** mem_addr=nullptr;
    if(elems.count(key))
        return &elems[key];
    else if(elems.count("parents"))
    {
        nasal_val* val_addr=elems["parents"];
        if(val_addr->type==vm_vec)
            for(auto i:val_addr->ptr.vec->elems)
            {
                if(i->type==vm_hash)
                    mem_addr=i->ptr.hash->get_mem(key);
                if(mem_addr)
                    return mem_addr;
            }
    }
    return nullptr;
}
void nasal_hash::print()
{
    std::cout<<'{';
    for(auto& i:elems)
    {
        std::cout<<i.first<<':';
        nasal_val* tmp=i.second;
        switch(tmp->type)
        {
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<tmp->ptr.num;     break;
            case vm_str:  std::cout<<*tmp->ptr.str;    break;
            case vm_vec:  tmp->ptr.vec->print();       break;
            case vm_hash: tmp->ptr.hash->print();      break;
            case vm_func: std::cout<<"func(...){...}"; break;
        }
        std::cout<<',';
    }
    std::cout<<'}';
    return;
}

/*functions of nasal_func*/
nasal_func::nasal_func()
{
    dynpara=-1;
    return;
}
void nasal_func::clear()
{
    dynpara=-1;
    default_para.clear();
    key_table.clear();
    closure.clear();
    return;
}

/*functions of nasal_val*/
nasal_val::nasal_val(int val_type)
{
    mark=GC_COLLECTED;
    type=val_type;
    switch(type)
    {
        case vm_num:  ptr.num=0;               break;
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
double nasal_val::to_number()
{
    if(type==vm_str)
        return str2num(ptr.str->c_str());
    return ptr.num;
}
std::string nasal_val::to_string()
{
    if(type==vm_str)
        return *ptr.str;
    else if(type==vm_num)
        return num2str(ptr.num);
    return "";
}

struct nasal_gc
{
#define STACK_MAX_DEPTH (65536<<1)
    nasal_val*              zero_addr;               // reserved address of nasal_val,type vm_num, 0
    nasal_val*              one_addr;                // reserved address of nasal_val,type vm_num, 1
    nasal_val*              nil_addr;                // reserved address of nasal_val,type vm_nil
    nasal_val*              val_stack[STACK_MAX_DEPTH+16];// 16 reserved to avoid stack overflow
    nasal_val**             stack_top;               // stack top
    std::vector<nasal_val*> num_addrs;               // reserved address for const vm_num
    std::vector<nasal_val*> str_addrs;               // reserved address for const vm_str
    std::vector<nasal_val*> slice_stack;             // slice stack for vec[val,val,val:val]
    std::vector<nasal_val*> memory;                  // gc memory
    std::queue <nasal_val*> free_list[vm_type_size]; // gc free list
    std::vector<nasal_val*> global;
    std::list<std::vector<nasal_val*>> local;
    void                    mark();
    void                    sweep();
    void                    gc_init(std::vector<double>&,std::vector<std::string>&);
    void                    gc_clear();
    nasal_val*              gc_alloc(int);
    nasal_val*              builtin_alloc(int);
};

/* gc functions */
void nasal_gc::mark()
{
    std::queue<nasal_val*> bfs;
    for(auto i:global)
        bfs.push(i);
    for(auto& i:local)
        for(auto j:i)
            bfs.push(j);
    for(auto i:slice_stack)
        bfs.push(i);
    for(nasal_val** i=val_stack;i<=stack_top;++i)
        bfs.push(*i);
    while(!bfs.empty())
    {
        nasal_val* tmp=bfs.front();
        bfs.pop();
        if(!tmp || tmp->mark) continue;
        tmp->mark=GC_FOUND;
        if(tmp->type==vm_vec)
            for(auto i:tmp->ptr.vec->elems)
                bfs.push(i);
        else if(tmp->type==vm_hash)
            for(auto& i:tmp->ptr.hash->elems)
                bfs.push(i.second);
        else if(tmp->type==vm_func)
        {
            for(auto i:tmp->ptr.func->closure)
                bfs.push(i);
            for(auto i:tmp->ptr.func->default_para)
                bfs.push(i);
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
void nasal_gc::gc_init(std::vector<double>& nums,std::vector<std::string>& strs)
{
    for(int i=vm_num;i<vm_type_size;++i)
        for(int j=0;j<increment[i];++j)
        {
            nasal_val* tmp=new nasal_val(i);
            memory.push_back(tmp);
            free_list[i].push(tmp);
        }

    stack_top=val_stack; // set stack_top to val_stack

    zero_addr=new nasal_val(vm_num); // init constant 0
    zero_addr->ptr.num=0;

    one_addr=new nasal_val(vm_num); // init constant 1
    one_addr->ptr.num=1;

    nil_addr=new nasal_val(vm_nil); // init nil

    *val_stack=nil_addr; // the first space will not store any values,but gc checks
    // init constant numbers
    num_addrs.resize(nums.size());
    for(int i=0;i<nums.size();++i)
    {
        num_addrs[i]=new nasal_val(vm_num);
        num_addrs[i]->ptr.num=nums[i];
    }
    // init constant strings
    str_addrs.resize(strs.size());
    for(int i=0;i<strs.size();++i)
    {
        str_addrs[i]=new nasal_val(vm_str);
        *str_addrs[i]->ptr.str=strs[i];
    }
    return;
}
void nasal_gc::gc_clear()
{
    for(auto i:memory)
        delete i;
    memory.clear();
    for(int i=0;i<vm_type_size;++i)
        while(!free_list[i].empty())
            free_list[i].pop();
    global.clear();
    local.clear();
    slice_stack.clear();

    delete nil_addr;
    delete one_addr;
    delete zero_addr;
    for(auto i:num_addrs)
        delete i;
    num_addrs.clear();
    for(auto i:str_addrs)
        delete i;
    str_addrs.clear();
    return;
}
nasal_val* nasal_gc::gc_alloc(int type)
{
    if(free_list[type].empty())
    {
        mark();
        sweep();
    }
    if(free_list[type].empty())
        for(int i=0;i<increment[type];++i)
        {
            nasal_val* tmp=new nasal_val(type);
            memory.push_back(tmp);
            free_list[type].push(tmp);
        }
    nasal_val* ret=free_list[type].front();
    ret->mark=GC_UNCOLLECTED;
    free_list[type].pop();
    return ret;
}
nasal_val* nasal_gc::builtin_alloc(int type)
{
    // when running a builtin function,alloc will run more than one time
    // this may cause mark-sweep in gc_alloc
    // and the value got before will be collected,this is a fatal error
    // so use builtin_alloc in builtin functions if this function uses alloc more then one time
    if(free_list[type].empty())
        for(int i=0;i<increment[type];++i)
        {
            nasal_val* tmp=new nasal_val(type);
            memory.push_back(tmp);
            free_list[type].push(tmp);
        }
    nasal_val* ret=free_list[type].front();
    ret->mark=GC_UNCOLLECTED;
    free_list[type].pop();
    return ret;
}
#endif