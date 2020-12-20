#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

enum runtime_scalar_type
{
    vm_nil=0,
    vm_number,
    vm_string,
    vm_closure,
    vm_function,
    vm_vector,
    vm_hash
};
/*
nasal_number: basic type(double)
nasal_string: basic type(std::string)
nasal_vector: elems[i] -> address in memory -> value address in gc
nasal_hash:   elems[key] -> address in memory -> value address in gc
nasal_function: closure -> value address in gc(type: nasal_closure)
nasal_closure: std::list<std::map<std::string,int>> -> std::map<std::string,int> -> (int) -> address in memory -> value address in gc
*/

class nasal_virtual_machine;
class nasal_vector;
class nasal_hash;
class nasal_function;
class nasal_closure;
class nasal_scalar;

class nasal_vector
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    nasal_virtual_machine& vm;
    std::vector<nasal_scalar*> elems;
public:
    nasal_vector(nasal_virtual_machine&);
    ~nasal_vector();
    void add_elem(nasal_scalar*);
    nasal_scalar*  del_elem();
    int  size();
    nasal_scalar*  operator[](const int);
    nasal_scalar*  get_value_address(int);
    nasal_scalar** get_mem_address(int);
    void print();
};

class nasal_hash
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    nasal_virtual_machine& vm;
    std::map<std::string,nasal_scalar*> elems;
public:
    nasal_hash(nasal_virtual_machine&);
    ~nasal_hash();
    void add_elem(std::string,nasal_scalar*);
    void del_elem(std::string);
    int  size();
    nasal_scalar*  get_special_para(std::string);
    nasal_scalar*  get_value_address(std::string);
    nasal_scalar** get_mem_address(std::string);
    bool check_contain(std::string);
    nasal_scalar*  get_keys();
    void print();
};

class nasal_function
{
private:
    nasal_virtual_machine& vm;
    int entry;
    nasal_scalar* closure_addr;
    std::vector<int> para_name;
    int dynamic_para_name;
    std::vector<nasal_scalar*> default_para_addr;
public:
    nasal_function(nasal_virtual_machine&);
    ~nasal_function();
    void set_entry(int);
    int  get_entry();
    void add_para(int,nasal_scalar*,bool);
    std::vector<int>& get_para();
    int  get_dynamic_para();
    std::vector<nasal_scalar*>& get_default();
    void set_closure_addr(nasal_scalar*);
    void set_new_closure();
    nasal_scalar* get_closure_addr();
};

class nasal_closure
{
private:
    // int in std::map<std::string,int> points to the space in nasal_vm::memory_manager_memory
    // and this memory_manager_memory space stores an address to garbage_collector_memory
    // and this address points to an nasal_hash
    nasal_virtual_machine& vm;
    std::list<std::map<int,nasal_scalar*> > elems;
public:
    nasal_closure(nasal_virtual_machine&);
    ~nasal_closure();
    void add_scope();
    void del_scope();
    void add_new_value(int,nasal_scalar*);
    nasal_scalar*  get_value_address(int);
    nasal_scalar** get_mem_address(int);
    void set_closure(nasal_closure&);
};

class nasal_scalar
{
protected:
    int type;
    union 
    {
        double num;
        std::string* str;
        nasal_vector* vec;
        nasal_hash* hash;
        nasal_function* func;
        nasal_closure* cls;
    }ptr;
public:
    int ref_cnt;
    nasal_scalar();
    nasal_scalar(int,nasal_virtual_machine&);
    ~nasal_scalar();
    void clear();
    void set_type(int,nasal_virtual_machine&);
    void set_number(double);
    void set_string(std::string);
    int             get_type();
    double          to_number();
    double          get_number();
    std::string     to_string();
    std::string     get_string();
    nasal_vector&   get_vector();
    nasal_hash&     get_hash();
    nasal_function& get_func();
    nasal_closure&  get_closure();
};

class nasal_virtual_machine
{
private:
    nasal_scalar error_returned_value;
    std::queue<nasal_scalar*> garbage_collector_free_space;
    std::vector<nasal_scalar*> garbage_collector_memory;
public:
    ~nasal_virtual_machine();
    void clear();
    void debug();
    nasal_scalar* gc_alloc(int);
    void add_reference(nasal_scalar*);
    void del_reference(nasal_scalar*);
};

