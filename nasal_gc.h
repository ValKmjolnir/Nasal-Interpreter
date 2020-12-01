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

#define MEM_BLK_SIZE 256 // 0x00 ~ 0xff
#define GC_BLK_SIZE  256 // 0x00 ~ 0xff
/*
nasal_number: basic type(double)
nasal_string: basic type(std::string)
nasal_vector: elems[i] -> address in memory -> value address in gc
nasal_hash:   elems[key] -> address in memory -> value address in gc
nasal_function: closure -> value address in gc(type: nasal_closure)
nasal_closure: std::list<std::map<std::string,int>> -> std::map<std::string,int> -> (int) -> address in memory -> value address in gc
*/

class nasal_virtual_machine;

class nasal_vector
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    nasal_virtual_machine& vm;
    std::vector<int> elems;
public:
    nasal_vector(nasal_virtual_machine&);
    ~nasal_vector();
    void set_vm(nasal_virtual_machine&);
    void add_elem(int);
    int  del_elem();
    int  size();
    int  get_value_address(int);
    int  get_mem_address(int);
    void print();
};

class nasal_hash
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    nasal_virtual_machine& vm;
    std::map<std::string,int> elems;
public:
    nasal_hash(nasal_virtual_machine&);
    ~nasal_hash();
    void set_vm(nasal_virtual_machine&);
    void add_elem(std::string,int);
    void del_elem(std::string);
    int  size();
    int  get_value_address(std::string);
    int  get_mem_address(std::string);
    bool check_contain(std::string);
    int  get_keys();
    void print();
};

class nasal_function
{
private:
    // this int points to the space in nasal_vm::garbage_collector_memory
    nasal_virtual_machine& vm;
    int closure_addr;
    nasal_ast argument_list;
    nasal_ast function_expr;
public:
    nasal_function(nasal_virtual_machine&);
    ~nasal_function();
    void set_vm(nasal_virtual_machine&);
    void set_closure_addr(int);
    int  get_closure_addr();
    void set_arguments(nasal_ast&);
    nasal_ast& get_arguments();
    void set_run_block(nasal_ast&);
    nasal_ast& get_run_block();
};

class nasal_closure
{
private:
    // int in std::map<std::string,int> points to the space in nasal_vm::memory_manager_memory
    // and this memory_manager_memory space stores an address to garbage_collector_memory
    // and this address points to an nasal_hash
    nasal_virtual_machine& vm;
    std::list<std::map<std::string,int> > elems;
public:
    nasal_closure(nasal_virtual_machine&);
    ~nasal_closure();
    void set_vm(nasal_virtual_machine&);
    void add_scope();
    void del_scope();
    void add_new_value(std::string,int);
    int  get_value_address(std::string);
    int  get_mem_address(std::string);
    void set_closure(nasal_closure&);
};

class nasal_scalar
{
protected:
    int type;
    void* scalar_ptr;
public:
    nasal_scalar();
    ~nasal_scalar();
    void clear();
    void set_type(int,nasal_virtual_machine&);
    void set_number(double);
    void set_string(std::string);
    int             get_type();
    double          get_number();
    std::string     get_string();
    nasal_vector&   get_vector();
    nasal_hash&     get_hash();
    nasal_function& get_func();
    nasal_closure&  get_closure();
};

