#ifndef __NASAL_BUILTINFUNC_H__
#define __NASAL_BUILTINFUNC_H__

int append(std::list<std::map<std::string,int> >& local_scope)
{
    int vector_addr=-1,elements_addr=-1;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
    {
        if(i->find("vector")!=i->end())
            vector_addr=(*i)["vector"];
        if(i->find("elements")!=i->end())
            elements_addr=(*i)["elements"];
    }
    if(vector_addr<0 || elements_addr<0)
        return -1;
    if(nasal_gc.get_scalar(vector_addr).get_type()!=scalar_vector)
    {
        std::cout<<">> [Runtime] append gets a value that is not a vector."<<std::endl;
        return -1;
    }
    for(int i=0;i<nasal_gc.get_scalar(elements_addr).get_vector().get_size();++i)
    {
        int data_addr=nasal_gc.get_scalar(elements_addr).get_vector().get_elem(i);
        if(data_addr<0)
            return -1;
        int new_addr=-1;
        switch(nasal_gc.get_scalar(data_addr).get_type())
        {
            case scalar_nil:
                new_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                break;
            case scalar_number:
                new_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(data_addr).get_number());
                break;
            case scalar_string:
                new_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(data_addr).get_string());
                break;
            case scalar_function:
                new_addr=nasal_gc.gc_alloc();
                nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(data_addr).get_function());
                break;
            case scalar_vector:
            case scalar_hash:
                new_addr=data_addr;
                nasal_gc.reference_add(new_addr);
                break;
        }
        nasal_gc.get_scalar(vector_addr).get_vector().vec_push(new_addr);
    }
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_nil);
    return ret_addr;
}

int setsize(std::list<std::map<std::string,int> >& local_scope)
{
    int vector_addr=-1,size_addr=-1;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
    {
        if(i->find("vector")!=i->end())
            vector_addr=(*i)["vector"];
        if(i->find("__size")!=i->end())
            size_addr=(*i)["__size"];
    }   
    if(vector_addr<0 || size_addr<0)
        return -1;
    int vector_size=-1;
    int aim_size=-1;
    if(nasal_gc.get_scalar(vector_addr).get_type()!=scalar_vector)
    {
        std::cout<<">> [Runtime] setsize gets a variable that is not a vector."<<std::endl;
        return -1;
    }
    vector_size=nasal_gc.get_scalar(vector_addr).get_vector().get_size();
    if(nasal_gc.get_scalar(size_addr).get_type()==scalar_string && 
        !check_numerable_string(nasal_gc.get_scalar(size_addr).get_string().get_string()))
    {
        std::cout<<">> [Runtime] __size is not a numerable string."<<std::endl;
        return -1;
    }
    if(nasal_gc.get_scalar(size_addr).get_type()!=scalar_string &&
        nasal_gc.get_scalar(size_addr).get_type()!=scalar_number)
    {
        std::cout<<">> [Runtime] __size must be a number or numerable string."<<std::endl;
        return -1;
    }
    if(nasal_gc.get_scalar(size_addr).get_type()==scalar_string && 
        check_numerable_string(nasal_gc.get_scalar(size_addr).get_string().get_string()))
            aim_size=(int)trans_string_to_number(nasal_gc.get_scalar(size_addr).get_string().get_string());
    else if(nasal_gc.get_scalar(size_addr).get_type()==scalar_number)
        aim_size=(int)nasal_gc.get_scalar(size_addr).get_number().get_number();
    if(aim_size<0)
    {
        std::cout<<">> [Runtime] __size must be greater than 0."<<std::endl;
        return -1;
    }
    if(vector_size<aim_size)
    {
        for(int i=vector_size;i<aim_size;++i)
        {
            int new_addr=nasal_gc.gc_alloc();
            nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
            nasal_gc.get_scalar(vector_addr).get_vector().vec_push(new_addr);
        }
    }
    else if(vector_size>aim_size)
        for(int i=aim_size;i<vector_size;++i)
        {
            int pop_addr=nasal_gc.get_scalar(vector_addr).get_vector().vec_pop();
            nasal_gc.reference_delete(pop_addr);
        }
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_nil);
    return ret_addr;
}

int print(std::list<std::map<std::string,int> >& local_scope)
{
    int vector_addr=-1;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
        if(i->find("dyn")!=i->end())
            vector_addr=(*i)["dyn"];
    if(vector_addr<0)
        return -1;
    for(int i=0;i<nasal_gc.get_scalar(vector_addr).get_vector().get_size();++i)
    {
        int data_addr=nasal_gc.get_scalar(vector_addr).get_vector().get_elem(i);
        if(data_addr<0)
            return -1;
        switch(nasal_gc.get_scalar(data_addr).get_type())
        {
            case scalar_nil:break;
            case scalar_number:std::cout<<nasal_gc.get_scalar(data_addr).get_number().get_number();break;
            case scalar_string:std::cout<<nasal_gc.get_scalar(data_addr).get_string().get_string();break;
            case scalar_vector:std::cout<<"[...]";break;
            case scalar_hash:  std::cout<<"{...}";break;
            case scalar_function:std::cout<<"func(...){...}";break;
        }
    }
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_nil);
    return ret_addr;
}

int system_type(std::list<std::map<std::string,int> >& local_scope)
{
    int data=-1;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
        if(i->find("thing")!=i->end())
            data=(*i)["thing"];
    if(data<0)
        return -1;
    print_scalar_type(nasal_gc.get_scalar(data).get_type());
    std::cout<<std::endl;
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_nil);
    return ret_addr;
}

#endif