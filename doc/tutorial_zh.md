# __教程__

![mandelbrotset](../doc/pic/mandelbrotset.png)

Nasal非常容易上手，你可以在15分钟之内看完基本教程并直接开始编写程序。

## __目录__

* [__基本类型__](#基本类型)
* [__运算符__](#运算符)
* [__定义变量__](#定义变量)
* [__多变量赋值__](#多变量赋值)
* [__条件语句__](#条件语句)
* [__循环语句__](#循环语句)
* [__生成子列表(subvec)__](#生成子列表subvec)
* [__特殊函数调用语法__](#特殊函数调用语法)
* [__Lambda 表达式__](#lambda表达式)
* [__闭包__](#闭包)
* [__特性与继承__](#特性与继承)
* [__多文件/模块导入__](#多文件模块导入)
* [__原生内置函数以及模块导入__](#原生内置函数以及模块导入)
* [__C++ 模块(开发者教程)__](#c-模块开发者教程)
* [__自定义类型(开发者教程)__](#自定义类型开发者教程)

## 基本类型

__`none`__ 是特殊的错误类型。这个类型用于终止虚拟机的执行，该类型只能由虚拟机在抛出错误时产生。

__`nil`__ 是空类型。类似于null。

```javascript
var spc = nil;
```

__`num`__ 有三种形式:十进制，十六进制以及八进制。并且该类型使用IEEE754标准的浮点数`double`格式来存储。

```javascript
# 该语言用 '#' 来作为注释的开头
var n = 2.71828;    # dec 十进制
var n = 2.147e16;   # dec 十进制
var n = 1e-10;      # dec 十进制
var n = 0xAA55;     # hex 十六进制
var n = 0o170001;   # oct 八进制

# 注意: true 和 false 关键字在现在的 nasal 里也是可用的
var n = true;       # n 实际上是数字 1.0
var n = false;      # n 实际上是数字 0.0
```

__`str`__ 也有三种不同的格式。第三种只允许包含一个的字符。

```javascript
var s = 'str';
var s = "another string";
var s = `c`;
# 该语言也支持一些特别的转义字符:
'\a'; '\b'; '\e'; '\f';
'\n'; '\r'; '\t'; '\v';
'\0'; '\\'; '\?'; '\'';
'\"';
```

__`vec`__ 有不受限制的长度并且可以存储所有类型的数据。(当然不能超过可分配内存空间的长度)

```javascript
var vec = [];
var vec = [0, nil, {}, [], func(){return 0}];
append(vec, 0, 1, 2);
```

__`hash`__ 使用哈希表 (类似于`python`中的`dict`)，通过键值对来存储数据。key可以是一个字符串，也可以是一个标识符。

```javascript
var hash = {
    member1: nil,
    member2: "str",
    "member3": "member\'s name can also be a string constant",
    funct: func() {
        return me.member2~me.member3;
    }
};
```

__`func`__ 函数类型。(实际上在这个语言里函数是一种`lambda`表达式)

```javascript
var f = func(x, y, z) {
    return nil;
}
# 函数声明可以没有参数列表以及 `(`, `)`
var f = func {
    return 114514;
}
var f = func(x, y, z, deft = 1) {
    return x+y+z+deft;
}
var f = func(args...) {
    var sum = 0;
    foreach(var i; args) {
        sum += i;
    }
    return sum;
}
```

__`upval`__ 是存储闭包数据的特殊类型, 在 __`vm`__ 中使用，以确保闭包功能正常。

__`ghost`__ 是用来存储`C/C++`的一些复杂数据结构。这种类型的数据由内置函数生成。如果想为nasal添加新的数据结构, 可以看下文如何通过修改本项目来添加内置函数。

## 运算符

Nasal拥有基本的四种数学运算符 `+` `-` `*` `/`以及一个特别的运算符 `~`，用于拼接字符串或者数组。

```javascript
1+2-(1+3)*(2+4)/(16-9);
"str1"~"str2";
[0]~[1]; # should be [0, 1]
```

对于条件语句，可以使用`==` `!=` `<` `>` `<=` `>=`来比较数据。`and` `or` 与C/C++中 `&&` `||`运算符一致。

```javascript
1+1 and (1<0 or 1>0);
1<=0 and 1>=0;
1==0 or 1!=0;
```

单目运算符`-` `!`与C/C++中的运算符功能类似。

```javascript
-1;
!0;
```

位运算符`~` `|` `&` `^`与C/C++中的运算符功能类似。

```javascript
# 运行过程:
# 1. 将 f64 强转为 i32 (static_cast<int32_t>)
# 2. 执行位运算符

~0x80000000; # 按位取反 2147483647
0x8|0x1;     # 按位或
0x1&0x2;     # 按位与
0x8^0x1;     # 按位异或
```

赋值运算符`=` `+=` `-=` `*=` `/=` `~=` `^=` `&=` `|=`正如其名，用于进行赋值。

```javascript
a = b = c = d = 1;
a += 1;
a -= 1;
a *= 1;
a /= 1;
a ~= "string";

a ^= 0xff;
a &= 0xca;
a |= 0xba;

a = [0];
a ~= [1]; # should be [0, 1]
```

`??` 运算符用于检查左侧值是否为 `nil`，如果不是则返回右侧的值：

```javascript
print(nil??"should get this string");
```

上面的例子会输出 `should get this string`。

`?.` 运算符用于先检查左侧不为 `nil`，如果左侧不是 `nil` 则尝试获取 hash 的字段。
当然如果左侧此时也不是 `hash` 类型，则报错退出。

```javascript
var a = nil;
print(a?.try_get); # nil

var a = {try_get: "congrats!"};
print(a?.try_get); # "congrats!"
```

## 定义变量

如下所示。

```javascript
var a = 1;             # 定义单个变量
var (a, b, c) = [0, 1, 2]; # 从数组中初始化多个变量
var (a, b, c) = (0, 1, 2); # 从元组中初始化多个变量
```

Nasal 有很多特别的全局变量：

```javascript
globals; # 包含所有全局声明变量名和对应数据的哈希表
arg;     # 在全局作用域，arg 是包含命令行参数的数组
         # 在局部作用域，arg 是函数调用时的动态参数数组
```

具体实例：

```javascript
var a = 1;
println(globals); # 输出 {a:1}
```

```javascript
# nasal a b c
println(arg); # 输出 ["a", "b", "c"]

func() {
    println(arg);
}(1, 2, 3);   # 输出 [1, 2, 3]
```

## 多变量赋值

最后这个语句通常用于交换两个变量的数据，类似于Python中的操作。

```javascript
(a, b[0], c.d) = [0, 1, 2];
(a, b[1], c.e) = (0, 1, 2);
(a, b) = (b, a);
```

## 条件语句

nasal在提供`else if`的同时还有另外一个关键字`elsif`。该关键字与`else if`有相同的功能。

```javascript
if (1) {
    ;
} elsif (2) {
    ;
} else if (3) {
    ;
} else {
    ;
}
```

## 循环语句

while循环和for循环大体上与C/C++是一致的。

```javascript
while(condition) {
    continue;
}
for(var i = 0; i<10; i += 1) {
    break;
}
```

同时，nasal还有另外两种直接遍历列表的循环方式:

`forindex` 会获取列表的下标，依次递增. 下标会从`0`递增到`size(elem)-1`结束。

```javascript
forindex(var i; elem) {
    print(elem[i]);
}
```

`foreach`会依次直接获取列表中的数据. 这些数据会从`elem[0]`依次获取到`elem[size(elem)-1]`.

```javascript
foreach(var i; elem) {
    print(i);
}
```

## 生成子列表(subvec)

nasal提供了下面第一句的类似语法来从列表中随机或者按照一个区间获取数据，并且拼接生成一个新的列表。当然如果中括号内只有一个下标的话，你会直接获得这个下标对应的数据而不是一个子列表。如果直接对string使用下标来获取内容的话，会得到对应字符的 __ascii值__。如果你想进一步获得这个字符串，可以尝试使用内置函数`chr()`。

```javascript
a[0];
a[-1, 1, 0:2, 0:, :3, :, nil:8, 3:nil, nil:nil];
"hello world"[0];
```

## 特殊函数调用语法

这种调用方式不是很高效，因为哈希表会使用字符串比对来找到数据存放的位置。

然而如果它用起来非常舒适，那效率也显得不是非常重要了……

```javascript
f(x:0, y:nil, z:[]);
```

## lambda表达式

函数有这样一种直接编写函数体并且立即调用的方式:

```javascript
func(x, y) {
    return x+y;
}(0, 1);
func(x) {
    return 1/(1+math.exp(-x));
}(0.5);
```

测试文件中有一个非常有趣的文件`y-combinator.nas`，可以试一试:

```javascript
var fib = func(f) {
    return f(f);
}(
    func(f) {
        return func(x) {
            if(x<2) return x;
            return f(f)(x-1)+f(f)(x-2);
        }
    }
);
```

## 闭包

闭包是一种特别的作用域，你可以从这个作用域中获取其保存的所有变量，
而这些变量原本不是你当前运行的函数的局部作用域中的。
下面这个例子里，结果是`1`:

```javascript
var f = func() {
    var a = 1;
    return func() {return a;};
}
print(f()());
```

如果善用闭包，你可以使用它来进行面向对象编程。

```javascript
var student = func(n, a) {
    var (name, age) = (n, a);
    return {
        print_info: func()  {println(name, ' ', age);},
        set_age:    func(a) {age = a;},
        get_age:    func()  {return age;},
        set_name:   func(n) {name = n;},
        get_name:   func()  {return name;}
    };
}
```

## 特性与继承

当然，也有另外一种办法来面向对象编程，那就是利用`trait`。

当一个hash类型中，有一个成员的key是`parents`，并且该成员是一个数组的话，
那么当你试图从这个hash中寻找一个它自己没有的成员名时，虚拟机会进一步搜索`parents`数组。
如果该数组中有一个hash类型，有一个成员的key与当前你搜索的成员名一致，
那么你会得到这个成员对应的值。

使用这个机制，我们可以进行面向对象编程，下面样例的结果是`114514`:

```javascript
var trait = {
    get: func {return me.val;},
    set: func(x) {me.val = x;}
};

var class = {
    new: func() {
        return {
            val: nil,
            parents: [trait]
        };
    }
};
var a = class.new();
a.set(114514);
println(a.get());
```

首先虚拟机会发现在`a`中找不到成员`set`，但是在`a.parents`中有个hash类型`trait`存在该成员，所以返回了这个成员的值。
成员`me`指向的是`a`自身，类似于一些语言中的`this`，所以我们通过这个函数，实际上修改了`a.val`。`get`函数的调用实际上也经过了相同的过程。

不过我们必须提醒你一点，如果你在这个地方使用该优化来减少hash的搜索开销:

```javascript
var trait = {
    get: func {return me.val;},
    set: func(x) {me.val = x;}
};

var class = {
    new: func() {
        return {
            val: nil,
            parents: [trait]
        };
    }
};
var a = class.new();
var b = class.new();
a.set(114);
b.set(514);
println(a.get());
println(b.get());

var c = a.get;
var d = b.get;

println(c());
println(c());
println(d());
println(d());
```

那么你会发现现在虚拟机会输出这个结果:

```bash
114
514
514
514
514
514
```

因为执行`a.get`时在`trait.get`函数的属性中进行了`me=a`的操作。而`b.get`则执行了`me=b`的操作。所以在运行`var d=b.get`后实际上c也变成`b.get`了。
如果你想要用这种小技巧来让程序运行更高效的话，最好是要知道这里存在这样一个机制。

## 多文件/模块导入

详情可见 [namespace.md](./namespace.md)

## 原生内置函数以及模块导入

这个部分对于纯粹的使用者来说是不需要了解的，
它将告诉你我们是如何为解释器添加新的内置函数的。
如果你对此很感兴趣，那么这个部分可能会帮到你，并且……

__警告:__ 如果你 __不想__ 通过直接修改解释器源码来添加你自定义的函数，那么你应该看下一个节 __`模块`__ 的内容。

如果你确实是想修改源码来搞一个自己私人订制的解释器 ———— “我他妈就是想自己私人订制，你们他妈的管得着吗？”，
参考源码中关于内置函数的部分，以及`lib.nas`中是如何包装这些函数的，下面是其中一个样例:

定义新的内置函数:

```C++
// 你可以使用这个宏来直接定义一个新的内置函数
var builtin_print(context*, gc*);
```

然后用C++完成这个函数的函数体:

```C++
var builtin_print(context* ctx, gc* ngc) {
    // 局部变量的下标其实是从 1 开始的
    // 因为 local[0] 是保留给 'me' 的空间
    for(auto& i : ctx->localr[1].vec().elems) {
        std::cout << i;
    }
    std::cout << std::flush;
    // 最后生成返回值,返回值必须是一个内置的类型，
    // 可以使用ngc::alloc(type)来申请一个需要内存管理的复杂数据结构
    // 或者用我们已经定义好的nil/one/zero，这些可以直接使用
    return nil;
}
```

当运行内置函数的时候，内存分配器如果运行超过一次，那么会有更大可能性多次触发垃圾收集器的mark-sweep。这个操作会在`gc::alloc`中触发。
如果先前获取的数值没有被正确存到可以被垃圾收集器索引到的地方，那么它会被错误地回收，这会导致严重的错误。

可以使用`gc::temp`来暂时存储一个会被返回的需要gc管理的变量，这样可以防止内部所有的申请错误触发垃圾回收。如下所示：

```C++
var builtin_keys(context* ctx, gc* ngc) {
    auto hash = ctx->localr[1];
    if (hash.type!=vm_hash && hash.type!=vm_map) {
        return nas_err("keys", "\"hash\" must be hash");
    }
    // 使用gc.temp来存储gc管理的变量，防止错误的回收
    auto res = ngc->temp = ngc->alloc(vm_vec);
    auto& vec = res.vec().elems;
    if (hash.type==vm_hash) {
        for(const auto& iter : hash.hash().elems) {
            vec.push_back(ngc->newstr(iter.first));
        }
    } else {
        for(const auto& iter : hash.map().mapper) {
            vec.push_back(ngc->newstr(iter.first));
        }
    }
    ngc->temp = nil;
    return res;
}
```

这些工作都完成之后，在内置函数注册表中填写它在nasal中的别名，并且在表中填对这个函数的函数指针:

```C++
nasal_builtin_table builtin[] = {
    {"__print", builtin_print},
    {nullptr,  nullptr}
};
```

最后，将其包装到nasal文件中:

```javascript
var print = func(elems...) {
    return __print(elems);
};
```

事实上`__print`后面跟着的传参列表不是必须要写的。所以这样写也对:

```javascript
var print = func(elems...) {
    return __print;
};
```

如果你不把内置函数包装到一个普通的nasal函数中，那么直接调用这个内置函数会在参数传入阶段出现 __segmentation fault(段错误)__。

在nasal文件中使用`import("文件名.nas")`可以导入该文件中你包装的所有内置函数，接下来你就可以使用他们了。
当然也有另外一种办法来导入这些nasal文件，下面两种导入方式的效果是一样的：

```javascript
use dirname.dirname.filename;
import("./dirname/dirname/filename.nas");
```

## C++ 模块(开发者教程)

如果只有上文中那种方式来添加你自定义的函数到nasal中，这肯定是非常麻烦的。
因此，我们实现了一组实用的内置函数来帮助你添加你自己创建的模块。

用于加载动态库的函数在`std/dylib.nas`中:

```javascript
var dlopen = func(libname) {
    ...
}

var dlclose = func(lib) {
    ...
}

var dlcall = func(ptr, args...) {
    ...
}

var limitcall = func(arg_size = 0) {
    ...
}
```

这些函数是用来加载动态库的，这样nasal解释器可以根据用户需求灵活加载动态库来执行。让我们看看这些函数该如何使用。

首先，用C++写个项目，并且编译成动态库。我们就拿`fib.cpp`作为例子来说明(样例代码可以在`./module`中找到):

```C++
// 这个头文件得加上，因为我们需要拿到nasal的api
#include "nasal.h"
double fibonaci(double x) {
    if (x<=2) {
        return x;
    }
    return fibonaci(x-1)+fibonaci(x-2);
}

// 模块函数的参数列表一律以这个为准
var fib(var* args, usize size, gc* ngc) {
    if (!size) {
        return nas_err("fib", "lack arguments");
    }
    // 传参会给予一个var指针，指向一个vm_vec的data()
    var num = args[0];
    // 如果你想让这个函数有更强的稳定性，那么一定要进行合法性检查
    // nas_err会输出错误信息并返回错误类型让虚拟机终止执行
    if(num.type!=vm_num) {
        return nas_err("extern_fib", "\"num\" must be number");
    }
    // vm_num作为普通的数字类型，不是内存管理的对象，所以无需申请
    // 如果需要返回内存管理的对象，请使用ngc->alloc(type)
    return var::num(fibonaci(num.tonum()));
}

// 然后将函数名字和函数地址放到一个表里，一定要记住表尾是{nullptr,nullptr}
module_func_info func_tbl[] = {
    {"fib", fib},
    {nullptr, nullptr}
};

// 必须实现这个函数, 这样nasal可以通过字符串名字获得函数指针
// 之所以用这种方式来获取函数指针, 是因为`var`是有构造函数的
// 有构造函数的类型作为返回值, 和C是不兼容的, 这导致
// 类似 "extern "C" var fib" 的写法会得到编译错误
NASAL_EXTERN module_func_info* get() {
    return func_tbl;
}
```

接着我们把`fib.cpp`编译成动态库。

Linux(`.so`):

`clang++ -c -O3 fib.cpp -fPIC -o fib.o`

`clang++ -shared -o libfib.so fib.o`

Mac(`.so` & `.dylib`): 和Linux下操作相同。

Windows(`.dll`):

`g++ -c -O3 fib.cpp -fPIC -o fib.o`

`g++ -shared -o libfib.dll fib.o`

好了，那么我们可以写一个测试用的nasal代码来运行这个斐波那契函数了。
下面例子中`os.platform()`是用来检测当前运行的系统环境的，这样可以实现跨平台:

```javascript
use std.dylib;
var dlhandle = dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
var fib = dlhandle.fib;
for(var i = 1; i<30; i += 1)
    println(dylib.dlcall(fib, i));
dylib.dlclose(dlhandle.lib);
```

`dylib.dlopen`用于加载动态库并从动态库中获得函数地址。

`dylib.dlcall`用于调用函数，第一个参数是动态库函数的地址，这是个特殊类型，一定要保证这个参数是`vm_obj`类型并且`type=obj_extern`。

`dylib.dlclose`用于卸载动态库，当然，在这个函数调用之后，所有从该库中获取的函数都作废。

`dylib.limitcall`用于获取使用固定长度传参的 `dlcall` 函数，这种函数可以提高你的程序运行效率，因为它不需要用 `vm_vec` 来存储传入参数，而是使用局部作用域来直接存储，从而避免了频繁调用可能导致的频繁垃圾收集。所以上面展示的代码同样可以这样写：

```javascript
use std.dylib;
var dlhandle = dylib.dlopen("libfib."~(os.platform()=="windows"?"dll":"so"));
var fib = dlhandle.fib;
var invoke = dylib.limitcall(1); # this means the called function has only one parameter
for(var i = 1; i<30; i += 1)
    println(invoke(fib, i));
dylib.dlclose(dlhandle.lib);
```

如果得到如下运行结果，恭喜你！

```bash
./nasal a.nas
1
2 
3 
5 
8 
13
21
34
55
89
144
233
377
610
987
1597
2584
4181
6765
10946
17711
28657
46368
75025
121393
196418
317811
514229
832040
```

## 自定义类型(开发者教程)

创建一个自定义类型很容易。下面是使用示例：

```c++
const auto ghost_for_test = "ghost_for_test";

// 声明自定义类型的析构函数
void ghost_for_test_destructor(void* ptr) {
    std::cout << "ghost_for_test::destructor (0x";
    std::cout << std::hex << reinterpret_cast<u64>(ptr) << std::dec << ") {\n";
    delete static_cast<u32*>(ptr);
    std::cout << "    delete 0x" << std::hex;
    std::cout << reinterpret_cast<u64>(ptr) << std::dec << ";\n";
    std::cout << "}\n";
}

var create_new_ghost(var* args, usize size, gc* ngc) {
    var res = ngc->alloc(vm_obj);
    // 创建自定义类型
    res.ghost().set(ghost_for_test, ghost_for_test_destructor, new u32);
    return res;
}

var set_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    if (!res.object_check(ghost_for_test)) {
        std::cout << "set_new_ghost: not ghost for test type.\n";
        return nil;
    }
    f64 num = args[1].num();
    *(reinterpret_cast<u32*>(res.ghost().pointer)) = static_cast<u32>(num);
    std::cout << "set_new_ghost: successfully set ghost = " << num << "\n";
    return nil;
}

var print_new_ghost(var* args, usize size, gc* ngc) {
    var res = args[0];
    // 用自定义类型的名字来检查是否是正确的自定义类型
    if (!res.object_check(ghost_for_test)) {
        std::cout << "print_new_ghost: not ghost for test type.\n";
        return nil;
    }
    std::cout << "print_new_ghost: " << res.ghost() << " result = "
              << *((u32*)res.ghost().pointer) << "\n";
    return nil;
}
```

我们使用下面这个函数来创建一个自定义类型：

`void nas_ghost::set(const std::string&, nasal::nas_ghost::destructor, void*);`

`const std::string&` 是自定义类型的类型名。

`nasal::nas_ghost::destructor` 是自定义类型的析构函数指针。

`void*` 是指向自定义类型实例的指针。

我们使用下面的这个函数检测是否是正确的自定义类型：

`bool var::object_check(const std::string&);`

参数是自定义类型的类型名。
