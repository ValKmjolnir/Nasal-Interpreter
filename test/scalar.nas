# basic type
nil;
2147483647;
0x7fffffff;
0xdeadbeef;
0o70120327;
"hello world!";
'hello world!';
-12;
!0;
-((30));
[];
{};
[0,1,2,3,4,5][2];       # 2
[0,1,2,3,4,5,6,7,8,9,10][0,2,4:7,9];
[0,1,2,3,4,5,6,7,8,9,10,][0,2,4:7,9];
([0,1,2,3,4])[2];       # 2
(([0,1,2,3]))[2];       # 2
[0,1,2,3,4,5][5,4,3,2+1][0:2][0]; # 5
{str:"hello"}.str;      # "hello"
{str:"hello"}["str"];   # "hello"
{"str":"hello\"\"\n"}["str"]; # "hello"
20? 1:0;

# normal scalar
var number_1=1;
var number_2=0xdeadbeef;
var number_3=0x13702;
var number_4=0.12341490239423;
var string_1="hello";
var string_2='hello';
var string_3=number_1? 'yes':'no'; # yes

# vector
var vector_1=[];
var vector_2=[0,1,2,"str",number_1,vector_1];
var vector_3=vector_2[-3,-1];
var vector_4=vector_2[0:3];
var vector_5=vector_2[3:];

# hash
var hash_1={};
var hash_2={str1:'str1', str2:'str2', num1:0x7fffffff};
var hash_3={
 "member_1":1,
 "member_2":2,
 "member_3":3,
};
var hash_4={
 mem_1:hash_1,
 mem_2:hash_2.num1, # also this can be written as hash_2["num1"]
 mem_3:hash_3["member_1"]
};

# function
var func_1=func(){return 1;}
var prt=func(x){println(x);return nil;}
var func_with_dynamic_id=func(a,b,c,d...){return [a,b,c,d];}
var func_with_lack_para=func(a,b,c=1,d=2){return a+b+c+d;}
var func_with_func_para=func(a,f){return f(a);}

func_with_lack_para(a:1, b:2, c:3, d:4);
func_with_lack_para(b:1, c:3, a:4, d:1);
func_with_func_para(f:func prt,a:1);
func_with_func_para(func func_1(),func(x){return x;});
func_with_func_para(func_1(),func_1);
prt(func func_1());
var test_func_ret_number_1=func func_1(); # 1
var test_func_ret_number_2=func_1();      # 1

var source={
 member_1: func func_1(),         # this will get a number
 member_2: func {return 2.71828;} # this will get a function
};
println(source['member_2']());
println(source.member_2());

var test_func=func{return 1;}
println(func test_func());       # 1
println(test_func());            # 1
println(func test_func);         # nothing
println(test_func);              # nothing
println(([0,1,2,3])[1]);         # 1
println(({str:"what?"})["str"]); # what?
println(({str:"what?"}).str);    # what?

# lambda
(func(x){return x>0? x:0;})(12);
(func{println("hello world");})();
(((func(x){return 1.0/math.exp(x);})))(0);

# flexible definition & assignment
var (r,g,b)=[0x00,0x10,0xff];
(var r,g,b)=[0x00,0x10,0xff];
var color=[0x00,0x10,0xff];
var (r,g,b)=color;
(var r,g,b)=color;
(r,g,b)=(b,g,r);
(number_1,number_2)=(number_2,number_1);
var (swap_a,swap_b)=(0x1,0x80);
(swap_a,swap_b)=(swap_b,swap_a);
# ((swap_a),(swap_b))=(swap_b,swap_a) is wrong
# anything that use multi_assignment must not have curve around them
var multi_assign_1=[0,1,2,3,4];
var multi_assign_2=[10,9,8,7];
(multi_assign_1[1],multi_assign_2[0])=(multi_assign_1[2],multi_assign_2[1]);

