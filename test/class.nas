import("lib.nas");

var student=func(name,age)
{
    var (n,a)=(name,age);
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