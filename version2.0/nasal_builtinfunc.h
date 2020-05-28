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

int subvec(std::list<std::map<std::string,int> >& local_scope)
{
    int vector_addr=-1,start=-1,length=-1,type=-1;
    int begin_iter=0,len_iter=0;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
    {
        if(i->find("vector")!=i->end())
            vector_addr=(*i)["vector"];
        if(i->find("start")!=i->end())
            start=(*i)["start"];
        if(i->find("length")!=i->end())
            length=(*i)["length"];
    }
    if(vector_addr<0 || start<0 || length<0)
        return -1;
    if(nasal_gc.get_scalar(vector_addr).get_type()!=scalar_vector)
    {
        std::cout<<">> subvec gets a value that is not a vector."<<std::endl;
        return -1;
    }
    type=nasal_gc.get_scalar(start).get_type();
    if(type!=scalar_number && type!=scalar_string)
    {
        std::cout<<">> [Runtime] \'start\' is not a number or numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_string && !check_numerable_string(nasal_gc.get_scalar(start).get_string().get_string()))
    {
        std::cout<<">> [Runtime] \'start\' is not a numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_number)
        begin_iter=(int)nasal_gc.get_scalar(start).get_number().get_number();
    else if(type==scalar_string)
        begin_iter=(int)trans_string_to_number(nasal_gc.get_scalar(start).get_string().get_string());
    type=nasal_gc.get_scalar(length).get_type();
    if(type!=scalar_nil && type!=scalar_number && type!=scalar_string)
    {
        std::cout<<">> [Runtime] \'length\' is not a nil type,number or numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_string && !check_numerable_string(nasal_gc.get_scalar(length).get_string().get_string()))
    {
        std::cout<<">> [Runtime] \'length\' is not a numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_nil)
        len_iter=nasal_gc.get_scalar(vector_addr).get_vector().get_size()-begin_iter;
    else if(type==scalar_number)
        len_iter=(int)nasal_gc.get_scalar(length).get_number().get_number();
    else if(type==scalar_string)
        len_iter=(int)trans_string_to_number(nasal_gc.get_scalar(length).get_string().get_string());
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_vector);
    if(type==scalar_nil)
        for(int i=begin_iter;i<nasal_gc.get_scalar(vector_addr).get_vector().get_size();++i)
        {
            int tmp=nasal_gc.get_scalar(vector_addr).get_vector().get_elem(i);
            if(tmp<0)
            {
                std::cout<<">> [Runtime] subvec out of memory boundary."<<std::endl;
                return -1;
            }
            int new_addr=-1;
            switch(nasal_gc.get_scalar(tmp).get_type())
            {
                case scalar_nil:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                    break;
                case scalar_number:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                    nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp).get_number());
                    break;
                case scalar_string:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                    nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp).get_string());
                    break;
                case scalar_function:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                    nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp).get_function());
                    break;
                case scalar_vector:
                case scalar_hash:
                    new_addr=tmp;
                    nasal_gc.reference_add(new_addr);
                    break;
            }
            nasal_gc.get_scalar(ret_addr).get_vector().vec_push(new_addr);
        }
    else
        for(int i=0;i<len_iter;++i)
        {
            int tmp=nasal_gc.get_scalar(vector_addr).get_vector().get_elem(begin_iter+i);
            if(tmp<0)
            {
                std::cout<<">> [Runtime] subvec out of memory boundary."<<std::endl;
                return -1;
            }
            int new_addr=-1;
            switch(nasal_gc.get_scalar(tmp).get_type())
            {
                case scalar_nil:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_nil);
                    break;
                case scalar_number:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_number);
                    nasal_gc.get_scalar(new_addr).get_number().deep_copy(nasal_gc.get_scalar(tmp).get_number());
                    break;
                case scalar_string:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_string);
                    nasal_gc.get_scalar(new_addr).get_string().deep_copy(nasal_gc.get_scalar(tmp).get_string());
                    break;
                case scalar_function:
                    new_addr=nasal_gc.gc_alloc();
                    nasal_gc.get_scalar(new_addr).set_type(scalar_function);
                    nasal_gc.get_scalar(new_addr).get_function().deep_copy(nasal_gc.get_scalar(tmp).get_function());
                    break;
                case scalar_vector:
                case scalar_hash:
                    new_addr=tmp;
                    nasal_gc.reference_add(new_addr);
                    break;
            }
            nasal_gc.get_scalar(ret_addr).get_vector().vec_push(new_addr);
        }
    return ret_addr;
}

int print(std::list<std::map<std::string,int> >& local_scope)
{
    int vector_addr=-1;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
        if(i->find("dyn")!=i->end())
            vector_addr=(*i)["dyn"];
    if(vector_addr<0) return -1;
    for(int i=0;i<nasal_gc.get_scalar(vector_addr).get_vector().get_size();++i)
    {
        int data_addr=nasal_gc.get_scalar(vector_addr).get_vector().get_elem(i);
        if(data_addr<0) return -1;
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
    if(data<0) return -1;
    print_scalar_type(nasal_gc.get_scalar(data).get_type());
    std::cout<<std::endl;
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_nil);
    return ret_addr;
}

int nas_trigfunc(std::list<std::map<std::string,int> >& local_scope,int func_type)
{
    int data=-1;
    double num=0;
    int type=-1;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
        if(i->find("x")!=i->end())
            data=(*i)["x"];
    if(data<0) return -1;
    type=nasal_gc.get_scalar(data).get_type();
    if(type!=scalar_number && type!=scalar_string)
    {
        std::cout<<">> [Runtime] x must be a number or numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_string && !check_numerable_string(nasal_gc.get_scalar(data).get_string().get_string()))
    {
        std::cout<<">> [Runtime] x is not a numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_number)
        num=nasal_gc.get_scalar(data).get_number().get_number();
    else if(type==scalar_string)
        num=trans_string_to_number(nasal_gc.get_scalar(data).get_string().get_string());
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
    switch(func_type)
    {
        case 0:nasal_gc.get_scalar(ret_addr).get_number().set_number(std::sin(num));break;
        case 1:nasal_gc.get_scalar(ret_addr).get_number().set_number(std::cos(num));break;
        case 2:nasal_gc.get_scalar(ret_addr).get_number().set_number(std::tan(num));break;
    }
    return ret_addr;
}

int nas_exp(std::list<std::map<std::string,int> >& local_scope)
{
    int data=-1,type=-1;
    double num=0;
    for(std::list<std::map<std::string,int> >::iterator i=local_scope.begin();i!=local_scope.end();++i)
        if(i->find("x")!=i->end())
            data=(*i)["x"];
    if(data<0)
        return -1;
    type=nasal_gc.get_scalar(data).get_type();
    if(type!=scalar_number && type!=scalar_string)
    {
        std::cout<<">> [Runtime] x must be a number or numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_string && !check_numerable_string(nasal_gc.get_scalar(data).get_string().get_string()))
    {
        std::cout<<">> [Runtime] x is not a numerable string."<<std::endl;
        return -1;
    }
    if(type==scalar_number)
        num=nasal_gc.get_scalar(data).get_number().get_number();
    else if(type==scalar_string)
        num=trans_string_to_number(nasal_gc.get_scalar(data).get_string().get_string());
    int ret_addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(ret_addr).set_type(scalar_number);
    nasal_gc.get_scalar(ret_addr).get_number().set_number(std::exp(num));
    return ret_addr;
}
#endif