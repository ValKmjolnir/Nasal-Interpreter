// module for test

#include <iostream>
#include "../src/nasal.h"
#include "../src/nasal_type.h"
#include "../src/nasal_gc.h"

namespace nasal {
namespace fib_module {

double fibonaci(double x) {
    if (x<=2) {
        return x;
    }
    return fibonaci(x-1)+fibonaci(x-2);
}

var fib(var* args, usize size, gc* ngc) {
    if (!size) {
        return nas_err("fib", "lack arguments");
    }
    var num = args[0];
    return var::num(fibonaci(num.to_num()));
}

var quick_fib(var* args, usize size, gc* ngc) {
    if (!size) {
        return nas_err("quick_fib","lack arguments");
    }
    double num = args[0].to_num();
    if (num<2) {
        return var::num(num);
    }
    double a = 1, b = 1, res = 0;
    for(double i = 1; i<num; ++i) {
        res = a+b;
        a = b;
        b = res;
    }
    return var::num(res);
}

const auto ghost_for_test = "ghost_for_test";

void ghost_for_test_destructor(void* ptr) {
    std::cout << "ghost_for_test::destructor (0x";
    std::cout << std::hex << reinterpret_cast<u64>(ptr) << std::dec << ") {\n";
    delete static_cast<u32*>(ptr);
    std::cout << "    delete 0x" << std::hex;
    std::cout << reinterpret_cast<u64>(ptr) << std::dec << ";\n";
    std::cout << "}\n";
}

var create_new_ghost(var* args, usize size, gc* ngc) {
    var res = ngc->alloc(vm_obj);
    res.ghost().set(ghost_for_test, ghost_for_test_destructor, new u32);
    return res;
}

var set_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    if (!res.object_check(ghost_for_test)) {
        std::cout << "set_new_ghost: not ghost for test type.\n";
        return nil;
    }
    f64 num = args[1].num();
    *(reinterpret_cast<u32*>(res.ghost().pointer)) = static_cast<u32>(num);
    std::cout << "set_new_ghost: successfully set ghost = " << num << "\n";
    return nil;
}

var print_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    if (!res.object_check(ghost_for_test)) {
        std::cout << "print_new_ghost: not ghost for test type.\n";
        return nil;
    }
    std::cout << "print_new_ghost: " << res.ghost() << " result = "
              << *((u32*)res.ghost().pointer) << "\n";
    return nil;
}

module_func_info func_tbl[] = {
    {"fib", fib},
    {"quick_fib", quick_fib},
    {"create_ghost", create_new_ghost},
    {"set_ghost", set_new_ghost},
    {"print_ghost", print_new_ghost},
    {nullptr, nullptr}
};

}

extern "C" module_func_info* get() {
    return fib_module::func_tbl;
}

}