class nasal_virtual_machine
{
    struct gc_unit
    {
        bool collected;
        int ref_cnt;
        nasal_scalar elem;
        gc_unit()
        {
            collected=true;
            ref_cnt=0;
            return;
        }
    };
private:
    nasal_scalar error_returned_value;
    std::queue<int> garbage_collector_free_space;
    std::vector<gc_unit*> garbage_collector_memory;
    std::queue<int> memory_manager_free_space;
    std::vector<int> memory_manager_memory;
public:
    nasal_virtual_machine();
    ~nasal_virtual_machine();
    void clear();
    void debug();
    int  gc_alloc(int);          // garbage collector gives a new space
    nasal_scalar& gc_get(int);   // get scalar that stored in gc
    void add_reference(int);
    void del_reference(int);
    int  mem_alloc(int);         // memory gives a new space
    void mem_free(int);          // give space back to memory
    void mem_change(int,int);    // change value in memory space
    int  mem_get(int);           // get value in memory space
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
        vm.mem_free(elems[i]);
    elems.clear();
    return;
}
void nasal_vector::set_vm(nasal_virtual_machine& nvm)
{
    vm=nvm;
    return;
}
void nasal_vector::add_elem(int value_address)
{
    int memory_address=vm.mem_alloc(value_address);
    elems.push_back(memory_address);
    return;
}
int nasal_vector::del_elem()
{
    // pop back
    if(!elems.size())
        return -1;
    int ret=vm.mem_get(elems.back());
    vm.add_reference(ret);
    vm.mem_free(elems.back());
    elems.pop_back();
    return ret;
}
int nasal_vector::size()
{
    return elems.size();
}
int nasal_vector::get_value_address(int index)
{
    int vec_size=elems.size();
    int left_range=-vec_size;
    int right_range=vec_size-1;
    if(index<left_range || index>right_range)
    {
        std::cout<<">> [runtime] nasal_vector::get_value_address: index out of range: "<<index<<"\n";
        return -1;
    }
    return vm.mem_get(elems[(index+vec_size)%vec_size]);
}
int nasal_vector::get_mem_address(int index)
{
    int vec_size=elems.size();
    int left_range=-vec_size;
    int right_range=vec_size-1;
    if(index<left_range || index>right_range)
    {
        std::cout<<">> [runtime] nasal_vector::get_mem_address: index out of range: "<<index<<"\n";
        return -1;
    }
    return elems[(index+vec_size)%vec_size];
}
void nasal_vector::print()
{
    int size=elems.size();
    std::cout<<"[";
    if(!size)
        std::cout<<"]";
    for(int i=0;i<size;++i)
    {
        nasal_scalar& tmp=vm.gc_get(vm.mem_get(elems[i]));
        switch(tmp.get_type())
        {
            case vm_nil:std::cout<<"nil";break;
            case vm_number:std::cout<<tmp.get_number();break;
            case vm_string:std::cout<<tmp.get_string();break;
            case vm_vector:tmp.get_vector().print();break;
            case vm_hash:tmp.get_hash().print();break;
            case vm_function:std::cout<<"func(...){...}";break;
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
    for(std::map<std::string,int>::iterator iter=elems.begin();iter!=elems.end();++iter)
        vm.mem_free(iter->second);
    elems.clear();
    return;
}
void nasal_hash::set_vm(nasal_virtual_machine& nvm)
{
    vm=nvm;
    return;
}
void nasal_hash::add_elem(std::string key,int value_address)
{
    if(elems.find(key)==elems.end())
    {
        int memory_address=vm.mem_alloc(value_address);
        elems[key]=memory_address;
    }
    return;
}
void nasal_hash::del_elem(std::string key)
{
    if(elems.find(key)!=elems.end())
    {
        vm.mem_free(elems[key]);
        elems.erase(key);
    }
    return;
}
int nasal_hash::size()
{
    return elems.size();
}
int nasal_hash::get_value_address(std::string key)
{
    int ret_value_addr=-1;
    if(elems.find(key)!=elems.end())
        return vm.mem_get(elems[key]);
    else if(elems.find("parents")!=elems.end())
    {
        int mem_addr=elems["parents"];
        int val_addr=vm.mem_get(mem_addr);
        if(vm.gc_get(val_addr).get_type()==vm_vector)
        {
            nasal_vector& vec_ref=vm.gc_get(val_addr).get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                int tmp_val_addr=vec_ref.get_value_address(i);
                if(vm.gc_get(tmp_val_addr).get_type()==vm_hash)
                    ret_value_addr=vm.gc_get(tmp_val_addr).get_hash().get_value_address(key);
                if(ret_value_addr>=0)
                    break;
            }
        }
    }
    return ret_value_addr;
}
int nasal_hash::get_mem_address(std::string key)
{
    int ret_mem_addr=-1;
    if(elems.find(key)!=elems.end())
        return elems[key];
    else if(elems.find("parents")!=elems.end())
    {
        int mem_addr=elems["parents"];
        int val_addr=vm.mem_get(mem_addr);
        if(vm.gc_get(val_addr).get_type()==vm_vector)
        {
            nasal_vector& vec_ref=vm.gc_get(val_addr).get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                int tmp_val_addr=vec_ref.get_value_address(i);
                if(vm.gc_get(tmp_val_addr).get_type()==vm_hash)
                    ret_mem_addr=vm.gc_get(tmp_val_addr).get_hash().get_mem_address(key);
                if(ret_mem_addr>=0)
                    break;
            }
        }
    }
    return ret_mem_addr;
}
bool nasal_hash::check_contain(std::string key)
{
    if(elems.find(key)!=elems.end())
        return true;
    if(elems.find("parents")!=elems.end())
    {
        bool result=false;
        int mem_addr=elems["parents"];
        int val_addr=vm.mem_get(mem_addr);
        if(vm.gc_get(val_addr).get_type()==vm_vector)
        {
            nasal_vector& vec_ref=vm.gc_get(val_addr).get_vector();
            int size=vec_ref.size();
            for(int i=0;i<size;++i)
            {
                int tmp_val_addr=vec_ref.get_value_address(i);
                if(vm.gc_get(tmp_val_addr).get_type()==vm_hash)
                    result=vm.gc_get(tmp_val_addr).get_hash().check_contain(key);
                if(result)
                    break;
            }
        }
        return result;
    }
    return false;
}
int nasal_hash::get_keys()
{
    int ret_addr=vm.gc_alloc(vm_vector);
    nasal_vector& ref_vec=vm.gc_get(ret_addr).get_vector();
    for(std::map<std::string,int>::iterator iter=elems.begin();iter!=elems.end();++iter)
    {
        int str_addr=vm.gc_alloc(vm_string);
        vm.gc_get(str_addr).set_string(iter->first);
        ref_vec.add_elem(str_addr);
    }
    return ret_addr;
}
void nasal_hash::print()
{
    std::cout<<"{";
    if(!elems.size())
        std::cout<<"}";
    for(std::map<std::string,int>::iterator i=elems.begin();i!=elems.end();++i)
    {
        std::cout<<i->first<<":";
        nasal_scalar& tmp=vm.gc_get(vm.mem_get(i->second));
        switch(tmp.get_type())
        {
            case vm_nil:std::cout<<"nil";break;
            case vm_number:std::cout<<tmp.get_number();break;
            case vm_string:std::cout<<tmp.get_string();break;
            case vm_vector:tmp.get_vector().print();break;
            case vm_hash:tmp.get_hash().print();break;
            case vm_function:std::cout<<"func(...){...}";break;
        }
        std::cout<<",}"[(++i)==elems.end()];
        --i;
    }
    return;
}

