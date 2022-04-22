# Y combinator by ValKmjolnir
var fib=func(f){
    return f(f);
}(
    func(f){
        return func(x){
            if(x<2) return x;
            var tmp=f(f);
            return tmp(x-1)+tmp(x-2);
        }
    }
);

for(var i=1;i<31;i+=1)
    println(fib(i));
