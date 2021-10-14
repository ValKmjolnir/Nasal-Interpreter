import("lib.nas");

var student=func(n,a){
    return {
        print_info:func println(n,' ',a),
        set_age:   func(age) a=age,
        get_age:   func return a,
        set_name:  func(name) n=name,
        get_name:  func return n
    };
}
var s=student('valk',24);
s.print_info();
println(s.get_age(),' ',s.get_name());
s.set_age(20);
s.set_name('aluo');
s.print_info();
println(s.get_age(),' ',s.get_name());
s.set_age(20);
s.set_name('Sidi Liang');
s.print_info();
println(s.get_age(),' ',s.get_name());
# flightgear nasal-console cannot use this kind of object initializing
var m=func(){
    var (_1,_2)=(0,1);
    return {
        a:func(){
            print(_1,' ',_2,'\n');
        },
        b:func(x){
            _1=x;
        },
        c:func(x){
            _2=x;
        },
        d:func(x){
            return func{
                print(_1,' ',_2,' ',x,'\n');
            };
        },
        g:func(x){
            var y=x;
            return func{
                print(_1,' ',_2,' ',x,' ',y,'\n');
            }
        },
        h:func(){
            return func(x){
                _1=x;
            }
        }
    };
}();

m.a(); # 0 1
m.b(2048);
m.c(1024);
var a=m.d(-1);
var b=m.g(1);
a(); # 2048 1024 -1
b(); # 2048 1024 1 1

m.h()(2147483647);
m.a();# 2147483647 1024