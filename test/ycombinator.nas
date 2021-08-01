# Y combinator by ValKmjolnir

import("lib.nas");
var count=0;
var fib=func(f){
    return f(f);
}(
    func(f){
        return func(x){
            count+=1;
            if(x<2) return x;
            return f(f)(x-1)+f(f)(x-2);
        }
    }
);

for(var i=1;i<=20;i+=1)
    println(fib(i));