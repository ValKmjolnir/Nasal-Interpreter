#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum runtime_scalar_type
{
    vm_nil=0,
    vm_num,
    vm_str,
    vm_scop,
    vm_func,
    vm_vec,
    vm_hash
};

class nasal_gc;
class nasal_vec;
class nasal_hash;
class nasal_func;
class nasal_scop;
class nasal_val;

class nasal_vec
{
private:
    nasal_gc& gc;
    std::vector<nasal_val*> elems;
public:
    nasal_vec(nasal_gc&);
    ~nasal_vec();
    int  size();
    void add_elem(nasal_val*);
    void print();
    nasal_val*  del_elem();
    nasal_val*  operator[](const int);
    nasal_val*  get_value_address(int);
    nasal_val** get_mem_address(int);
};

class nasal_hash
{
private:
    nasal_gc& gc;
    std::map<std::string,nasal_val*> elems;
public:
    nasal_hash(nasal_gc&);
    ~nasal_hash();
    int  size();
    bool check_contain(std::string);
    void add_elem(std::string,nasal_val*);
    void del_elem(std::string);
    void print();
    nasal_val*  get_special_para(std::string);
    nasal_val*  get_value_address(std::string);
    nasal_val** get_mem_address(std::string);
    nasal_val*  get_keys();
};

class nasal_func
{
private:
    nasal_gc& gc;
    int entry;
    nasal_val* closure_addr;
    std::vector<int> para;
    int dynpara;
    std::vector<nasal_val*> default_para_addr;
public:
    nasal_func(nasal_gc&);
    ~nasal_func();
    int  get_entry();
    int  get_dynamic_para();
    void set_entry(int);
    void add_para(int,nasal_val*,bool);
    void set_closure_addr(nasal_val*);
    void set_new_closure();
    nasal_val* get_closure_addr();
    std::vector<int>& get_para();
    std::vector<nasal_val*>& get_default();
};

class nasal_scop
{
private:
    nasal_gc& gc;
    std::list<std::map<int,nasal_val*> > elems;
public:
    nasal_scop(nasal_gc&);
    ~nasal_scop();
    void add_scope();
    void del_scope();
    void set_closure(nasal_scop&);
    void add_new_value(int,nasal_val*);
    nasal_val*  get_value_address(int);
    nasal_val** get_mem_address(int);
};

class nasal_val
{
protected:
    int type;
    union 
    {
        double num;
        std::string* str;
        nasal_vec*   vec;
        nasal_hash*  hash;
        nasal_func*  func;
        nasal_scop* cls;
    }ptr;
public:
    int ref_cnt;
    nasal_val();
    nasal_val(int,nasal_gc&);
    ~nasal_val();
    void clear();
    void set_type(int,nasal_gc&);
    void set_number(double);
    void set_string(std::string);
    int         get_type();
    double      to_number();
    double      get_number();
    std::string to_string();
    std::string get_string();
    nasal_vec&  get_vector();
    nasal_hash& get_hash();
    nasal_func& get_func();
    nasal_scop& get_closure();
};

class nasal_gc
{
private:
    std::queue<nasal_val*>  free_space;
    std::vector<nasal_val*> memory;
public:
    ~nasal_gc();
    void clear();
    void debug();
    nasal_val* gc_alloc(int);
    void add_reference(nasal_val*);
    void del_reference(nasal_val*);
};