# calculation
1+1;
1+1-2+3-4+5-6;
1+1*8-9/3;
1*-1;
1*(1+2*(3+4*(5+6*(7+8*(9+10/(1+1))))));
((-1*2+9))/7-1;
((({num:2})))["num"]*2*2*2;
((((([0,1,2])[0:2]))[0:2]))[1]-1;
println((((((((((((((((((((1+1+2+3+5)+8))+13)))+21))))+34)))))+55))))*89); # 12727
number_1*(number_2+number_3)/90-number_4;
(func test_func)()-1;
hash_3.member_3+(func {return {what:"i don't tell you.",case_small:80,case_large:100}})()["case_large"]/10;
-1*10+5 or 10-10;
nil and 1+7*8;
(number_1 or number_2) and (number_3 or number_4-number_4*1);
[0,1,4,3,2][4]*2-4+1*2*2*2*2*2/8;
{num:0}.num or {what_is_the_secret_of_universe:42}["what_is_the_secret_of_universe"];
"123"~"456"-123456*2/2;

var hash={str:'hello',f:func{return me.str;}};
var tmp_f=hash.f;
hash=1;
println(tmp_f());
# undefined symbol 'me'
# this means that 
# when generating local_scope for function f,
# nasal_gc will not count 'me' as one reference of this hash

var h1={str:'hello',f:func{return me.str;}};
var h2={str:'world',f:func{return nil;}};
h2.f=h1.f;
println(h2.f());
# print 'world'
# this means that 'me' in hash's functions
# only points to the hash this function belongs to

var f1=func(){println(1);return 1;}
var f2=func(){println(2);return 0;}
f1() or f2();
# print '1'
# this means that when using 'or' or 'and',
# if the result is clear when calculating,
# objects behind will not be calculated

print(
    subvec([0,1,2,3],2),'\n',
    subvec([0,1,2,3],2,1),'\n',
    abs(1),'\n',
    abs(-1),'\n',
    systime(),'\n',
    isfunc(func{}),' ',isfunc([]),'\n',
    ishash({}),' ',ishash([]),'\n',
    isint(114.514),' ',isint(114514),'\n',
    isnum("0xaa55"),' ',isnum("?"),'\n',
    isscalar(0.618),' ',isscalar("hello"),' ',isscalar([]),'\n',
    isstr("hello"),' ',isstr(func{}),'\n',
    isvec([]),' ',isvec("[]"),'\n',
    vecindex([0,1,2,3,4],1),'\n',
    vecindex(["apple","banana"],"apple")!=nil,'\n'
);

println(values({
    a:1,
    b:2,
    c:3
}));
println(find("cd", "abcdef")); # prints 2
println(find("x", "abcdef"));  # prints -1
println(find("cd", "abcdef")); # prints 2

var a={
    new: func(x=0){
        return {
            x:x,
            parents:[a]
        };
    },
    new2: func(x=0){
        return {
            x:x,
            parents:a
        };
    }
};
println(isa(a.new(),a)); # 1
println(isa(a.new2(),a));# 0

var a=[10,-10,0,1,2,3,nil,"string","hello",[],[0,1,2,3],{},{a:0,b:1,c:2},func{}];
println("type\tsize\tnum\tsrc");
foreach(var i;a){
    println(typeof(i),'\t',size(i),'\t',num(i),'\t',i);
}
foreach(i;a){
    ;
}
println(runtime.argv());
func(a,b,c,d="只有红茶可以吗"){
    println(a,' ',b,' ',c,' ',d,' true: ',true,' false: ',false);
}(c:1919810,b:514,a:114);

# test binary negation
println("~ 0x80: ",~0x80);
println("~ 0x8000: ",~0x8000);
println("~ 0x80000000: ",~0x80000000);
println("~ 0x8000000000000000: ",~0x8000000000000000);
println(~0x80000000==~0x8000000000000000);

var h=split(" ","0 1 2 3 4 5 6 7 8 9 a b c d e f");
for(var a=0;a<16;a+=1) {
    for(var b=0;b<16;b+=1) {
        for(var c=0;c<16;c+=1) {
            if(((a^b)&c)!=(a^(b&c))) {
                println("0x"~h[a],"^","0x"~h[b],"&","0x"~h[c]," -> a^b&c = 0x",h[a^b&c]," (a^b)&c = 0x",h[(a^b)&c]," a^(b&c) = 0x",h[(a^(b&c))]);
            }
        }
    }
}
for(var a=0;a<16;a+=1) {
    for(var b=0;b<16;b+=1) {
        var temp=b;
        println("temp^=0x"~h[a]~" -> 0x",h[temp^=a]," temp&=0x"~h[a]~" -> 0x",h[temp&=a]," temp|=0x"~h[a]~" -> 0x",h[temp|=a]);
    }
}