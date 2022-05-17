var is_prime=func(x){
    for(var i=2;i<x;i+=1)
        if(x/i==int(x/i))
            return 0;
    return 1;
}

var is_prime_sqrt=func(x){
    for(var i=2;i<=math.sqrt(x);i+=1)
        if(x/i==int(x/i))
            return 0;
    return 1;
}

var primes=[];
var filter=func(x){
    foreach(var i;primes){
        if(x/i==int(x/i))
            return 0;
        if(x>=i){
            for(var j=i+1;j<=math.sqrt(x);j+=1)
                if(x/j==int(x/j))
                    return 0;
            append(primes,x);
            return 1;
        }
    }
    append(primes,x);
    return 1;
}

func(){
    var cnt=0;
    for(var i=2;i<50000;i+=1)
        if(filter(i))
            cnt+=1;
    println(cnt);
}();
