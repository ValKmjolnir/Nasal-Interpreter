#ifndef __NASAL_GC_H__
#define __NASAL_GC_H__

#define GC_BLK_SIZE 128

class nasal_number;
class nasal_string;
class nasal_vector;
class nasal_hash;
class nasal_function;
class nasal_closure;

class nasal_scalar
{
private:
    int type;
    void* scalar_ptr;
public:
    nasal_scalar()
    {
        type=0;
        scalar_ptr=(void*)NULL;
        return;
    }
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
    free_space.pop();
    return ret;
}

int nasal_gc::add_ref(int mem_space)
{
    if(0<=mem_space && mem_space<memory.size()*GC_BLK_SIZE)
        ++memory[mem_space/GC_BLK_SIZE][mem_space%GC_BLK_SIZE].ref_cnt;
    else
    {
        std::cout<<">> [gc] add_ref:unexpected memory \'"<<mem_space<<"\'."<<std::endl;
        return 0;
    }
    return 1;
    
}

#endif