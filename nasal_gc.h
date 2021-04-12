#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum nasal_type
{
    vm_nil=0,
    vm_num,
    vm_str,
    vm_func,
    vm_vec,
    vm_hash
};

struct nasal_vec;
struct nasal_hash;
struct nasal_func;
struct nasal_scop;
struct nasal_val;

struct nasal_vec
{
    std::vector<nasal_val*> elems;

    void        print();
    nasal_val*  get_val(int);
    nasal_val** get_mem(int);
};

struct nasal_hash
{
    std::unordered_map<std::string,nasal_val*> elems;

    bool        check_contain(std::string&);
    void        print();
    nasal_val*  get_val(std::string&);
    nasal_val** get_mem(std::string&);
};

struct nasal_func
{
    int dynpara;
    int entry;
    std::vector<int> para;
    std::vector<nasal_val*> default_para;
    std::unordered_map<int,nasal_val*> closure;

    nasal_func();
};

struct nasal_val
{
    bool mark;
    int type;
    union 
    {
        double       num;
        std::string* str;
        nasal_vec*   vec;
        nasal_hash*  hash;
        nasal_func*  func;
    }ptr;

    nasal_val();
    nasal_val(int);
    ~nasal_val();
    void        clear();
    void        set_type(int);
    double      to_number();
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
    int size=elems.size();
    std::cout<<"[";
    if(!size)
        std::cout<<"]";
    for(int i=0;i<size;++i)
    {
        nasal_val* tmp=elems[i];
        switch(tmp->type)
        {
            case vm_nil:  std::cout<<"nil";            break;
            case vm_num:  std::cout<<tmp->ptr.num;     break;
            case vm_str:  std::cout<<*tmp->ptr.str;    break;
            case vm_vec:  tmp->ptr.vec->print();       break;
            case vm_hash: tmp->ptr.hash->print();      break;
            case vm_func: std::cout<<"func(...){...}"; break;
        }
        std::cout<<",]"[i==size-1];
    }
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
        {
            std::vector<nasal_val*>& vec_ref=val_addr->ptr.vec->elems;
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_val* tmp=vec_ref[i];
                if(tmp->type==vm_hash)
                    ret_addr=tmp->ptr.hash->get_val(key);
                if(ret_addr)
                    return ret_addr;
            }
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
        {
            std::vector<nasal_val*>& vec_ref=val_addr->ptr.vec->elems;
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_val* tmp=vec_ref[i];
                if(tmp->type==vm_hash)
                    mem_addr=tmp->ptr.hash->get_mem(key);
                if(mem_addr)
                    return mem_addr;
            }
        }
    }
    return nullptr;
}
bool nasal_hash::check_contain(std::string& key)
{
    if(elems.count(key))
        return true;
    if(elems.count("parents"))
    {
        nasal_val* val_addr=elems["parents"];
        if(val_addr->type==vm_vec)
        {
            bool result=false;
            std::vector<nasal_val*>& vec_ref=val_addr->ptr.vec->elems;
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_val* tmp=vec_ref[i];
                if(tmp->type==vm_hash)
                    result=tmp->ptr.hash->check_contain(key);
                if(result)
                    return true;
            }
        }
    }
    return false;
}
void nasal_hash::print()
{
    std::cout<<'{';
    if(!elems.size())
    {
        std::cout<<'}';
        return;
    }
    for(auto i=elems.begin();i!=elems.end();++i)
    {
        std::cout<<i->first<<':';
        nasal_val* tmp=i->second;
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

/*functions of nasal_val*/
nasal_val::nasal_val()
{
    mark=false;
    type=vm_nil;
    return;
}
nasal_val::nasal_val(int val_type)
{
    mark=false;
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
void nasal_val::clear()
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
void nasal_val::set_type(int val_type)
{
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
double nasal_val::to_number()
{
    if(type==vm_str)
        return trans_string_to_number(*ptr.str);
    return ptr.num;
}
std::string nasal_val::to_string()
{
    if(type==vm_str)
        return *ptr.str;
    else if(type==vm_num)
        return trans_number_to_string(ptr.num);
    return "";
}

struct nasal_gc
{
#define STACK_MAX_DEPTH (65536<<4)
    nasal_val*               zero_addr;   // reserved address of nasal_val,type vm_num, 0
    nasal_val*               one_addr;    // reserved address of nasal_val,type vm_num, 1
    nasal_val*               nil_addr;    // reserved address of nasal_val,type vm_nil
    nasal_val*               val_stack[STACK_MAX_DEPTH];
    nasal_val**              stack_top;   // stack top
    std::vector<nasal_val*>  num_addrs;   // reserved address for const vm_num
    std::vector<nasal_val*>  str_addrs;   // reserved address for const vm_str
    std::vector<nasal_val*>  slice_stack; // slice stack for vec[val,val,val:val]
    std::vector<nasal_val*>  memory;      // gc memory
    std::queue <nasal_val*>  free_list;   // gc free list
    std::unordered_map<int,nasal_val*> global;
    std::vector<std::unordered_map<int,nasal_val*> > local;
    void                     mark();
    void                     sweep();
    void                     gc_init(std::vector<double>&,std::vector<std::string>&);
    void                     gc_clear();
    nasal_val*               gc_alloc(int);
};

/* gc functions */
void nasal_gc::mark()
{
    std::queue<nasal_val*> bfs;
    zero_addr->mark=true;
    one_addr->mark=true;
    nil_addr->mark=true;
    for(auto i=global.begin();i!=global.end();++i)
        bfs.push(i->second);

    int size=num_addrs.size();
    for(int i=0;i<size;++i)
        bfs.push(num_addrs[i]);
    size=local.size();
    for(auto i=local.begin();i!=local.end();++i)
        for(auto j=i->begin();j!=i->end();++j)
            bfs.push(j->second);
    size=slice_stack.size();
    for(int i=0;i<size;++i)
        bfs.push(slice_stack[i]);
    for(nasal_val** i=val_stack;i<=stack_top;++i)
        bfs.push(*i);
    while(!bfs.empty())
    {
        nasal_val* tmp=bfs.front();
        bfs.pop();
        if(tmp->mark) continue;
        tmp->mark=true;
        if(tmp->type==vm_vec)
        {
            std::vector<nasal_val*>& vec=tmp->ptr.vec->elems;
            for(auto i=vec.begin();i!=vec.end();++i)
                if(!(*i)->mark)
                    bfs.push(*i);
        }
        else if(tmp->type==vm_hash)
        {
            std::unordered_map<std::string,nasal_val*>& hash=tmp->ptr.hash->elems;
            for(auto i=hash.begin();i!=hash.end();++i)
                if(!i->second->mark)
                    bfs.push(i->second);
        }
        else if(tmp->type==vm_func)
        {
            std::unordered_map<int,nasal_val*>& cls=tmp->ptr.func->closure;
            std::vector<nasal_val*>& def=tmp->ptr.func->default_para;
            for(auto i=cls.begin();i!=cls.end();++i)
                if(!i->second->mark)
                    bfs.push(i->second);
            for(auto i=def.begin();i!=def.end();++i)
                if(*i && !(*i)->mark)
                    bfs.push(*i);
        }
    }
    return;
}
void nasal_gc::sweep()
{
    int size=memory.size();
    for(int i=0;i<size;++i)
    {
        if(!memory[i]->mark)
        {
            memory[i]->clear();
            free_list.push(memory[i]);
        }
        memory[i]->mark=false;
    }
    return;
}
void nasal_gc::gc_init(std::vector<double>& nums,std::vector<std::string>& strs)
{
    for(int i=0;i<65536;++i)
    {
        nasal_val* tmp=new nasal_val;
        memory.push_back(tmp);
        free_list.push(tmp);
    }
    stack_top=val_stack; // set stack_top to val_stack

    zero_addr=new nasal_val(vm_num); // init constant 0
    zero_addr->ptr.num=0;
    memory.push_back(zero_addr);

    one_addr=new nasal_val(vm_num); // init constant 1
    one_addr->ptr.num=1;
    memory.push_back(one_addr);

    nil_addr=new nasal_val(vm_nil); // init nil
    memory.push_back(nil_addr);

    *val_stack=nil_addr; // the first space will not store any values,but gc checks

    // init constant numbers
    for(int i=0;i<nums.size();++i)
    {
        nasal_val* tmp=new nasal_val(vm_num);
        tmp->ptr.num=nums[i];
        num_addrs.push_back(tmp);
    }

    // init constant strings
    for(int i=0;i<strs.size();++i)
    {
        nasal_val* tmp=new nasal_val(vm_str);
        *tmp->ptr.str=strs[i];
        str_addrs.push_back(tmp);
    }
    return;
}
void nasal_gc::gc_clear()
{
    int size=memory.size();
    for(int i=0;i<size;++i)
    {
        memory[i]->clear();
        delete memory[i];
    }
    memory.clear();
    while(!free_list.empty())
        free_list.pop();
    global.clear();
    local.clear();
    slice_stack.clear();

    for(int i=0;i<num_addrs.size();++i)
        delete num_addrs[i];
    num_addrs.clear();
    for(int i=0;i<str_addrs.size();++i)
        delete str_addrs[i];
    str_addrs.clear();
    return;
}
nasal_val* nasal_gc::gc_alloc(int type)
{
    if(free_list.empty())
    {
        mark();
        sweep();
    }
    if(free_list.empty())
        for(int i=0;i<65536;++i)
        {
            nasal_val* tmp=new nasal_val;
            memory.push_back(tmp);
            free_list.push(tmp);
        }
    nasal_val* ret=free_list.front();
    free_list.pop();
    ret->set_type(type);
    return ret;
}
#endif