/*functions of nasal_vector*/
nasal_vector::nasal_vector(nasal_virtual_machine& nvm):vm(nvm)
{
    return;
}
nasal_vector::~nasal_vector()
{
    int size=elems.size();
    for(int i=0;i<size;++i)
        vm.del_reference(elems[i]);
    elems.clear();
    return;
}
void nasal_vector::add_elem(nasal_scalar* value_address)
{
    elems.push_back(value_address);
    return;
}
nasal_scalar* nasal_vector::del_elem()
{
    // pop back
    if(!elems.size())
        return NULL;
    nasal_scalar* ret=elems.back();
    elems.pop_back();
    return ret;
}
int nasal_vector::size()
{
    return elems.size();
}
nasal_scalar* nasal_vector::operator[](const int index)
{
    return elems[index];
}
nasal_scalar* nasal_vector::get_value_address(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
    {
        std::cout<<">> [runtime] nasal_vector::get_value_address: index out of range: "<<index<<"\n";
        return NULL;
    }
    return elems[(index+vec_size)%vec_size];
}
nasal_scalar** nasal_vector::get_mem_address(int index)
{
    int vec_size=elems.size();
    if(index<-vec_size || index>=vec_size)
    {
        std::cout<<">> [runtime] nasal_vector::get_mem_address: index out of range: "<<index<<"\n";
        return NULL;
    }
    return &elems[(index+vec_size)%vec_size];
}
void nasal_vector::print()
{
    int size=elems.size();
    std::cout<<"[";
    if(!size)
        std::cout<<"]";
    for(int i=0;i<size;++i)
    {
        nasal_scalar* tmp=elems[i];
        switch(tmp->get_type())
        {
            case vm_nil:      std::cout<<"nil";             break;
            case vm_number:   std::cout<<tmp->get_number(); break;
            case vm_string:   std::cout<<tmp->get_string(); break;
            case vm_vector:   tmp->get_vector().print();    break;
            case vm_hash:     tmp->get_hash().print();      break;
            case vm_function: std::cout<<"func(...){...}";  break;
        }
        std::cout<<",]"[i==size-1];
    }
    return;
}

