#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

#define GC_BLK_SIZE 128

class nasal_vector
{
private:
    // this int points to the space in nasal_vm_memory_manager
    std::vector<int> elems;
public:
    nasal_vector()
    {
        elems.clear();
        return;
    }
};
class nasal_hash
{
private:
    // this int points to the space in nasal_vm_memory_manager
    std::map<std::string,int> elems;
public:
    nasal_hash()
    {
        elems.clear();
        return;
    }
};
class nasal_function
{
private:
    // this int points to the space in nasal_vm_memory_manager
    std::list<int> closures;
    nasal_ast function_tree;
public:
    nasal_function()
    {
        closures.clear();
        function_tree.clear();
        return;
    }
};
class nasal_closure
{
private:
    std::map<std::string,int> elems;
public:
    nasal_closure()
    {
        elems.clear();
        return;
    }
};

class nasal_scalar
{
private:
    int type;
    void* scalar_ptr;
public:
    nasal_scalar()
    {
        this->type=vm_nil;
        this->scalar_ptr=(void*)NULL;
        return;
    }
    ~nasal_scalar()
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
    bool set_type(int nasal_scalar_type)
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
    // +-*/~
    // =
    // unary - !
};

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

class nasal_gc
{
private:
    std::queue<int> free_space;
    std::vector<gc_unit*> memory;
public:
    nasal_gc();
    ~nasal_gc();
    int gc_alloc();
    int add_ref(int);
    int del_ref(int);
};

nasal_gc::nasal_gc()
{
    memory.clear();
    return;
}

nasal_gc::~nasal_gc()
{
    int size=memory.size();
    for(int i=0;i<size;++i)
        delete []memory[i];
    while(!free_space.empty())
        free_space.pop();
    return;
}

int nasal_gc::gc_alloc()
{
    if(free_space.empty())
    {
        gc_unit* new_block=new gc_unit[GC_BLK_SIZE];
        memory.push_back(new_block);
        int mem_size=memory.size();
        for(int i=(mem_size-1)*GC_BLK_SIZE;i<mem_size*GC_BLK_SIZE;++i)
            free_space.push(i);
    }
    int ret=free_space.front();
    memory[ret/GC_BLK_SIZE][ret%GC_BLK_SIZE].collected=false;
    memory[ret/GC_BLK_SIZE][ret%GC_BLK_SIZE].ref_cnt=1;
    free_space.pop();
    return ret;
}

int nasal_gc::add_ref(int mem_space)
{
    int blk_num=mem_space/GC_BLK_SIZE;
    int blk_plc=mem_space%GC_BLK_SIZE;
    if(0<=mem_space && mem_space<memory.size()*GC_BLK_SIZE && !memory[blk_num][blk_plc].collected)
        ++memory[blk_num][blk_plc].ref_cnt;
    else
    {
        std::cout<<">> [gc] add_ref:unexpected memory \'"<<mem_space<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}

int nasal_gc::del_ref(int mem_space)
{
    int blk_num=mem_space/GC_BLK_SIZE;
    int blk_plc=mem_space%GC_BLK_SIZE;
    if(0<=mem_space && mem_space<memory.size()*GC_BLK_SIZE && !memory[blk_num][blk_plc].collected)
        --memory[blk_num][blk_plc].ref_cnt;
    else
    {
        std::cout<<">> [gc] del_ref:unexpected memory \'"<<mem_space<<"\'."<<std::endl;
        return 0;
    }
    if(!memory[blk_num][blk_plc].ref_cnt)
    {
        memory[blk_num][blk_plc].collected=true;
        free_space.push(mem_space);
    }
    return 1;
}

#endif