/*functions of nasal_function*/
nasal_function::nasal_function(nasal_virtual_machine& nvm):vm(nvm)
{
    closure_addr=-1;
    argument_list.clear();
    function_expr.clear();
    return;
}
nasal_function::~nasal_function()
{
    if(closure_addr>=0)
        vm.del_reference(closure_addr);
    argument_list.clear();
    function_expr.clear();
    return;
}
void nasal_function::set_closure_addr(int value_address)
{
    if(closure_addr>=0)
        vm.del_reference(closure_addr);
    int new_closure=vm.gc_alloc(vm_closure);
    vm.gc_get(new_closure).get_closure().set_closure(vm.gc_get(value_address).get_closure());
    closure_addr=new_closure;
    return;
}
int nasal_function::get_closure_addr()
{
    return closure_addr;
}
void nasal_function::set_arguments(nasal_ast& node)
{
    argument_list=node;
    return;
}
nasal_ast& nasal_function::get_arguments()
{
    return argument_list;
}
void nasal_function::set_run_block(nasal_ast& node)
{
    function_expr=node;
    return;
}
nasal_ast& nasal_function::get_run_block()
{
    return function_expr;
}

/*functions of nasal_closure*/
nasal_closure::nasal_closure(nasal_virtual_machine& nvm):vm(nvm)
{
    return;
}
nasal_closure::~nasal_closure()
{
    for(std::list<std::map<std::string,int> >::iterator i=elems.begin();i!=elems.end();++i)
        for(std::map<std::string,int>::iterator j=i->begin();j!=i->end();++j)
            vm.mem_free(j->second);
    elems.clear();
    return;
}
void nasal_closure::add_scope()
{
    std::map<std::string,int> new_scope;
    elems.push_back(new_scope);
    return;
}
void nasal_closure::del_scope()
{
    if(this->elems.empty())
        return;
    for(std::map<std::string,int>::iterator i=elems.back().begin();i!=elems.back().end();++i)
        vm.mem_free(i->second);
    this->elems.pop_back();
    return;
}
void nasal_closure::add_new_value(std::string key,int value_address)
{
    int new_mem_address=vm.mem_alloc(value_address);
    if(elems.back().find(key)!=elems.back().end())
    {
        // if this value already exists,delete the old value and update a new value
        int old_mem_address=elems.back()[key];
        vm.mem_free(old_mem_address);
    }
    elems.back()[key]=new_mem_address;
    return;
}
int nasal_closure::get_value_address(std::string key)
{
    int ret_address=-1;
    for(std::list<std::map<std::string,int> >::iterator i=elems.begin();i!=elems.end();++i)
    {
        if(i->find(key)!=i->end())
            ret_address=vm.mem_get((*i)[key]);
    }
    return ret_address;
}
int nasal_closure::get_mem_address(std::string key)
{
    int ret_address=-1;
    for(std::list<std::map<std::string,int> >::iterator i=elems.begin();i!=elems.end();++i)
    {
        if(i->find(key)!=i->end())
            ret_address=(*i)[key];
    }
    return ret_address;
}
void nasal_closure::set_closure(nasal_closure& tmp)
{
    for(std::list<std::map<std::string,int> >::iterator i=elems.begin();i!=elems.end();++i)
        for(std::map<std::string,int>::iterator j=i->begin();j!=i->end();++j)
            vm.mem_free(j->second);
    elems.clear();
    for(std::list<std::map<std::string,int> >::iterator i=tmp.elems.begin();i!=tmp.elems.end();++i)
    {
        this->add_scope();
        for(std::map<std::string,int>::iterator j=i->begin();j!=i->end();++j)
        {
            int value_addr=vm.mem_get(j->second);
            int new_mem_addr=vm.mem_alloc(value_addr);
            vm.add_reference(value_addr);
            elems.back()[j->first]=new_mem_addr;
        }
    }
    return;
}

