#ifndef __NASAL_VM_H__
#define __NASAL_VM_H__

#define MEM_BLK_SIZE 128

class nasal_vm
{
private:
    std::queue<int> free_space;
    std::vector<int*> memory;
public:
    nasal_vm();
    ~nasal_vm();
    int nas_alloc();
    int nas_free(int);
    int nas_store(int,int);
};

nasal_vm::nasal_vm()
{
    memory.clear();
    return;
}

nasal_vm::~nasal_vm()
{
    int size=memory.size();
    for(int i=0;i<size;++i)
        delete []memory[i];
    memory.clear();
    while(!free_space.empty())
        free_space.pop();
    return;
}

int nasal_vm::nas_alloc()
{
    if(free_space.empty())
    {
        int* new_block=new int[MEM_BLK_SIZE];
        memory.push_back(new_block);
        int mem_size=memory.size();
        for(int i=(mem_size-1)*MEM_BLK_SIZE;i<mem_size*MEM_BLK_SIZE;++i)
            free_space.push(i);
    }
    int ret=free_space.front();
    free_space.pop();
    return ret;
}

int nasal_vm::nas_free(int space_num)
{
    if(0<=space_num && space_num<memory.size()*MEM_BLK_SIZE)
        free_space.push(space_num);
    else
    {
        std::cout<<">> [vm] nas_free:unexpected memory \'"<<space_num<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}

int nasal_vm::nas_store(int mem_space,int value_space)
{
    // be careful! this process doesn't check if this mem_space is in use.
    if(0<=mem_space && mem_space<memory.size()*MEM_BLK_SIZE)
        memory[mem_space/MEM_BLK_SIZE][mem_space%MEM_BLK_SIZE]=value_space;
    else
    {
        std::cout<<">> [vm] nas_store:unexpected memory \'"<<mem_space<<"\'."<<std::endl;
        return 0;
    }
    return 1;
}

#endif