/*functions of nasal_vec*/
nasal_vec::nasal_vec(nasal_gc& ngc):gc(ngc)
{
    return;
}
nasal_vec::~nasal_vec()
{
    int size=elems.size();
    for(int i=0;i<size;++i)
        gc.del_reference(elems[i]);
    elems.clear();
    return;
}
void nasal_vec::add_elem(nasal_val* value_address)
{
    elems.push_back(value_address);
    return;
}
nasal_val* nasal_vec::del_elem()
{
    // pop back
    if(!elems.size())
        return NULL;
    nasal_val* ret=elems.back();
    elems.pop_back();
    return ret;
}
int nasal_vec::size()
{
    return elems.size();
}
nasal_val* nasal_vec::operator[](const int index)
{
    return elems[index];
}
nasal_val* nasal_vec::get_value_address(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
    {
        std::cout<<">> [gc] nasal_vec::get_value_address: index out of range: "<<index<<"\n";
        return NULL;
    }
    int idx[2]={index+vec_size,index};
    return elems[idx[index>=0]];
}
nasal_val** nasal_vec::get_mem_address(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
    {
        std::cout<<">> [gc] nasal_vec::get_mem_address: index out of range: "<<index<<"\n";
        return NULL;
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
        switch(tmp->get_type())
        {
            case vm_nil:      std::cout<<"nil";             break;
            case vm_num:   std::cout<<tmp->get_number(); break;
            case vm_str:   std::cout<<tmp->get_string(); break;
            case vm_vec:   tmp->get_vector().print();    break;
            case vm_hash:     tmp->get_hash().print();      break;
            case vm_func: std::cout<<"func(...){...}";  break;
        }
        std::cout<<",]"[i==size-1];
    }
    return;
}

/*functions of nasal_hash*/
nasal_hash::nasal_hash(nasal_gc& ngc):gc(ngc)
{
    return;
}
nasal_hash::~nasal_hash()
{
    for(std::map<std::string,nasal_val*>::iterator iter=elems.begin();iter!=elems.end();++iter)
        gc.del_reference(iter->second);
    elems.clear();
    return;
}
void nasal_hash::add_elem(std::string key,nasal_val* value_address)
{
    if(elems.find(key)==elems.end())
        elems[key]=value_address;
    return;
}
void nasal_hash::del_elem(std::string key)
{
    if(elems.find(key)!=elems.end())
    {
        gc.del_reference(elems[key]);
        elems.erase(key);
    }
    return;
}
int nasal_hash::size()
{
    return elems.size();
}
nasal_val* nasal_hash::get_special_para(std::string key)
{
    if(elems.find(key)!=elems.end())
        return elems[key];
    return NULL;
}
nasal_val* nasal_hash::get_value_address(std::string key)
{
    nasal_val* ret_value_addr=NULL;
    if(elems.find(key)!=elems.end())
        return elems[key];
    else if(elems.find("parents")!=elems.end())
    {
        nasal_val* val_addr=elems["parents"];
        if(val_addr->get_type()==vm_vec)
        {
            nasal_vec& vec_ref=val_addr->get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_val* tmp_val_addr=vec_ref.get_value_address(i);
                if(tmp_val_addr->get_type()==vm_hash)
                    ret_value_addr=tmp_val_addr->get_hash().get_value_address(key);
                if(ret_value_addr>=0)
                    break;
            }
        }
    }
    return ret_value_addr;
}
nasal_val** nasal_hash::get_mem_address(std::string key)
{
    nasal_val** mem_addr=NULL;
    if(elems.find(key)!=elems.end())
        return &elems[key];
    else if(elems.find("parents")!=elems.end())
    {
        nasal_val* val_addr=elems["parents"];
        if(val_addr->get_type()==vm_vec)
        {
            nasal_vec& vec_ref=val_addr->get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_val* tmp_val_addr=vec_ref.get_value_address(i);
                if(tmp_val_addr->get_type()==vm_hash)
                    mem_addr=tmp_val_addr->get_hash().get_mem_address(key);
                if(mem_addr>0)
                    break;
            }
        }
    }
    return mem_addr;
}
bool nasal_hash::check_contain(std::string key)
{
    if(elems.find(key)!=elems.end())
        return true;
    if(elems.find("parents")!=elems.end())
    {
        bool result=false;
        nasal_val* val_addr=elems["parents"];
        if(val_addr->get_type()==vm_vec)
        {
            nasal_vec& vec_ref=val_addr->get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_val* tmp_val_addr=vec_ref.get_value_address(i);
                if(tmp_val_addr->get_type()==vm_hash)
                    result=tmp_val_addr->get_hash().check_contain(key);
                if(result)
                    break;
            }
        }
        return result;
    }
    return false;
}
nasal_val* nasal_hash::get_keys()
{
    nasal_val* ret_addr=gc.gc_alloc(vm_vec);
    nasal_vec& ref_vec=ret_addr->get_vector();
    for(std::map<std::string,nasal_val*>::iterator iter=elems.begin();iter!=elems.end();++iter)
    {
        nasal_val* str_addr=gc.gc_alloc(vm_str);
        str_addr->set_string(iter->first);
        ref_vec.add_elem(str_addr);
    }
    return ret_addr;
}
void nasal_hash::print()
{
    std::cout<<"{";
    if(!elems.size())
        std::cout<<"}";
    for(std::map<std::string,nasal_val*>::iterator i=elems.begin();i!=elems.end();++i)
    {
        std::cout<<i->first<<":";
        nasal_val* tmp=i->second;
        switch(tmp->get_type())
        {
            case vm_nil:      std::cout<<"nil";             break;
            case vm_num:   std::cout<<tmp->get_number(); break;
            case vm_str:   std::cout<<tmp->get_string(); break;
            case vm_vec:   tmp->get_vector().print();    break;
            case vm_hash:     tmp->get_hash().print();      break;
            case vm_func: std::cout<<"func(...){...}";  break;
        }
        std::cout<<",}"[(++i)==elems.end()];
        --i;
    }
    return;
}