/*functions of nasal_scalar*/
nasal_scalar::nasal_scalar()
{
    this->type=vm_nil;
    this->scalar_ptr=(void*)NULL;
    return;
}
nasal_scalar::~nasal_scalar()
{
    // must set type and scalar_ptr to default first
    // this operation will avoid SIGTRAP caused by circular reference
    // circular reference will cause using destructor repeatedly
    int tmp_type=this->type;
    void* tmp_ptr=this->scalar_ptr;

    this->type=vm_nil;
    this->scalar_ptr=NULL;
    switch(tmp_type)
    {
        case vm_nil:      break;
        case vm_number:   delete (double*)(tmp_ptr);         break;
        case vm_string:   delete (std::string*)(tmp_ptr);    break;
        case vm_vector:   delete (nasal_vector*)(tmp_ptr);   break;
        case vm_hash:     delete (nasal_hash*)(tmp_ptr);     break;
        case vm_function: delete (nasal_function*)(tmp_ptr); break;
        case vm_closure:  delete (nasal_closure*)(tmp_ptr);  break;
    }
    return;
}
void nasal_scalar::clear()
{
    // must set type and scalar_ptr to default first
    // this operation will avoid SIGTRAP caused by circular reference
    // circular reference will cause using destructor repeatedly
    int tmp_type=this->type;
    void* tmp_ptr=this->scalar_ptr;

    this->type=vm_nil;
    this->scalar_ptr=NULL;
    switch(tmp_type)
    {
        case vm_nil:      break;
        case vm_number:   delete (double*)(tmp_ptr);         break;
        case vm_string:   delete (std::string*)(tmp_ptr);    break;
        case vm_vector:   delete (nasal_vector*)(tmp_ptr);   break;
        case vm_hash:     delete (nasal_hash*)(tmp_ptr);     break;
        case vm_function: delete (nasal_function*)(tmp_ptr); break;
        case vm_closure:  delete (nasal_closure*)(tmp_ptr);  break;
    }
    return;
}
void nasal_scalar::set_type(int nasal_scalar_type,nasal_virtual_machine& nvm)
{
    if(this->scalar_ptr)
    {
        std::cout<<">> [vm] scalar_ptr is in use: "<<type<<" "<<scalar_ptr<<"\n";
        return;
    }
    this->type=nasal_scalar_type;
    switch(nasal_scalar_type)
    {
        case vm_nil:      this->scalar_ptr=(void*)NULL;                      break;
        case vm_number:   this->scalar_ptr=(void*)(new double);              break;
        case vm_string:   this->scalar_ptr=(void*)(new std::string);         break;
        case vm_vector:   this->scalar_ptr=(void*)(new nasal_vector(nvm));   break;
        case vm_hash:     this->scalar_ptr=(void*)(new nasal_hash(nvm));     break;
        case vm_function: this->scalar_ptr=(void*)(new nasal_function(nvm)); break;
        case vm_closure:  this->scalar_ptr=(void*)(new nasal_closure(nvm));  break;
    }
    return;
}
void nasal_scalar::set_number(double num)
{
    *(double*)(this->scalar_ptr)=num;
    return;
}
void nasal_scalar::set_string(std::string str)
{
    *(std::string*)(this->scalar_ptr)=str;
    return;
}
int nasal_scalar::get_type()
{
    return this->type;
}
double nasal_scalar::get_number()
{
    return *(double*)(this->scalar_ptr);
}
std::string nasal_scalar::get_string()
{
    return *(std::string*)(this->scalar_ptr);
}
nasal_vector& nasal_scalar::get_vector()
{
    return *(nasal_vector*)(this->scalar_ptr);
}
nasal_hash& nasal_scalar::get_hash()
{
    return *(nasal_hash*)(this->scalar_ptr);
}
nasal_function& nasal_scalar::get_func()
{
    return *(nasal_function*)(this->scalar_ptr);
}
nasal_closure& nasal_scalar::get_closure()
{
    return *(nasal_closure*)(this->scalar_ptr);
}

