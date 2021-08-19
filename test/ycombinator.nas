# Y combinator by ValKmjolnir

import("lib.nas");

var fib=func(f){
    return f(f);
}(
    func(f){
        return func(x){
            if(x<2) return x;
            return f(f)(x-1)+f(f)(x-2);
        }
    }
);

for(var i=1;i<31;i+=1)
    println(fib(i));