/*functions of nasal_hash*/
nasal_hash::nasal_hash(nasal_virtual_machine& nvm):vm(nvm)
{
    return;
}
nasal_hash::~nasal_hash()
{
    for(std::map<std::string,nasal_scalar*>::iterator iter=elems.begin();iter!=elems.end();++iter)
        vm.del_reference(iter->second);
    elems.clear();
    return;
}
void nasal_hash::add_elem(std::string key,nasal_scalar* value_address)
{
    if(elems.find(key)==elems.end())
        elems[key]=value_address;
    return;
}
void nasal_hash::del_elem(std::string key)
{
    if(elems.find(key)!=elems.end())
    {
        vm.del_reference(elems[key]);
        elems.erase(key);
    }
    return;
}
int nasal_hash::size()
{
    return elems.size();
}
nasal_scalar* nasal_hash::get_special_para(std::string key)
{
    if(elems.find(key)!=elems.end())
        return elems[key];
    return NULL;
}
nasal_scalar* nasal_hash::get_value_address(std::string key)
{
    nasal_scalar* ret_value_addr=NULL;
    if(elems.find(key)!=elems.end())
        return elems[key];
    else if(elems.find("parents")!=elems.end())
    {
        nasal_scalar* val_addr=elems["parents"];
        if(val_addr->get_type()==vm_vector)
        {
            nasal_vector& vec_ref=val_addr->get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_scalar* tmp_val_addr=vec_ref.get_value_address(i);
                if(tmp_val_addr->get_type()==vm_hash)
                    ret_value_addr=tmp_val_addr->get_hash().get_value_address(key);
                if(ret_value_addr>=0)
                    break;
            }
        }
    }
    return ret_value_addr;
}
nasal_scalar** nasal_hash::get_mem_address(std::string key)
{
    nasal_scalar** mem_addr=NULL;
    if(elems.find(key)!=elems.end())
        return &elems[key];
    else if(elems.find("parents")!=elems.end())
    {
        nasal_scalar* val_addr=elems["parents"];
        if(val_addr->get_type()==vm_vector)
        {
            nasal_vector& vec_ref=val_addr->get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_scalar* tmp_val_addr=vec_ref.get_value_address(i);
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
        nasal_scalar* val_addr=elems["parents"];
        if(val_addr->get_type()==vm_vector)
        {
            nasal_vector& vec_ref=val_addr->get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                nasal_scalar* tmp_val_addr=vec_ref.get_value_address(i);
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
nasal_scalar* nasal_hash::get_keys()
{
    nasal_scalar* ret_addr=vm.gc_alloc(vm_vector);
    nasal_vector& ref_vec=ret_addr->get_vector();
    for(std::map<std::string,nasal_scalar*>::iterator iter=elems.begin();iter!=elems.end();++iter)
    {
        nasal_scalar* str_addr=vm.gc_alloc(vm_string);
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
    for(std::map<std::string,nasal_scalar*>::iterator i=elems.begin();i!=elems.end();++i)
    {
        std::cout<<i->first<<":";
        nasal_scalar* tmp=i->second;
        switch(tmp->get_type())
        {
            case vm_nil:      std::cout<<"nil";             break;
            case vm_number:   std::cout<<tmp->get_number(); break;
            case vm_string:   std::cout<<tmp->get_string(); break;
            case vm_vector:   tmp->get_vector().print();    break;
            case vm_hash:     tmp->get_hash().print();      break;
            case vm_function: std::cout<<"func(...){...}";  break;
        }
        std::cout<<",}"[(++i)==elems.end()];
        --i;
    }
    return;
}

/*functions of nasal_function*/
nasal_function::nasal_function(nasal_virtual_machine& nvm):vm(nvm)
{
    closure_addr=NULL;
    dynamic_para_name=-1;
    return;
}
nasal_function::~nasal_function()
{
    if(closure_addr)
        vm.del_reference(closure_addr);
    for(int i=0;i<default_para_addr.size();++i)
        if(default_para_addr[i])
            vm.del_reference(default_para_addr[i]);
    return;
}
void nasal_function::set_entry(int etr)
{
    entry=etr;
    return;
}
int nasal_function::get_entry()
{
    return entry;
}
void nasal_function::add_para(int name_index,nasal_scalar* val_addr=NULL,bool is_dynamic=false)
{
    if(is_dynamic)
    {
        dynamic_para_name=name_index;
        return;
    }
    para_name.push_back(name_index);
    default_para_addr.push_back(val_addr);
    return;
}
std::vector<int>& nasal_function::get_para()
{
    return para_name;
}
int nasal_function::get_dynamic_para()
{
    return dynamic_para_name;
}
std::vector<nasal_scalar*>& nasal_function::get_default()
{
    return default_para_addr;
}
void nasal_function::set_closure_addr(nasal_scalar* value_address)
{
    nasal_scalar* new_closure=vm.gc_alloc(vm_closure);
    new_closure->get_closure().set_closure(value_address->get_closure());
    closure_addr=new_closure;
    return;
}
void nasal_function::set_new_closure()
{
    closure_addr=vm.gc_alloc(vm_closure);
    return;
}
nasal_scalar* nasal_function::get_closure_addr()
{
    return closure_addr;
}

/*functions of nasal_closure*/
nasal_closure::nasal_closure(nasal_virtual_machine& nvm):vm(nvm)
{
    std::map<int,nasal_scalar*> new_scope;
    elems.push_back(new_scope);
    return;
}
nasal_closure::~nasal_closure()
{
    for(std::list<std::map<int,nasal_scalar*> >::iterator i=elems.begin();i!=elems.end();++i)
        for(std::map<int,nasal_scalar*>::iterator j=i->begin();j!=i->end();++j)
            vm.del_reference(j->second);
    elems.clear();
    return;
}
void nasal_closure::add_scope()
{
    std::map<int,nasal_scalar*> new_scope;
    elems.push_back(new_scope);
    return;
}
void nasal_closure::del_scope()
{
    std::map<int,nasal_scalar*>& last_scope=elems.back();
    for(std::map<int,nasal_scalar*>::iterator i=last_scope.begin();i!=last_scope.end();++i)
        vm.del_reference(i->second);
    elems.pop_back();
    return;
}
void nasal_closure::add_new_value(int key,nasal_scalar* value_address)
{
    if(elems.back().find(key)!=elems.back().end())
    {
        // if this value already exists,delete the old value and update a new value
        nasal_scalar* old_val_address=elems.back()[key];
        vm.del_reference(old_val_address);
    }
    elems.back()[key]=value_address;
    return;
}
nasal_scalar* nasal_closure::get_value_address(int key)
{
    nasal_scalar* ret_address=NULL;
    for(std::list<std::map<int,nasal_scalar*> >::iterator i=elems.begin();i!=elems.end();++i)
        if(i->find(key)!=i->end())
            ret_address=(*i)[key];
    return ret_address;
}
nasal_scalar** nasal_closure::get_mem_address(int key)
{
    nasal_scalar** ret_address=NULL;
    for(std::list<std::map<int,nasal_scalar*> >::iterator i=elems.begin();i!=elems.end();++i)
        if(i->find(key)!=i->end())
            ret_address=&((*i)[key]);
    return ret_address;
}
void nasal_closure::set_closure(nasal_closure& tmp)
{
    for(std::list<std::map<int,nasal_scalar*> >::iterator i=elems.begin();i!=elems.end();++i)
        for(std::map<int,nasal_scalar*>::iterator j=i->begin();j!=i->end();++j)
            vm.del_reference(j->second);
    elems.clear();
    for(std::list<std::map<int,nasal_scalar*> >::iterator i=tmp.elems.begin();i!=tmp.elems.end();++i)
    {
        std::map<int,nasal_scalar*> new_scope;
        elems.push_back(new_scope);
        for(std::map<int,nasal_scalar*>::iterator j=i->begin();j!=i->end();++j)
        {
            nasal_scalar* value_addr=j->second;
            vm.add_reference(value_addr);
            elems.back()[j->first]=value_addr;
        }
    }
    return;
}

/*functions of nasal_scalar*/
nasal_scalar::nasal_scalar()
{
    ref_cnt=1;
    type=vm_nil;
    return;
}
nasal_scalar::nasal_scalar(int nasal_scalar_type,nasal_virtual_machine& nvm)
{
    ref_cnt=1;
    type=nasal_scalar_type;
    switch(nasal_scalar_type)
    {
        case vm_nil:      break;
        case vm_number:   ptr.num=0;                        break;
        case vm_string:   ptr.str=new std::string;          break;
        case vm_vector:   ptr.vec=new nasal_vector(nvm);    break;
        case vm_hash:     ptr.hash=new nasal_hash(nvm);     break;
        case vm_function: ptr.func=new nasal_function(nvm); break;
        case vm_closure:  ptr.cls=new nasal_closure(nvm);   break;
    }
    return;
}
nasal_scalar::~nasal_scalar()
{
    // must set type and scalar_ptr to default first
    // this operation will avoid SIGTRAP caused by circular reference
    // circular reference will cause using destructor repeatedly
    int tmp_type=type;
    type=vm_nil;
    switch(tmp_type)
    {
        case vm_nil:      break;
        case vm_number:   break;
        case vm_string:   delete ptr.str;  break;
        case vm_vector:   delete ptr.vec;  break;
        case vm_hash:     delete ptr.hash; break;
        case vm_function: delete ptr.func; break;
        case vm_closure:  delete ptr.cls;  break;
    }
    return;
}
void nasal_scalar::clear()
{
    // must set type and scalar_ptr to default first
    // this operation will avoid SIGTRAP caused by circular reference
    // circular reference will cause using destructor repeatedly
    int tmp_type=type;
    type=vm_nil;
    switch(tmp_type)
    {
        case vm_nil:      break;
        case vm_number:   break;
        case vm_string:   delete ptr.str;  break;
        case vm_vector:   delete ptr.vec;  break;
        case vm_hash:     delete ptr.hash; break;
        case vm_function: delete ptr.func; break;
        case vm_closure:  delete ptr.cls;  break;
    }
    return;
}
void nasal_scalar::set_type(int nasal_scalar_type,nasal_virtual_machine& nvm)
{
    type=nasal_scalar_type;
    switch(nasal_scalar_type)
    {
        case vm_nil:      break;
        case vm_number:   ptr.num=0;                        break;
        case vm_string:   ptr.str=new std::string;          break;
        case vm_vector:   ptr.vec=new nasal_vector(nvm);    break;
        case vm_hash:     ptr.hash=new nasal_hash(nvm);     break;
        case vm_function: ptr.func=new nasal_function(nvm); break;
        case vm_closure:  ptr.cls=new nasal_closure(nvm);   break;
    }
    return;
}
void nasal_scalar::set_number(double num)
{
    ptr.num=num;
    return;
}
void nasal_scalar::set_string(std::string str)
{
    *ptr.str=str;
    return;
}
int nasal_scalar::get_type()
{
    return type;
}
double nasal_scalar::to_number()
{
    switch(type)
    {
        case vm_nil:return 0;
        case vm_number: return ptr.num;
        case vm_string: return trans_string_to_number(*ptr.str);
        default: return std::nan("");
    }
    return 0;
}
double nasal_scalar::get_number()
{
    return ptr.num;
}
std::string nasal_scalar::to_string()
{
    switch(type)
    {
        case vm_number:return trans_number_to_string(ptr.num);
        case vm_string:return *ptr.str;
        default:return "";
    }
    return "";
}
std::string nasal_scalar::get_string()
{
    return *ptr.str;
}
nasal_vector& nasal_scalar::get_vector()
{
    return *ptr.vec;
}
nasal_hash& nasal_scalar::get_hash()
{
    return *ptr.hash;
}
nasal_function& nasal_scalar::get_func()
{
    return *ptr.func;
}
nasal_closure& nasal_scalar::get_closure()
{
    return *ptr.cls;
}

/*functions of nasal_virtual_machine*/
nasal_virtual_machine::~nasal_virtual_machine()
{
    int gc_mem_size=garbage_collector_memory.size();
    for(int i=0;i<gc_mem_size;++i)
        garbage_collector_memory[i]->clear();
    for(int i=0;i<gc_mem_size;++i)
        delete garbage_collector_memory[i];
    while(!garbage_collector_free_space.empty())
        garbage_collector_free_space.pop();
    garbage_collector_memory.clear();
    return;
}
void nasal_virtual_machine::debug()
{
    int gc_mem_size=garbage_collector_memory.size();
    for(int i=0;i<gc_mem_size;++i)
        if(garbage_collector_memory[i]->ref_cnt)
        {
            std::cout<<">> [debug] "<<i<<": "<<garbage_collector_memory[i]->ref_cnt<<" ";
            switch(garbage_collector_memory[i]->get_type())
            {
                case vm_nil:std::cout<<"nil";break;
                case vm_number:std::cout<<"number "<<garbage_collector_memory[i]->get_number();break;
                case vm_string:std::cout<<"string "<<garbage_collector_memory[i]->get_string();break;
                case vm_vector:std::cout<<"vector";break;
                case vm_hash:std::cout<<"hash";break;
                case vm_function:std::cout<<"function";break;
                case vm_closure:std::cout<<"closure";break;
            }
            std::cout<<"\n";
        }
    return;
}
void nasal_virtual_machine::clear()
{
    int gc_mem_size=garbage_collector_memory.size();
    for(int i=0;i<gc_mem_size;++i)
        garbage_collector_memory[i]->clear();
    for(int i=0;i<gc_mem_size;++i)
        delete garbage_collector_memory[i];
    while(!garbage_collector_free_space.empty())
        garbage_collector_free_space.pop();
    garbage_collector_memory.clear();
    return;
}
nasal_scalar* nasal_virtual_machine::gc_alloc(int val_type)
{
    if(garbage_collector_free_space.empty())
    {
        nasal_scalar* new_unit=new nasal_scalar(val_type,*this);
        garbage_collector_memory.push_back(new_unit);
        return new_unit;
    }
    nasal_scalar* ret=garbage_collector_free_space.front();
    garbage_collector_free_space.pop();
    ret->ref_cnt=1;
    ret->set_type(val_type,*this);
    return ret;
}
void nasal_virtual_machine::add_reference(nasal_scalar* value_address)
{
    if(value_address)
        ++value_address->ref_cnt;
    return;
}
void nasal_virtual_machine::del_reference(nasal_scalar* value_address)
{
    if(value_address)
        --value_address->ref_cnt;
    else
        return;
    if(!value_address->ref_cnt)
    {
        value_address->clear();
        garbage_collector_free_space.push(value_address);
    }
    return;
}

#endif