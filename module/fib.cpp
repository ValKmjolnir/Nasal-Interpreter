#include "../nasal.h"

double fibonaci(double x){
    if(x<=2)
        return x;
    return fibonaci(x-1)+fibonaci(x-2);
}
extern "C" nasal_ref fib(std::vector<nasal_ref>& args,nasal_gc& gc){
    nasal_ref num=args[0];
    if(num.type!=vm_num)
        return builtin_err("extern_fib","\"num\" must be number");
    return {vm_num,fibonaci(num.to_number())};
}