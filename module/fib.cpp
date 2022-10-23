#include <iostream>
#include "../nasal.h"

double fibonaci(double x){
    if(x<=2)
        return x;
    return fibonaci(x-1)+fibonaci(x-2);
}

var fib(var* args,usize size,gc* ngc){
    std::cout<<"[mod] this is the first test module of nasal\n";
    if(!size)
        return nas_err("fib","lack arguments");
    var num=args[0];
    if(num.type!=vm_num)
        return nas_err("extern_fib","\"num\" must be number");
    return {vm_num,fibonaci(num.tonum())};
}

var quick_fib(var* args,usize size,gc* ngc){
    std::cout<<"[mod] this is the first test module of nasal\n";
    if(!size)
        return nas_err("fib","lack arguments");
    var num=args[0];
    if(num.type!=vm_num)
        return nas_err("extern_quick_fib","\"num\" must be number");
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

extern "C" mod get(const char* n){
    string name=n;
    if(name=="fib")
        return fib;
    else if(name=="quick_fib")
        return quick_fib;
    return nullptr;
}