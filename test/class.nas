import("lib.nas");

var student=func(name,age)
{
    var val={
        name:name,
        age:age
    };
    return {
        print_info:func(){println(val.name,' ',val.age);},
        set_age:   func(age){val.age=age;},
        get_age:   func(){return val.age;},
        set_name:  func(name){val.name=name;},
        get_name:  func(){return val.name;}
    }
};
var s=student('valk',24);
s.print_info();
println(s.get_age(),' ',s.get_name());
s.set_age(18);
s.set_name('aluo');
s.print_info();
println(s.get_age(),' ',s.get_name());