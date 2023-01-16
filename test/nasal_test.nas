# This is written for Nasal Intepreter
# Sidi Liang
var w = 1;
var x = "hello";
var f = func(){
    println("f is called");
}
var f2 = func(){
    return 2;
}
var f3 = func(){
    return "numb";
}
var y = [w, x];
var y1 = [w, x, y, f];
var z = {
    numb:w,
    stri:x,
    listt:y1,
    funcc: f,
    funcccall: func f(),
};
var z1 = {
    numb:w,
    stri:x,
    listt:y,
    hashh:z
};
var y2 = [w, x, y, z1];
var z2 = {
    hashh: z1,
    listt2: y2,
};

println(w);                      #//1
println(x);                      #//hello
println(y);                      #//[1,hello]
println(z);                      #//{...}
println(z1);                     #//{...}
println(y2);                     #//[...]
println(y[0]);                   #//1
println(y1[2][1]);               #//hello
println(z.numb);                 #//1
println(z.listt[2][1]);          #//hello
println(z1.hashh.listt[2][1]);   #//hello
println(y2[3].hashh.listt[2][1]);#//hello
println(f);                      #//func(..){..}
f();                             #//f is called
println(z.funcc);                #//func(..){..}
z.funcc();                       #//f is called
println(z.funcccall);            #//func(..){..}
z2.listt2[3].hashh.funcc();      #//f is called
println(y1[f2()][w]);            #//hello
println(true,' ',false);         #//1 0


# ValKmjolnir
func(){
    var tm=maketimestamp();
    var duration=0;
    var f1=func(){}
    var f2=func(){var a=1;return a+1;}
    var f3=func(){var (a,b)=(1,1);return a+b+1;}
    tm.stamp();
    for(var i=0;i<1e6;i+=1);
    duration=tm.elapsedMSec();
    println(str(int(1e6/duration))," calc/ms");
    tm.stamp();
    for(var i=0;i<1e6;i+=1)f1();
    duration=tm.elapsedMSec();
    println(str(int(1e6/duration))," calc/ms");
    tm.stamp();
    for(var i=0;i<1e6;i+=1)func{}();
    duration=tm.elapsedMSec();
    println(str(int(1e6/duration))," calc/ms");
    tm.stamp();
    for(var i=0;i<1e6;i+=1)f2();
    duration=tm.elapsedMSec();
    println(str(int(1e6/duration))," calc/ms");
    tm.stamp();
    for(var i=0;i<1e6;i+=1)
        func{
            var a=1;
            return a+1;
        }();
    duration=tm.elapsedMSec();
    println(str(int(1e6/duration))," calc/ms");
    tm.stamp();
    for(var i=0;i<1e6;i+=1)f3();
    duration=tm.elapsedMSec();
    println(str(int(1e6/duration))," calc/ms");
    tm.stamp();
    for(var i=0;i<1e6;i+=1)
        func{
            var (a,b)=(1,1);
            return a+b+1;
        }();
    duration=tm.elapsedMSec();
    println(str(int(1e6/duration))," calc/ms");
}();