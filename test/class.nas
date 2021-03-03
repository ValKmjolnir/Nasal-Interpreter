import("lib.nas");

var student=
{
    new:func(name,age)
    {
        return {
            parents:[student],
            name:name,
            age:age
        };
    },
    print_info:func(){println(me.name,' ',me.age);},
    get_age:func(){return me.age;},
    get_name:func(){return me.name;}
};
var s=student.new('tiansuohaoer',24);
s.print_info();
println(s.get_age(),' ',s.get_name());