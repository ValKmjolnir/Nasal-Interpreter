#include <iostream>
#include "../nasal.h"

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

mod_func func_tbl[]={
    {"fib",fib},
    {"quick_fib",quick_fib},
    {nullptr, nullptr},
};

extern "C" mod_func* get() {
    return func_tbl;
}