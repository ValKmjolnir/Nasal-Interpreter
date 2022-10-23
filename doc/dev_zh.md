# __开发历史记录__

## __目录__

* [__语法分析__](#语法分析)
  * [v1.0](#version-10-parser-last-update-20191014)
* [__抽象语法树__](#抽象语法树)
  * [v1.2](#version-12-ast-last-update-20191031)
  * [v2.0](#version-20-ast-last-update-2020831)
  * [v3.0](#version-30-ast-last-update-20201023)
  * [v5.0](#version-50-ast-last-update-202137)
* [__字节码虚拟机__](#字节码虚拟机)
  * [v4.0](#version-40-vm-last-update-20201217)
  * [v5.0](#version-50-vm-last-update-202137)
  * [v6.0](#version-60-vm-last-update-202161)
  * [v6.5](#version-65-vm-last-update-2021624)
  * [v7.0](#version-70-vm-last-update-2021108)
  * [v8.0](#version-80-vm-last-update-2022212)
  * [v9.0](#version-90-vm-last-update-2022518)
  * [v10.0](#version-100-vm-latest)
* [__发行日志__](#发行日志)

## __语法分析__

有特殊语法检查的`LL(1)`语法分析器。

```javascript
(var a,b,c)=[{b:nil},[1,2],func return 0;];
(a.b,b[0],c)=(1,2,3);
```

这两个表达式有同一个first集，所以纯粹的`LL(1)`很难实现这个语言的语法分析。所以我们为其添加了特殊语法检查机制。本质上还是`LL(1)`的内核。

上面这个问题已经解决很久了，不过我最近发现了一个新的语法问题:

```javascript
var f=func(x,y,z){return x+y+z}
(a,b,c)=(0,1,2);
```

这种写法会被错误识别合并成下面这种:

```javascript
var f=func(x,y,z){return x+y+z}(a,b,c)
=(0,1,2);
```

语法分析器会认为这是个严重的语法错误。我在Flightgear中也测试了这个代码，它内置的语法分析器也认为这是错误语法。当然我认为这是语法设计中的一个比较严重的缺漏。为了避免这个语法问题，只需要添加一个分号就可以了:

```javascript
var f=func(x,y,z){return x+y+z};
                               ^ 就是这里
(a,b,c)=(0,1,2);
```

### version 1.0 parser (last update 2019/10/14)

第一版功能完备的nasal语法分析器完成了。

在version 1.0之前，我多次尝试构建一个正确的语法分析器但总是存在一些问题。

最终我学习了`LL(1)`和`LL(k)`文法并且在version 0.16(last update 2019/9/14)中完成了一个能识别数学算式的语法分析器。

在version 0.17(2019/9/15) 0.18(2019/9/18) 0.19(2019/10/1)中我只是抱着玩的心态在测试语法分析器，不过在那之后我还是完成了version 1.0的语法分析器。

__该项目于2019/7/25正式开始__。

## __抽象语法树__

### version 1.2 ast (last update 2019/10/31)

抽象语法树在这个版本初步完成。

### version 2.0 ast (last update 2020/8/31)

在这个版本我们基于抽象语法树实现了一个树解释器，并且完成了部分内置函数。

### version 3.0 ast (last update 2020/10/23)

我们重构了抽象语法树的代码，现在可以更容易地读懂代码并进行维护。

这个版本的树解释器用了新的优化方式，所以可以更高效地执行代码。

在这个版本用户已经可以自行添加内置函数。

我想在v4.0发布之后仍然保留这个树解释器，毕竟花了很长时间才写完这坨屎。

### version 5.0 ast (last update 2021/3/7)

我改变想法了，树解释器给维护带来了太大的麻烦。如果想继续保留这个解释器，那么为了兼容性，字节码虚拟机的优化工作会更难推进。

## __字节码虚拟机__

![op](../doc/gif/opcode.gif)

### version 4.0 vm (last update 2020/12/17)

我在这个版本实现了第一版字节码虚拟机。不过这个虚拟机仍然在测试中，在这次测试结束之后，我会发布v4.0发行版。

现在我在找一些隐藏很深的bug。如果有人想帮忙的话，非常欢迎！:)

下面是生成的字节码的样例:

```javascript
for(var i=0;i<4000000;i+=1);
```

```x86asm
.number 0
.number 4e+006
.number 1
.symbol i
0x00000000: pzero  0x00000000
0x00000001: loadg  0x00000000 (i)
0x00000002: callg  0x00000000 (i)
0x00000003: pnum   0x00000001 (4e+006)
0x00000004: less   0x00000000
0x00000005: jf     0x0000000b
0x00000006: pone   0x00000000
0x00000007: mcallg 0x00000000 (i)
0x00000008: addeq  0x00000000
0x00000009: pop    0x00000000
0x0000000a: jmp    0x00000002
0x0000000b: nop    0x00000000
```

### version 5.0 vm (last update 2021/3/7)

从这个版本起，我决定持续优化字节码虚拟机。

毕竟现在这玩意从`0`数到`4000000-1`要花费1.5秒。这效率完全不能忍。

2021/1/23 update: 现在它确实可以在1.5秒内从`0`数到`4000000-1`了。

### version 6.0 vm (last update 2021/6/1)

使用`loadg`/`loadl`/`callg`/`calll`/`mcallg`/`mcalll`指令来减少分支语句的调用。

删除了`vm_scop`类型。

添加作为常量的`vm_num`来减少内存分配的开销。

将垃圾收集器从引用计数改为了标记清理。

`vapp`和`newf`开始使用先前未被使用的.num段来压缩字节码生成数量，减少生成的`exec_code`的大小。

2021/4/3 update: 从`0`数到`4e6-1`只需要不到0.8秒了。

2021/4/19 update: 从`0`数到`4e6-1`只需要不到0.4秒了。

在这次的更新中，我把全局变量和局部变量的存储结构从`unordered_map`变为了`vector`，从而提升执行效率。所以现在生成的字节码大变样了。

```javascript
for(var i=0;i<4000000;i+=1);
```

```x86asm
.number 4e+006
0x00000000: intg   0x00000001
0x00000001: pzero  0x00000000
0x00000002: loadg  0x00000000
0x00000003: callg  0x00000000
0x00000004: pnum   0x00000000 (4e+006)
0x00000005: less   0x00000000
0x00000006: jf     0x0000000c
0x00000007: pone   0x00000000
0x00000008: mcallg 0x00000000
0x00000009: addeq  0x00000000
0x0000000a: pop    0x00000000
0x0000000b: jmp    0x00000003
0x0000000c: nop    0x00000000
```

### version 6.5 vm (last update 2021/6/24)

2021/5/31 update:

现在垃圾收集器不会错误地重复收集未使用变量了。

添加了`builtin_alloc`以防止在运行内置函数的时候错误触发标记清除。

建议在获取大空间数组的时候尽量使用setsize，因为`append`在被频繁调用时可能会频繁触发垃圾收集器。

2021/6/3 update:

修复了垃圾收集器还是他妈的会重复收集的bug，这次我设计了三个标记状态来保证垃圾是被正确收集了。

将`callf`指令拆分为`callfv`和`callfh`。并且`callfv`将直接从`val_stack`获取传参，而不是先通过一个`vm_vec`把参数收集起来再传入，后者是非常低效的做法。

建议更多使用`callfv`而不是`callfh`，因为`callfh`只能从栈上获取参数并整合为`vm_hash`之后才能传给该指令进行处理，拖慢执行速度。

```javascript
var f=func(x,y){return x+y;}
f(1024,2048);
```

```x86asm
.number 1024
.number 2048
.symbol x   
.symbol y
0x00000000: intg   0x00000001
0x00000001: newf   0x00000007
0x00000002: intl   0x00000003
0x00000003: offset 0x00000001
0x00000004: para   0x00000000 (x)
0x00000005: para   0x00000001 (y)
0x00000006: jmp    0x0000000b
0x00000007: calll  0x00000001
0x00000008: calll  0x00000002
0x00000009: add    0x00000000
0x0000000a: ret    0x00000000
0x0000000b: loadg  0x00000000
0x0000000c: callg  0x00000000
0x0000000d: pnum   0x00000000 (1024)
0x0000000e: pnum   0x00000001 (2048)
0x0000000f: callfv 0x00000002
0x00000010: pop    0x00000000
0x00000011: nop    0x00000000
```

2021/6/21 update:

现在垃圾收集器不会收集空指针了。并且调用链中含有函数调用的赋值语句现在也可以执行了，下面这些赋值方式是合法的:

```javascript
var f=func()
{
    var _=[{_:0},{_:1}];
    return func(x)
    {
        return _[x];
    }
}
var m=f();
m(0)._=m(1)._=10;

[0,1,2][1:2][0]=0;
```

在老版本中，语法分析器会检查左值，并且在检测到有特别调用的情况下直接告知用户这种左值是不被接受的(bad lvalue)。但是现在它可以正常运作了。为了保证这种赋值语句能正常执行，codegen模块会优先使用`codegen::call_gen()`生成前面调用链的字节码而不是全部使用 `codegen::mcall_gen()`，在最后一个调用处才会使用`codegen::mcall_gen()`。

所以现在生成的相关字节码也完全不同了:

```x86asm
.number 10
.number 2
.symbol _
.symbol x
0x00000000: intg   0x00000002
0x00000001: newf   0x00000005
0x00000002: intl   0x00000002
0x00000003: offset 0x00000001
0x00000004: jmp    0x00000017
0x00000005: newh   0x00000000
0x00000006: pzero  0x00000000
0x00000007: happ   0x00000000 (_)
0x00000008: newh   0x00000000
0x00000009: pone   0x00000000
0x0000000a: happ   0x00000000 (_)
0x0000000b: newv   0x00000002
0x0000000c: loadl  0x00000001
0x0000000d: newf   0x00000012
0x0000000e: intl   0x00000003
0x0000000f: offset 0x00000002
0x00000010: para   0x00000001 (x)
0x00000011: jmp    0x00000016
0x00000012: calll  0x00000001
0x00000013: calll  0x00000002
0x00000014: callv  0x00000000
0x00000015: ret    0x00000000
0x00000016: ret    0x00000000
0x00000017: loadg  0x00000000
0x00000018: callg  0x00000000
0x00000019: callfv 0x00000000
0x0000001a: loadg  0x00000001
0x0000001b: pnum   0x00000000 (10.000000)
0x0000001c: callg  0x00000001
0x0000001d: pone   0x00000000
0x0000001e: callfv 0x00000001
0x0000001f: mcallh 0x00000000 (_)
0x00000020: meq    0x00000000
0x00000021: callg  0x00000001
0x00000022: pzero  0x00000000
0x00000023: callfv 0x00000001
0x00000024: mcallh 0x00000000 (_)
0x00000025: meq    0x00000000
0x00000026: pop    0x00000000
0x00000027: pzero  0x00000000
0x00000028: pzero  0x00000000
0x00000029: pone   0x00000000
0x0000002a: pnum   0x00000001 (2.000000)
0x0000002b: newv   0x00000003
0x0000002c: slcbeg 0x00000000
0x0000002d: pone   0x00000000
0x0000002e: pnum   0x00000001 (2.000000)
0x0000002f: slc2   0x00000000
0x00000030: slcend 0x00000000
0x00000031: pzero  0x00000000
0x00000032: mcallv 0x00000000
0x00000033: meq    0x00000000
0x00000034: pop    0x00000000
0x00000035: nop    0x00000000
```

从上面这些字节码可以看出，`mcall`/`mcallv`/`mcallh`指令的使用频率比以前减小了一些，而`call`/`callv`/`callh`/`callfv`/`callfh`则相反。

并且因为新的数据结构，`mcall`指令以及`addr_stack`，一个曾用来存储指针的栈，从`vm`中被移除。现在`vm`使用`nas_val** mem_addr`来暂存获取的内存地址。这不会导致严重的问题，因为内存空间是 __获取即使用__ 的。

### version 7.0 vm (last update 2021/10/8)

2021/6/26 update:

指令分派方式从call-threading改为了computed-goto。在更改了指令分派方式之后，`vm`的执行效率有了非常巨大的提升。现在虚拟机可以在0.2秒内执行完`test/bigloop`和`test/pi`！并且在linux平台虚拟机可以在0.8秒内执行完`test/fib`。你可以在下面的测试数据部分看到测试的结果。

这个分派方式使用了g++扩展"labels as values"，clang++目前也支持这种指令分派的实现方式。(不过MSVC支不支持就不得而知了，哈哈)

`gc`中也有部分改动:
全局变量不再用`std::vector`存储，而是全部存在操作数栈上(从`val_stack+0`到`val_stack+intg-1`)。

2021/6/29 update:

添加了一些直接用常量进行运算的指令:
`op_addc`,`op_subc`,`op_mulc`,`op_divc`,`op_lnkc`,`op_addeqc`,`op_subeqc`,`op_muleqc`,`op_diveqc`,`op_lnkeqc`。

现在test/bigloop.nas的字节码是这样的:

```x86asm
.number 4e+006
.number 1
0x00000000: intg   0x00000001
0x00000001: pzero  0x00000000
0x00000002: loadg  0x00000000
0x00000003: callg  0x00000000
0x00000004: pnum   0x00000000 (4000000)
0x00000005: less   0x00000000
0x00000006: jf     0x0000000b
0x00000007: mcallg 0x00000000
0x00000008: addeqc 0x00000001 (1)
0x00000009: pop    0x00000000
0x0000000a: jmp    0x00000003
0x0000000b: nop    0x00000000
```

在这次更新之后，这个测试文件可以在0.1秒内运行结束。大多数的运算操作速度都有提升。

并且赋值相关的字节码也有一些改动。现在赋值语句只包含一个标识符时，会优先调用`op_load`来赋值，而不是使用`op_meq`和`op_pop`。

```javascript
var (a,b)=(1,2);
a=b=0;
```

```x86asm
.number 2
0x00000000: intg   0x00000002
0x00000001: pone   0x00000000
0x00000002: loadg  0x00000000
0x00000003: pnum   0x00000000 (2)
0x00000004: loadg  0x00000001
0x00000005: pzero  0x00000000
0x00000006: mcallg 0x00000001
0x00000007: meq    0x00000000 (b=2 use meq,pop->a)
0x00000008: loadg  0x00000000 (a=b use loadg)
0x00000009: nop    0x00000000
```

### version 8.0 vm (last update 2022/2/12)

2021/10/8 update:

从这个版本开始`vm_nil`和`vm_num`不再由`gc`管理，这会大幅度降低`gc::alloc`的调用并且会大幅度提升执行效率。

添加了新的数据类型: `vm_obj`。这个类型是留给用户定义他们想要的数据类型的。相关的API会在未来加入。

功能完备的闭包：添加了读写闭包数据的指令。删除了老的指令`op_offset`。

2021/10/13 update:

字节码信息输出格式修改为如下形式:

```x86asm
0x000002f2: newf   0x2f6
0x000002f3: intl   0x2
0x000002f4: para   0x3e ("x")
0x000002f5: jmp    0x309
0x000002f6: calll  0x1
0x000002f7: lessc  0x0 (2)
0x000002f8: jf     0x2fb
0x000002f9: calll  0x1
0x000002fa: ret
0x000002fb: upval  0x0[0x1]
0x000002fc: upval  0x0[0x1]
0x000002fd: callfv 0x1
0x000002fe: calll  0x1
0x000002ff: subc   0x1d (1)
0x00000300: callfv 0x1
0x00000301: upval  0x0[0x1]
0x00000302: upval  0x0[0x1]
0x00000303: callfv 0x1
0x00000304: calll  0x1
0x00000305: subc   0x0 (2)
0x00000306: callfv 0x1
0x00000307: add
0x00000308: ret
0x00000309: ret
0x0000030a: callfv 0x1
0x0000030b: loadg  0x32
```

2022/1/22 update:

删除`op_pone`和`op_pzero`。这两个指令在目前已经没有实际意义，并且已经被`op_pnum`替代。

### version 9.0 vm (last update 2022/5/18)

2022/2/12 update:

局部变量现在也被 __存储在栈上__。
所以函数调用比以前也会快速很多。
在v8.0如果你想调用一个函数，
新的`vm_vec`将被分配出来用于模拟局部作用域，这个操作会导致标记清除过程会被频繁触发并且浪费太多的执行时间。
在测试文件`test/bf.nas`中，这种调用方式使得大部分时间都被浪费了，因为这个测试文件包含大量且频繁的函数调用(详细数据请看测试数据一节中`version 8.0 (R9-5900HX ubuntu-WSL 2022/1/23)`)。

现在闭包会在第一次在局部作用域创建新函数的时候产生，使用`vm_vec`。
在那之后如果再创建新的函数，则他们会共享同一个闭包，这些闭包会在每次于局部作用域创建新函数时同步。

2022/3/27 update:

在这个月的更新中我们把闭包的数据结构从`vm_vec`换成了一个新的对象`vm_upval`，这种类型有着和另外一款编程语言 __`Lua`__ 中闭包相类似的结构。

同时我们也修改了字节码的输出格式。新的格式看起来像是 `objdump`:

```x86asm
  0x0000029b:       0a 00 00 00 00        newh

func <0x29c>:
  0x0000029c:       0b 00 00 02 a0        newf    0x2a0
  0x0000029d:       02 00 00 00 02        intl    0x2
  0x0000029e:       0d 00 00 00 66        para    0x66 ("libname")
  0x0000029f:       32 00 00 02 a2        jmp     0x2a2
  0x000002a0:       40 00 00 00 42        callb   0x42 <__dlopen@0x41dc40>
  0x000002a1:       4a 00 00 00 00        ret
<0x29c>;

  0x000002a2:       0c 00 00 00 67        happ    0x67 ("dlopen")

func <0x2a3>:
  0x000002a3:       0b 00 00 02 a8        newf    0x2a8
  0x000002a4:       02 00 00 00 03        intl    0x3
  0x000002a5:       0d 00 00 00 68        para    0x68 ("lib")
  0x000002a6:       0d 00 00 00 69        para    0x69 ("sym")
  0x000002a7:       32 00 00 02 aa        jmp     0x2aa
  0x000002a8:       40 00 00 00 43        callb   0x43 <__dlsym@0x41df00>
  0x000002a9:       4a 00 00 00 00        ret
<0x2a3>;

  0x000002aa:       0c 00 00 00 6a        happ    0x6a ("dlsym")
```

### version 10.0 vm (latest)

2022/5/19 update:

在这个版本中我们给nasal加入了协程:

```javascript
var coroutine={
    create: func(function){return __cocreate;},
    resume: func(co)      {return __coresume;},
    yield:  func(args...) {return __coyield; },
    status: func(co)      {return __costatus;},
    running:func()        {return __corun;   }
};
```

`coroutine.create`用于创建新的协程对象。不过创建之后协程并不会直接运行。

`coroutine.resume`用于继续运行一个协程。

`coroutine.yield`用于中断一个协程的运行过程并且抛出一些数据。这些数据会被`coroutine.resume`接收并返回。而在协程函数中`coroutine.yield`本身只返回`vm_nil`。

`coroutine.status`用于查看协程的状态。协程有三种不同的状态：`suspended`挂起，`running`运行中，`dead`结束运行。

`coroutine.running`用于判断当前是否有协程正在运行。

__注意:__ 协程不能在其他正在运行的协程中创建。

__接下来我们解释这个协程的运行原理:__

当`op_callb`被执行时，栈帧如下所示:

```C++
+----------------------+(主操作数栈)
| old pc(vm_ret)       | <- top[0]
+----------------------+
| old localr(vm_addr)  | <- top[-1]
+----------------------+
| old upvalr(vm_upval) | <- top[-2]
+----------------------+
| local scope(var)     |
| ...                  |
+----------------------+ <- local pointer stored in localr
| old funcr(vm_func)   | <- old function stored in funcr
+----------------------+
```

在`op_callb`执行过程中，下一步的栈帧如下:

```C++
+----------------------+(主操作数栈)
| nil(vm_nil)          | <- push nil
+----------------------+
| old pc(vm_ret)       |
+----------------------+
| old localr(vm_addr)  |
+----------------------+
| old upvalr(vm_upval) |
+----------------------+
| local scope(var)     |
| ...                  |
+----------------------+ <- local pointer stored in localr
| old funcr(vm_func)   | <- old function stored in funcr
+----------------------+
```

接着我们调用`resume`，这个函数会替换操作数栈。我们会看到，协程的操作数栈上已经保存了一些数据，但是我们首次进入协程执行时，这个操作数栈的栈顶将会是`vm_ret`，并且返回的`pc`值是`0`。

为了保证栈顶的数据不会被破坏，`resume`会返回`gc.top[0]`。`op_callb`将会执行`top[0]=resume()`，所以栈顶的数据虽然被覆盖了一次，但是实际上还是原来的数据。

```C++
+----------------------+(协程操作数栈)
| pc:0(vm_ret)         | <- now gc.top[0]
+----------------------+
```

当我们调用`yield`的时候，该函数会执行出这个情况，我们发现`op_callb` 已经把`nil`放在的栈顶。但是应该返回的`local[1]`到底发送到哪里去了？

```C++
+----------------------+(协程操作数栈)
| nil(vm_nil)          | <- push nil
+----------------------+
| old pc(vm_ret)       |
+----------------------+
| old localr(vm_addr)  |
+----------------------+
| old upvalr(vm_upval) |
+----------------------+
| local scope(var)     |
| ...                  |
+----------------------+ <- local pointer stored in localr
| old funcr(vm_func)   | <- old function stored in funcr
+----------------------+
```

当`builtin_coyield`执行完毕之后，栈又切换到了主操作数栈上，这时可以看到返回的`local[1]`实际上被`op_callb`放在了这里的栈顶:

```C++
+----------------------+(主操作数栈)
| return_value(var)    |
+----------------------+
| old pc(vm_ret)       |
+----------------------+
| old localr(vm_addr)  |
+----------------------+
| old upvalr(vm_upval) |
+----------------------+
| local scope(var)     |
| ...                  |
+----------------------+ <- local pointer stored in localr
| old funcr(vm_func)   | <- old function stored in funcr
+----------------------+
```

所以主程序会认为顶部这个返回值好像是`resume`返回的。而实际上`resume`的返回值在协程的操作数栈顶。综上所述:

```C++
resume (main->coroutine) return coroutine.top[0]. coroutine.top[0] = coroutine.top[0];
yield  (coroutine->main) return a vector.         main.top[0]      = vector;
```

## __发行日志__

### __version 8.0 release__

这个版本的发行版有个 __严重的问题__:

in __`nasal_dbg.h:215`__: `auto canary=gc.stack+STACK_MAX_DEPTH-1;`

这个会导致不正确的`stackoverflow`报错。因为它覆盖了原有的变量。
请修改为:

`canary=gc.stack+STACK_MAX_DEPTH-1;`

如果不修改这一行，调试器运行肯定是不正常的。在`v9.0`第一个commit中我们修复了这个问题。

另外一个bug在 `nasal_err.h:class nasal_err`这边，要给这个类添加一个构造函数来进行初始化，否则会出问题:

```C++
    nasal_err():error(0){}
```

同样这个也在`v9.0`中修复了。所以我们建议不要使用`v8.0`。
