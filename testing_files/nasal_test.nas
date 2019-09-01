#//This is written for Nasal Intepreter
#//Sidi Liang

var w = 1;
var x = "hello";
var f = func(){
    print("f is called");
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

print(w);#//1
print("\n");
print(x);#//hello
print("\n");
print(y);#//Empty
print("\n");
print(z);#//Empty
print("\n");
print(z1);#//Empty
print("\n");
print(y2);#//Empty
print("\n");
print(y[0]);#//1
print("\n");
print(y1[2][1]);#//hello
print("\n");
print(z.numb);#//1
print("\n");
print(z.listt[2][1]);#//hello
print("\n");
print(z1.hashh.listt[2][1]);#//hello
print("\n");
print(y2[3].hashh.listt[2][1]);#//hello
print("\n");
print(f);#//Empty
print("\n");
f();#//f is called
print("\n");
print(z.funcc);#//Empty
print("\n");
z.funcc();#//f is called
print("\n");
print(z.funcccall);#//Empty
print("\n");
z.funcccall();#//f is called
print("\n");
z2.listt2[3].hashh.funcc();#//f is called
print("\n");
print(y1[f2()][w]);#//hello
print("\n");
#//print(z.f3()); Error
call(f);#//f is called
print("\n");
call(z.funcc);#//f is called
print("\n");

















