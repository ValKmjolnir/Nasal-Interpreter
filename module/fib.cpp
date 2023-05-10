// module for test

#include <iostream>
#include "../nasal.h"

namespace nasal_fib_module {

double fibonaci(double x) {
    if (x<=2) {
        return x;
    }
    return fibonaci(x-1)+fibonaci(x-2);
}

var fib(var* args, usize size, gc* ngc) {
    if (!size) {
        return nas_err("fib","lack arguments");
    }
    var num=args[0];
    return var::num(fibonaci(num.tonum()));
}

var quick_fib(var* args, usize size, gc* ngc) {
    if (!size) {
        return nas_err("quick_fib","lack arguments");
    }
    double num=args[0].tonum();
    if (num<2) {
        return var::num(num);
    }
    double a=1,b=1,res=0;
    for(double i=1;i<num;i+=1) {
        res=a+b;
        a=b;
        b=res;
    }
    return var::num(res);
}

u32 ghost_for_test;

void ghost_for_test_destructor(void* ptr) {
    std::cout<<"delete "<<ptr<<"\n";
    delete (u32*)ptr;
}

var create_new_ghost(var* args, usize size, gc* ngc) {
    var res=ngc->alloc(vm_obj);
    res.obj().set(ghost_for_test, new u32);
    return res;
}

var set_new_ghost(var* args, usize size, gc* ngc) {
    var res=args[0];
    if (!res.objchk(ghost_for_test)) {
        std::cout<<"set_new_ghost: not ghost for test type.\n";
        return nil;
    }
    std::cout<<"set_new_ghost: successfully set ghost.\n";
    f64 num=args[1].num();
    *((u32*)res.obj().ptr)=static_cast<u32>(num);
    return nil;
}

var print_new_ghost(var* args, usize size, gc* ngc) {
    var res=args[0];
    if (!res.objchk(ghost_for_test)) {
        std::cout<<"print_new_ghost: not ghost for test type.\n";
        return nil;
    }
    std::cout<<"print_new_ghost: result = "<<*((u32*)res.obj().ptr)<<"\n";
    return nil;
}

module_func_info func_tbl[]={
    {"fib", fib},
    {"quick_fib", quick_fib},
    {"create_ghost", create_new_ghost},
    {"set_ghost", set_new_ghost},
    {"print_ghost", print_new_ghost},
    {nullptr, nullptr}
};

}

extern "C" module_func_info* get(ghost_register_table* table) {
    nasal_fib_module::ghost_for_test=table->register_ghost_type(
        "fib_for_test",
        nasal_fib_module::ghost_for_test_destructor
    );
    return nasal_fib_module::func_tbl;
}