/*functions of nasal_func*/
nasal_func::nasal_func(nasal_gc& ngc):gc(ngc)
{
    closure_addr=NULL;
    dynpara=-1;
    return;
}
nasal_func::~nasal_func()
{
    if(closure_addr)
        gc.del_reference(closure_addr);
    for(int i=0;i<default_para_addr.size();++i)
        if(default_para_addr[i])
            gc.del_reference(default_para_addr[i]);
    return;
}
void nasal_func::set_entry(int etr)
{
    entry=etr;
    return;
}
int nasal_func::get_entry()
{
    return entry;
}
void nasal_func::add_para(int name_index,nasal_val* val_addr=NULL,bool is_dynamic=false)
{
    if(is_dynamic)
    {
        dynpara=name_index;
        return;
    }
    para.push_back(name_index);
    default_para_addr.push_back(val_addr);
    return;
}
std::vector<int>& nasal_func::get_para()
{
    return para;
}
int nasal_func::get_dynamic_para()
{
    return dynpara;
}
std::vector<nasal_val*>& nasal_func::get_default()
{
    return default_para_addr;
}
void nasal_func::set_closure_addr(nasal_val* value_address)
{
    nasal_val* new_closure=gc.gc_alloc(vm_scop);
    new_closure->get_closure().set_closure(value_address->get_closure());
    closure_addr=new_closure;
    return;
}
void nasal_func::set_new_closure()
{
    closure_addr=gc.gc_alloc(vm_scop);
    return;
}
nasal_val* nasal_func::get_closure_addr()
{
    return closure_addr;
}

