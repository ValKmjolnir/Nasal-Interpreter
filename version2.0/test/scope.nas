var global_value=0;
var global_hash=
{
    var1:1,
    var2:2,
    var3:func(){return me.var2;}
};
print(global_value);
print(global_hash.var3());

var func1=func()
{
    global_value=1;
    print(global_value);
    var closure_value=1;
    var temp_value=1;
    print(temp_value);
    return func{return closure_value;};
}

var func2=func()
{
    for(var temp_value=0;temp_value<100;temp_value+=1)
    {
        if(temp_value<10)
            print(temp_value,"< 10");
        elsif(10<=temp_value and temp_value<50)
            print(temp_value,"< 50");
        10=temp_value;
    }
    return;
}

var func3=func()
{
    var fake_closure_value=1;
    return func()
    {
        var fake_closure_value=2;
        return fake_closure_value;
    };
}

func1()();
func2();
func3()();

if(!global_value)
{
    var temp_value=1;
    if(temp_value)
    {
        var temp_value=2;
        if(temp_value>=1)
        {
            var temp_value=3;
            print(temp_value);
        }
        print(temp_value);
    }
    print(temp_value);
}