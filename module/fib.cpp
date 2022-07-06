#include <iostream>
#include "../nasal.h"

double fibonaci(double x){
    if(x<=2)
        return x;
    return fibonaci(x-1)+fibonaci(x-2);
}
extern "C" nasal_ref fib(std::vector<nasal_ref>& args,nasal_gc& gc){
    std::cout<<"[mod] this is the first test module of nasal\n";
    if(!args.size())
        return builtin_err("fib","lack arguments");
    nasal_ref num=args[0];
    if(num.type!=vm_num)
        return builtin_err("extern_fib","\"num\" must be number");
    return {vm_num,fibonaci(num.tonum())};
}
extern "C" nasal_ref quick_fib(std::vector<nasal_ref>& args,nasal_gc& gc){
    std::cout<<"[mod] this is the first test module of nasal\n";
    if(!args.size())
        return builtin_err("fib","lack arguments");
    nasal_ref num=args[0];
    if(num.type!=vm_num)
        return builtin_err("extern_quick_fib","\"num\" must be number");
    if(num.num()<2)
        return num;
    double a=1,b=1,res=0;
    for(double i=1;i<num.num();i+=1){
        res=a+b;
        a=b;
        b=res;
    }
    return {vm_num,res};
}