/*functions of nasal_scop*/
nasal_scop::nasal_scop(nasal_gc& ngc):gc(ngc)
{
    std::map<int,nasal_val*> new_scope;
    elems.push_back(new_scope);
    return;
}
nasal_scop::~nasal_scop()
{
    for(std::list<std::map<int,nasal_val*> >::iterator i=elems.begin();i!=elems.end();++i)
        for(std::map<int,nasal_val*>::iterator j=i->begin();j!=i->end();++j)
            gc.del_reference(j->second);
    elems.clear();
    return;
}
void nasal_scop::add_scope()
{
    std::map<int,nasal_val*> new_scope;
    elems.push_front(new_scope);
    return;
}
void nasal_scop::del_scope()
{
    std::map<int,nasal_val*>& last_scope=elems.front();
    for(std::map<int,nasal_val*>::iterator i=last_scope.begin();i!=last_scope.end();++i)
        gc.del_reference(i->second);
    elems.pop_front();
    return;
}
void nasal_scop::add_new_value(int key,nasal_val* value_address)
{
    if(elems.front().find(key)!=elems.front().end())
    {
        // if this value already exists,delete the old value and update a new value
        nasal_val* old_val_address=elems.front()[key];
        gc.del_reference(old_val_address);
    }
    elems.front()[key]=value_address;
    return;
}
nasal_val* nasal_scop::get_value_address(int key)
{
    nasal_val* ret_address=NULL;
    for(std::list<std::map<int,nasal_val*> >::iterator i=elems.begin();i!=elems.end();++i)
        if(i->find(key)!=i->end())
            return (*i)[key];
    return ret_address;
}
nasal_val** nasal_scop::get_mem_address(int key)
{
    nasal_val** ret_address=NULL;
    for(std::list<std::map<int,nasal_val*> >::iterator i=elems.begin();i!=elems.end();++i)
        if(i->find(key)!=i->end())
            return &((*i)[key]);
    return ret_address;
}
void nasal_scop::set_closure(nasal_scop& tmp)
{
    for(std::list<std::map<int,nasal_val*> >::iterator i=elems.begin();i!=elems.end();++i)
        for(std::map<int,nasal_val*>::iterator j=i->begin();j!=i->end();++j)
            gc.del_reference(j->second);
    elems.clear();
    for(std::list<std::map<int,nasal_val*> >::iterator i=tmp.elems.begin();i!=tmp.elems.end();++i)
    {
        std::map<int,nasal_val*> new_scope;
        elems.push_back(new_scope);
        for(std::map<int,nasal_val*>::iterator j=i->begin();j!=i->end();++j)
        {
            nasal_val* value_addr=j->second;
            gc.add_reference(value_addr);
            elems.back()[j->first]=value_addr;
        }
    }
    return;
}

/*functions of nasal_val*/
nasal_val::nasal_val()
{
    ref_cnt=1;
    type=vm_nil;
    return;
}
nasal_val::nasal_val(int nasal_val_type,nasal_gc& nvm)
{
    ref_cnt=1;
    type=nasal_val_type;
    switch(nasal_val_type)
    {
        case vm_nil:      break;
        case vm_num:   ptr.num=0;                      break;
        case vm_str:   ptr.str=new std::string;        break;
        case vm_vec:   ptr.vec=new nasal_vec(nvm);     break;
        case vm_hash:     ptr.hash=new nasal_hash(nvm);   break;
        case vm_func: ptr.func=new nasal_func(nvm);   break;
        case vm_scop:  ptr.cls=new nasal_scop(nvm); break;
    }
    return;
}
nasal_val::~nasal_val()
{
    // must set type and scalar_ptr to default first
    // this operation will avoid SIGTRAP caused by circular reference
    // circular reference will cause using destructor repeatedly
    int tmp_type=type;
    type=vm_nil;
    switch(tmp_type)
    {
        case vm_nil:      break;
        case vm_num:   break;
        case vm_str:   delete ptr.str;  break;
        case vm_vec:   delete ptr.vec;  break;
        case vm_hash:     delete ptr.hash; break;
        case vm_func: delete ptr.func; break;
        case vm_scop:  delete ptr.cls;  break;
    }
    return;
}
void nasal_val::clear()
{
    // must set type and scalar_ptr to default first
    // this operation will avoid SIGTRAP caused by circular reference
    // circular reference will cause using destructor repeatedly
    int tmp_type=type;
    type=vm_nil;
    switch(tmp_type)
    {
        case vm_nil:      break;
        case vm_num:   break;
        case vm_str:   delete ptr.str;  break;
        case vm_vec:   delete ptr.vec;  break;
        case vm_hash:     delete ptr.hash; break;
        case vm_func: delete ptr.func; break;
        case vm_scop:  delete ptr.cls;  break;
    }
    return;
}
void nasal_val::set_type(int nasal_val_type,nasal_gc& nvm)
{
    type=nasal_val_type;
    switch(nasal_val_type)
    {
        case vm_nil:      break;
        case vm_num:   ptr.num=0;                      break;
        case vm_str:   ptr.str=new std::string;        break;
        case vm_vec:   ptr.vec=new nasal_vec(nvm);     break;
        case vm_hash:     ptr.hash=new nasal_hash(nvm);   break;
        case vm_func: ptr.func=new nasal_func(nvm);   break;
        case vm_scop:  ptr.cls=new nasal_scop(nvm); break;
    }
    return;
}
void nasal_val::set_number(double num)
{
    ptr.num=num;
    return;
}
void nasal_val::set_string(std::string str)
{
    *ptr.str=str;
    return;
}
int nasal_val::get_type()
{
    return type;
}
double nasal_val::to_number()
{
    switch(type)
    {
        case vm_nil:    return 0;
        case vm_num: return ptr.num;
        case vm_str: return trans_string_to_number(*ptr.str);
        default:        return std::nan("");
    }
    return 0;
}
double nasal_val::get_number()
{
    return ptr.num;
}
std::string nasal_val::to_string()
{
    switch(type)
    {
        case vm_num:return trans_number_to_string(ptr.num);
        case vm_str:return *ptr.str;
        default:return "";
    }
    return "";
}
std::string nasal_val::get_string()
{
    return *ptr.str;
}
nasal_vec& nasal_val::get_vector()
{
    return *ptr.vec;
}
nasal_hash& nasal_val::get_hash()
{
    return *ptr.hash;
}
nasal_func& nasal_val::get_func()
{
    return *ptr.func;
}
nasal_scop& nasal_val::get_closure()
{
    return *ptr.cls;
}

