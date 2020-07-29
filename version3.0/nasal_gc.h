#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

#define MEM_BLK_SIZE 256 // 0x00 ~ 0xff
#define GC_BLK_SIZE  256 // 0x00 ~ 0xff

class nasal_vector
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    std::vector<int> elems;
public:
    nasal_vector();
    ~nasal_vector();
    void add_elem(int);
    int  del_elem(int);
};
class nasal_hash
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    std::map<std::string,int> elems;
public:
    nasal_hash();
    ~nasal_hash();
    void add_elem(std::string,int);
    void del_elem(std::string);
};
class nasal_function
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    std::list<int> closures;
    nasal_ast function_tree;
public:
    nasal_function();
    ~nasal_function();
};
class nasal_closure
{
private:
    // this int points to the space in nasal_vm::memory_manager_memory
    std::map<std::string,int> elems;
public:
    nasal_closure();
    ~nasal_closure();
};

class nasal_scalar
{
private:
    int type;
    void* scalar_ptr;
public:
    nasal_scalar();
    ~nasal_scalar();
    void clear();
    bool set_type(int);
    // +-*/~
    // =
    // unary - !
};

class nasal_virtual_machine
{
    struct gc_unit
    {
        bool collected;
        char ref_cnt;
        nasal_scalar elem;
        gc_unit()
        {
            collected=true;
            ref_cnt=0;
            return;
        }
    };
private:
    std::queue<int> garbage_collector_free_space;
    std::vector<gc_unit*> garbage_collector_memory;
    std::queue<int> memory_manager_free_space;
    std::vector<int*> memory_manager_memory;
public:
    ~nasal_virtual_machine();
    int gc_alloc();
    int add_reference(int);
    int del_reference(int);
    int mem_alloc();
    int mem_free(int);
    int mem_store(int,int);
};

/*
    nasal runtime virtual machine
*/
nasal_virtual_machine nasal_vm;

/*functions of nasal_vector*/
nasal_vector::nasal_vector()
{
    elems.clear();
    return;
}
nasal_vector::~nasal_vector()
{
    int size=elems.size();
    for(int i=0;i<size;++i)
        nasal_vm.mem_free(elems[i]);
    elems.clear();
    return;
}
void nasal_vector::add_elem(int memory_address)
{
    elems.push_back(memory_address);
    return;
}
int nasal_vector::del_elem(int index)
{
    if(index>=elems.size())
        return -1;
    int ret=elems[index];
    nasal_vm.mem_free(ret);
    elems.pop_back();
    return ret;
}

/*functions of nasal_hash*/
nasal_hash::nasal_hash()
{
    elems.clear();
    return;
}
nasal_hash::~nasal_hash()
{
    for(std::map<std::string,int>::iterator iter=elems.begin();iter!=elems.end();++iter)
        nasal_vm.mem_free(iter->second);
    elems.clear();
    return;
}
void nasal_hash::add_elem(std::string key,int memory_address)
{
    elems[key]=memory_address;
    return;
}
void nasal_hash::del_elem(std::string key)
{
    nasal_vm.mem_free(elems[key]);
    elems.erase(key);
    return;
}

/*functions of nasal_function*/
nasal_function::nasal_function()
{
    closures.clear();
    function_tree.clear();
    return;
}
nasal_function::~nasal_function()
{
    for(std::list<int>::iterator iter=closures.begin();iter!=closures.end();++iter)
        nasal_vm.mem_free(*iter);
    closures.clear();
    function_tree.clear();
    return;
}

/*functions of nasal_closure*/
nasal_closure::nasal_closure()
{
    elems.clear();
    return;
}
nasal_closure::~nasal_closure()
{
    for(std::map<std::string,int>::iterator iter=elems.begin();iter!=elems.end();++iter)
        nasal_vm.mem_free(iter->second);
    elems.clear();
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
    switch(this->type)
    {
        case vm_nil:break;
        case vm_number:   delete (double*)(this->scalar_ptr);         break;
        case vm_string:   delete (std::string*)(this->scalar_ptr);    break;
        case vm_vector:   delete (nasal_vector*)(this->scalar_ptr);   break;
        case vm_hash:     delete (nasal_hash*)(this->scalar_ptr);     break;
        case vm_function: delete (nasal_function*)(this->scalar_ptr); break;
        case vm_closure:  delete (nasal_closure*)(this->scalar_ptr);  break;
    }
    return;
}
void nasal_scalar::clear()
{
    this->type=vm_nil;
    switch(this->type)
    {
        case vm_nil:break;
        case vm_number:   delete (double*)(this->scalar_ptr);         break;
        case vm_string:   delete (std::string*)(this->scalar_ptr);    break;
        case vm_vector:   delete (nasal_vector*)(this->scalar_ptr);   break;
        case vm_hash:     delete (nasal_hash*)(this->scalar_ptr);     break;
        case vm_function: delete (nasal_function*)(this->scalar_ptr); break;
        case vm_closure:  delete (nasal_closure*)(this->scalar_ptr);  break;
    }
    return;
}
bool nasal_scalar::set_type(int nasal_scalar_type)
{
    bool ret=true;
    this->type=nasal_scalar_type;
    switch(nasal_scalar_type)
    {
        case vm_nil:      this->scalar_ptr=(void*)NULL;                 break;
        case vm_number:   this->scalar_ptr=(void*)(new double);         break;
        case vm_string:   this->scalar_ptr=(void*)(new std::string);    break;
        case vm_vector:   this->scalar_ptr=(void*)(new nasal_vector);   break;
        case vm_hash:     this->scalar_ptr=(void*)(new nasal_hash);     break;
        case vm_function: this->scalar_ptr=(void*)(new nasal_function); break;
        case vm_closure:  this->scalar_ptr=(void*)(new nasal_closure);  break;
        default:
            std::cout<<">> [scalar] error scalar type: "<<nasal_scalar_type<<std::endl;
            this->type=vm_nil;
            this->scalar_ptr=(void*)NULL;
            ret=false;
            break;
    }
    return ret;
}