/*functions of nasal_virtual_machine*/
nasal_virtual_machine::nasal_virtual_machine()
{
    return;
}
nasal_virtual_machine::~nasal_virtual_machine()
{
    int gc_mem_size=garbage_collector_memory.size();
    for(int i=0;i<gc_mem_size;++i)
        if(garbage_collector_memory[i]->ref_cnt)
        {
            garbage_collector_memory[i]->ref_cnt=0;
            garbage_collector_memory[i]->collected=true;
            garbage_collector_memory[i]->elem.clear();
        }
    for(int i=0;i<gc_mem_size;++i)
        delete garbage_collector_memory[i];
    while(!garbage_collector_free_space.empty())
        garbage_collector_free_space.pop();
    while(!memory_manager_free_space.empty())
        memory_manager_free_space.pop();
    garbage_collector_memory.clear();
    memory_manager_memory.clear();
    return;
}
void nasal_virtual_machine::debug()
{
    int gc_mem_size=garbage_collector_memory.size();
    for(int i=0;i<gc_mem_size;++i)
        if(garbage_collector_memory[i]->ref_cnt)
        {
            std::cout<<">> [debug] "<<i<<": "<<garbage_collector_memory[i]->ref_cnt<<" ";
            switch(garbage_collector_memory[i]->elem.get_type())
            {
                case vm_nil:std::cout<<"nil";break;
                case vm_number:std::cout<<"number "<<garbage_collector_memory[i]->elem.get_number();break;
                case vm_string:std::cout<<"string "<<garbage_collector_memory[i]->elem.get_string();break;
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
        if(garbage_collector_memory[i]->ref_cnt)
        {
            garbage_collector_memory[i]->ref_cnt=0;
            garbage_collector_memory[i]->collected=true;
            garbage_collector_memory[i]->elem.clear();
        }
    for(int i=0;i<gc_mem_size;++i)
        delete garbage_collector_memory[i];
    while(!garbage_collector_free_space.empty())
        garbage_collector_free_space.pop();
    while(!memory_manager_free_space.empty())
        memory_manager_free_space.pop();
    garbage_collector_memory.clear();
    memory_manager_memory.clear();
    return;
}
int nasal_virtual_machine::gc_alloc(int val_type)
{
    if(garbage_collector_free_space.empty())
    {
        int mem_size=garbage_collector_memory.size();
        garbage_collector_memory.resize(mem_size+64);
        for(int i=mem_size;i<mem_size+64;++i)
        {
            garbage_collector_memory[i]=new gc_unit;
            garbage_collector_free_space.push(i);
        }
    }
    int ret=garbage_collector_free_space.front();
    gc_unit& unit_ref=*garbage_collector_memory[ret];
    unit_ref.collected=false;
    unit_ref.ref_cnt=1;
    unit_ref.elem.set_type(val_type,*this);
    garbage_collector_free_space.pop();
    return ret;
}
nasal_scalar& nasal_virtual_machine::gc_get(int value_address)
{
    if(0<=value_address && value_address<garbage_collector_memory.size() && !garbage_collector_memory[value_address]->collected)
        return garbage_collector_memory[value_address]->elem;
    return error_returned_value;
}
void nasal_virtual_machine::add_reference(int value_address)
{
    if(0<=value_address && value_address<garbage_collector_memory.size() && !garbage_collector_memory[value_address]->collected)
        ++garbage_collector_memory[value_address]->ref_cnt;
    return;
}
void nasal_virtual_machine::del_reference(int value_address)
{
    if(0<=value_address && value_address<garbage_collector_memory.size() && !garbage_collector_memory[value_address]->collected)
        --garbage_collector_memory[value_address]->ref_cnt;
    else
        return;
    if(!garbage_collector_memory[value_address]->ref_cnt)
    {
        garbage_collector_memory[value_address]->collected=true;
        garbage_collector_memory[value_address]->elem.clear();
        garbage_collector_free_space.push(value_address);
    }
    return;
}
int nasal_virtual_machine::mem_alloc(int value_address)
{
    if(memory_manager_free_space.empty())
    {
        int mem_size=memory_manager_memory.size();
        memory_manager_memory.resize(mem_size+256);
        for(int i=mem_size;i<mem_size+256;++i)
            memory_manager_free_space.push(i);
    }
    int ret=memory_manager_free_space.front();
    memory_manager_memory[ret]=value_address;
    memory_manager_free_space.pop();
    return ret;
}
void nasal_virtual_machine::mem_free(int memory_address)
{
    // mem_free has helped scalar to delete the reference
    // so don't need to delete reference again
    if(0<=memory_address && memory_address<memory_manager_memory.size())
    {
        this->del_reference(memory_manager_memory[memory_address]);
        memory_manager_free_space.push(memory_address);
    }
    return;
}
void nasal_virtual_machine::mem_change(int memory_address,int value_address)
{
    // this progress is used to change a memory space's value address
    // be careful! this process doesn't check if this mem_space is in use.
    if(0<=memory_address && memory_address<memory_manager_memory.size())
    {
        this->del_reference(memory_manager_memory[memory_address]);
        memory_manager_memory[memory_address]=value_address;
    }
    return;
}
int nasal_virtual_machine::mem_get(int memory_address)
{
    // be careful! this process doesn't check if this mem_space is in use.
    if(0<=memory_address && memory_address<memory_manager_memory.size())
        return memory_manager_memory[memory_address];
    return -1;
}

#endif