/*functions of nasal_gc*/
nasal_gc::~nasal_gc()
{
    int gc_mem_size=memory.size();
    for(int i=0;i<gc_mem_size;++i)
        memory[i]->clear();
    for(int i=0;i<gc_mem_size;++i)
        delete memory[i];
    while(!free_space.empty())
        free_space.pop();
    memory.clear();
    return;
}
void nasal_gc::debug()
{
    int gc_mem_size=memory.size();
    for(int i=0;i<gc_mem_size;++i)
        if(memory[i]->ref_cnt)
        {
            std::cout<<">> [debug] "<<i<<": "<<memory[i]->ref_cnt<<" ";
            switch(memory[i]->get_type())
            {
                case vm_nil:std::cout<<"nil";break;
                case vm_num:std::cout<<"number "<<memory[i]->get_number();break;
                case vm_str:std::cout<<"string "<<memory[i]->get_string();break;
                case vm_vec:std::cout<<"vector";break;
                case vm_hash:std::cout<<"hash";break;
                case vm_func:std::cout<<"function";break;
                case vm_scop:std::cout<<"closure";break;
            }
            std::cout<<"\n";
        }
    return;
}
void nasal_gc::clear()
{
    int gc_mem_size=memory.size();
    for(int i=0;i<gc_mem_size;++i)
        memory[i]->clear();
    for(int i=0;i<gc_mem_size;++i)
        delete memory[i];
    while(!free_space.empty())
        free_space.pop();
    memory.clear();
    return;
}
nasal_val* nasal_gc::gc_alloc(int val_type)
{
    if(free_space.empty())
    {
        nasal_val* new_unit=new nasal_val(val_type,*this);
        memory.push_back(new_unit);
        return new_unit;
    }
    nasal_val* ret=free_space.front();
    free_space.pop();
    ret->ref_cnt=1;
    ret->set_type(val_type,*this);
    return ret;
}
void nasal_gc::add_reference(nasal_val* value_address)
{
    ++value_address->ref_cnt;
    return;
}
void nasal_gc::del_reference(nasal_val* value_address)
{
    --value_address->ref_cnt;
    if(!value_address->ref_cnt)
    {
        value_address->clear();
        free_space.push(value_address);
    }
    return;
}

#endif