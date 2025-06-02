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
        return nas_err("quick_fib", "lack arguments");
    }
    double num = args[0].to_num();
    if (num<2) {
        return var::num(num);
    }
    double a = 1, b = 1, res = 0;
    for (double i = 1; i<num; ++i) {
        res = a+b;
        a = b;
        b = res;
    }
    return var::num(res);
}

const auto ghost_for_test = "ghost_for_test";

struct ghost_obj {
    u32 number = 0;
    var test_string = nil;
};

// if the dynamic library is closed, the pointer of this function will be unsafe
// make sure gc deletes the object before closing the dynamic library
// or just do not close the dynamic library...
void ghost_for_test_destructor(void* ptr) {
    std::cout << "ghost_for_test::destructor (0x";
    std::cout << std::hex << reinterpret_cast<u64>(ptr) << std::dec << ") {\n";

    delete static_cast<ghost_obj*>(ptr);

    std::cout << "    delete 0x" << std::hex;
    std::cout << reinterpret_cast<u64>(ptr) << std::dec << ";\n";
    std::cout << "}\n";
}

void ghost_for_test_gc_marker(void* ptr, std::vector<var>* bfs_queue) {
    std::cout << "ghost_for_test::mark (0x";
    std::cout << std::hex << reinterpret_cast<u64>(ptr) << std::dec << ") {\n";

    bfs_queue->push_back(static_cast<ghost_obj*>(ptr)->test_string);

    std::cout << "    mark 0x" << std::hex;
    std::cout << reinterpret_cast<u64>(ptr) << std::dec << "->test_string;\n";
    std::cout << "}\n";
}

var create_new_ghost(var* args, usize size, gc* ngc) {
    var res = ngc->alloc(vm_type::vm_ghost);
    res.ghost().set(
        ghost_for_test,
        ghost_for_test_destructor,
        ghost_for_test_gc_marker,
        new ghost_obj
    );
    return res;
}

var set_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    if (!res.object_check(ghost_for_test)) {
        std::cout << "set_new_ghost: not ghost for test type.\n";
        return nil;
    }
    f64 num = args[1].num();

    res.ghost().get<ghost_obj>()->number = static_cast<u32>(num);
    std::cout << "set_new_ghost: successfully set ghost.number = " << num << "\n";

    res.ghost().get<ghost_obj>()->test_string = ngc->newstr("just for test");
    std::cout << "set_new_ghost: successfully set ghost.test_string = just for test\n";
    return nil;
}

var print_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    if (!res.object_check(ghost_for_test)) {
        std::cout << "print_new_ghost: not ghost for test type.\n";
        return nil;
    }
    std::cout << "print_new_ghost: " << res.ghost() << " number = "
              << res.ghost().get<ghost_obj>()->number
              << " test_string = "
              << res.ghost().get<ghost_obj>()->test_string
              << "\n";
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

NASAL_EXPORT module_func_info* get() {
    return fib_module::func_tbl;
}

}