/*functions of nasal_virtual_machine*/
nasal_virtual_machine::~nasal_virtual_machine()
{
    int gc_mem_size=garbage_collector_memory.size();
    int mm_mem_size=memory_manager_memory.size();
    for(int i=0;i<gc_mem_size;++i)
        delete []garbage_collector_memory[i];
    for(int i=0;i<mm_mem_size;++i)
        delete []memory_manager_memory[i];
    garbage_collector_memory.clear();
    memory_manager_memory.clear();
    return;
}
int nasal_virtual_machine::gc_alloc()
{
    if(garbage_collector_free_space.empty())
    {
        gc_unit* new_block=new gc_unit[256];
        garbage_collector_memory.push_back(new_block);
        int mem_size=garbage_collector_memory.size();
        for(int i=((mem_size-1)<<8);i<(mem_size<<8);++i)
            garbage_collector_free_space.push(i);
    }
    int ret=garbage_collector_free_space.front();
    garbage_collector_memory[ret>>8][ret&0xff].collected=false;
    garbage_collector_memory[ret>>8][ret&0xff].ref_cnt=1;
    garbage_collector_free_space.pop();
    return ret;
}
int nasal_virtual_machine::add_reference(int memory_address)
{
    int blk_num=(memory_address>>8);
    int blk_plc=(memory_address&0xff);
    if(0<=memory_address && memory_address<(garbage_collector_memory.size()<<8) && !garbage_collector_memory[blk_num][blk_plc].collected)
        ++garbage_collector_memory[blk_num][blk_plc].ref_cnt;
    else
    {
        std::cout<<">> [vm] gc_add_ref:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}
int nasal_virtual_machine::del_reference(int memory_address)
{
    int blk_num=(memory_address>>8);
    int blk_plc=(memory_address&0xff);
    if(0<=memory_address && memory_address<(garbage_collector_memory.size()<<8) && !garbage_collector_memory[blk_num][blk_plc].collected)
        --garbage_collector_memory[blk_num][blk_plc].ref_cnt;
    else
    {
        std::cout<<">> [vm] gc_del_ref:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
        return 0;
    }
    if(!garbage_collector_memory[blk_num][blk_plc].ref_cnt)
    {
        garbage_collector_memory[blk_num][blk_plc].collected=true;
        garbage_collector_memory[blk_num][blk_plc].elem.clear();
        garbage_collector_free_space.push(memory_address);
    }
    return 1;
}
int nasal_virtual_machine::mem_alloc()
{
    if(memory_manager_free_space.empty())
    {
        int* new_block=new int[256];
        memory_manager_memory.push_back(new_block);
        int mem_size=memory_manager_memory.size();
        for(int i=((mem_size-1)<<8);i<(mem_size<<8);++i)
            memory_manager_free_space.push(i);
    }
    int ret=memory_manager_free_space.front();
    memory_manager_free_space.pop();
    return ret;
}
int nasal_virtual_machine::mem_free(int memory_address)
{
    // mem_free has helped scalar to delete the reference
    // so don't need to delete reference again
    if(0<=memory_address && memory_address<(memory_manager_memory.size()<<8))
    {
        this->del_reference(memory_manager_memory[memory_address>>8][memory_address&0xff]);
        memory_manager_free_space.push(memory_address);
    }
    else
    {
        std::cout<<">> [vm] mem_free:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}
int nasal_virtual_machine::mem_store(int memory_address,int value_space)
{
    // be careful! this process doesn't check if this mem_space is in use.
    if(0<=memory_address && memory_address<(memory_manager_memory.size()<<8))
        memory_manager_memory[memory_address>>8][memory_address&0xff]=value_space;
    else
    {
        std::cout<<">> [vm] mem_store:unexpected memory \'"<<memory_address<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}
#endif