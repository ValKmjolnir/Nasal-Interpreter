# __Development History__

![buringship](./pic/burningship.png)

## __Contents__

* [__Parser__](#parser)
  * [v1.0](#version-10-parser-last-update-20191014)
* [__Abstract Syntax Tree__](#abstract-syntax-tree)
  * [v1.2](#version-12-ast-last-update-20191031)
  * [v2.0](#version-20-ast-last-update-2020831)
  * [v3.0](#version-30-ast-last-update-20201023)
  * [v5.0](#version-50-ast-last-update-202137)
  * [v11.0](#version-110-ast-latest)
* [__Bytecode VM__](#bytecode-virtual-machine)
  * [v4.0](#version-40-vm-last-update-20201217)
  * [v5.0](#version-50-vm-last-update-202137)
  * [v6.0](#version-60-vm-last-update-202161)
  * [v6.5](#version-65-vm-last-update-2021624)
  * [v7.0](#version-70-vm-last-update-2021108)
  * [v8.0](#version-80-vm-last-update-2022212)
  * [v9.0](#version-90-vm-last-update-2022518)
  * [v10.0](#version-100-vm-last-update-2022816)
* [__Release Notes__](#release-notes)
  * [v8.0](#version-80-release)
  * [v11.0](#version-110-release)
  * [v11.1](#version-111-release)

## __Parser__

`LL(1)` parser with special check.

```javascript
(var a,b,c)=[{b:nil},[1,2],func return 0;];
(a.b,b[0],c)=(1,2,3);
```

These two expressions have the same first set,so `LL(1)` is useless for this language. We add some special checks in it.

Problems mentioned above have been solved for a long time, but recently i found a new problem here:

```javascript
var f = func(x, y, z) { return x + y + z }
(a, b, c) = (0, 1, 2);
```

This will be recognized as this:

```javascript
var f = func(x, y, z) { return x + y + z }(a, b, c)
= (0, 1, 2);
```

and causes fatal syntax error.
And i tried this program in flightgear nasal console.
It also found this is a syntax error.
I think this is a serious design fault.
To avoid this syntax error, change program like this, just add a semicolon:

```javascript
var f = func(x, y, z) { return x + y + z };
                                          ^ here
(a, b, c) = (0, 1, 2);
```

### version 1.0 parser (last update 2019/10/14)

First fully functional version of parser.

Before version 1.0,i tried many times to create a correct parser.

Finally i learned `LL(1)` and `LL(k)` and wrote a parser for math formulas in version 0.16(last update 2019/9/14).

In version 0.17(2019/9/15) 0.18(2019/9/18) 0.19(2019/10/1)i was playing the parser happily and after that i wrote version 1.0.

__This project began at 2019/7/25__.

## __Abstract Syntax Tree__

### version 1.2 ast (last update 2019/10/31)

The ast has been completed in this version.

### version 2.0 ast (last update 2020/8/31)

A completed ast-interpreter with unfinished lib functions.

### version 3.0 ast (last update 2020/10/23)

The ast is refactored and is now easier to read and maintain.

Ast-interpreter uses new techniques so it can run codes more efficiently.

Now you can add your own functions as builtin-functions in this interpreter!

I decide to save the ast interpreter after releasing v4.0. Because it took me a long time to think and write...

### version 5.0 ast (last update 2021/3/7)

I change my mind.
AST interpreter leaves me too much things to do.

If i continue saving this interpreter,
it will be harder for me to make the bytecode vm become more efficient.

### version 11.0 ast (latest)

Change ast framework. Now we use visitor pattern.

## __Bytecode Virtual Machine__

![op](../doc/gif/opcode.gif)

### version 4.0 vm (last update 2020/12/17)

I have just finished the first version of bytecode-interpreter.

This interpreter is still in test.
After this test, i will release version 4.0!

Now i am trying to search hidden bugs in this interpreter.
Hope you could help me! :)

There's an example of byte code below:

```javascript
for (var i=0;i<4000000;i+=1);
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

I decide to optimize bytecode vm in this version.

Because it takes more than 1.5s to count i from `0` to `4000000-1`.This is not efficient at all!

2021/1/23 update: Now it can count from `0` to `4000000-1` in 1.5s.

### version 6.0 vm (last update 2021/6/1)

Use `loadg`/`loadl`/`callg`/`calll`/`mcallg`/`mcalll` to avoid branches.

Delete type `vm_scop`.

Use const `vm_num` to avoid frequently new & delete.

Change garbage collector from reference-counting to mark-sweep.

`vapp` and `newf` operand use .num to reduce the size of `exec_code`.

2021/4/3 update: Now it can count from `0` to `4e6-1` in 0.8s.

2021/4/19 update: Now it can count from `0` to `4e6-1` in 0.4s.

In this update i changed global and local scope from `unordered_map` to `vector`.

So the bytecode generator changed a lot.

```javascript
for (var i=0;i<4000000;i+=1);
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

Now gc can collect garbage correctly without re-collecting,
which will cause fatal error.

Add `builtin_alloc` to avoid mark-sweep when running a built-in function,
which will mark useful items as useless garbage to collect.

Better use setsize and assignment to get a big array,
`append` is very slow in this situation.

2021/6/3 update:

Fixed a bug that gc still re-collects garbage,
this time i use three mark states to make sure garbage is ready to be collected.

Change `callf` to `callfv` and `callfh`.
And `callfv` fetches arguments from `val_stack` directly instead of using `vm_vec`,
a not very efficient way.

Better use `callfv` instead of `callfh`,
`callfh` will fetch a `vm_hash` from stack and parse it,
making this process slow.

```javascript
var f = func(x, y) { return x + y; }
f(1024, 2048);
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

2021/6/21 update: Now gc will not collect nullptr.
And the function of assignment is complete,
now these kinds of assignment is allowed:

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

In the old version,
parser will check this left-value and tells that these kinds of left-value are not allowed(bad lvalue).

But now it can work.
And you could see its use by reading the code above.
To make sure this assignment works correctly,
codegen will generate byte code by `codegen::call_gen()` instead of `codegen::mcall_gen()`,
and the last child of the ast will be generated by `codegen::mcall_gen()`.
So the bytecode is totally different now:

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

As you could see from the bytecode above,
`mcall`/`mcallv`/`mcallh` operands' using frequency will reduce,
`call`/`callv`/`callh`/`callfv`/`callfh` at the opposite.

And because of the new structure of `mcall`,
`addr_stack`, a stack used to store the memory address,
is deleted from `vm`,
and now `vm` use `nas_val** mem_addr` to store the memory address.
This will not cause fatal errors because the memory address is used __immediately__ after getting it.

### version 7.0 vm (last update 2021/10/8)

2021/6/26 update:

Instruction dispatch is changed from call-threading to computed-goto(with inline function).
After changing the way of instruction dispatch,
there is a great improvement in `vm`.
Now vm can run test/bigloop and test/pi in 0.2s!
And vm runs test/fib in 0.8s on linux.
You could see the time use data below,
in Test data section.

This version uses g++ extension "labels as values",
which is also supported by clang++.
(But i don't know if MSVC supports this)

There is also a change in `gc`:
`std::vector` global is deleted,
now the global values are all stored on stack(from `val_stack+0` to `val_stack+intg-1`).

2021/6/29 update:

Add some instructions that execute const values:
`op_addc`,`op_subc`,`op_mulc`,`op_divc`,`op_lnkc`,`op_addeqc`,`op_subeqc`,`op_muleqc`,`op_diveqc`,`op_lnkeqc`.

Now the bytecode of test/bigloop.nas seems like this:

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

And this test file runs in 0.1s after this update.
Most of the calculations are accelerated.

Also, assignment bytecode has changed a lot.
Now the first identifier that called in assignment will use `op_load` to assign,
instead of `op_meq`,`op_pop`.

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

In this version vm_nil and vm_num now is not managed by `gc`,
this will decrease the usage of `gc::alloc` and increase the efficiency of execution.

New value type is added: `vm_obj`.
This type is reserved for user to define their own value types.
Related API will be added in the future.

Fully functional closure:
Add new operands that get and set upvalues.
Delete an old operand `op_offset`.

2021/10/13 update:

The format of output information of bytecodes changes to this:

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

Delete `op_pone` and `op_pzero`.
Both of them are meaningless and will be replaced by `op_pnum`.

### version 9.0 vm (last update 2022/5/18)

2022/2/12 update:

Local values now are __stored on stack__.
So function calling will be faster than before.
Because in v8.0 when calling a function,
new `vm_vec` will be allocated by `gc`, this makes gc doing mark-sweep too many times and spends a quite lot of time.
In test file `test/bf.nas`, it takes too much time to test the file because this file has too many function calls(see test data below in table `version 8.0 (R9-5900HX ubuntu-WSL 2022/1/23)`).

Upvalue now is generated when creating first new function in the local scope, using `vm_vec`.
And after that when creating new functions, they share the same upvalue, and the upvalue will synchronize with the local scope each time creating a new function.

2022/3/27 update:

In this month's updates we change upvalue from `vm_vec` to `vm_upval`,
a special gc-managed object,
which has almost the same structure of that upvalue object in another programming language __`Lua`__.

Today we change the output format of bytecode.
New output format looks like `objdump`:

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

### version 10.0 vm (last update 2022/8/16)

2022/5/19 update:

Now we add coroutine in this runtime:

```javascript
var coroutine = {
    create:  func(function) { return __cocreate; },
    resume:  func(co)       { return __coresume; },
    yield:   func(args...)  { return __coyield;  },
    status:  func(co)       { return __costatus; },
    running: func()         { return __corun;    }
};
```

`coroutine.create` is used to create a new coroutine object using a function.
But this coroutine will not run immediately.

`coroutine.resume` is used to continue running a coroutine.

`coroutine.yield` is used to interrupt the running of a coroutine and throw some values.
These values will be accepted and returned by `coroutine.resume`.
And `coroutine.yield` it self returns `vm_nil` in the coroutine function.

`coroutine.status` is used to see the status of a coroutine.
There are 3 types of status:`suspended` means waiting for running,`running` means is running,`dead` means finished running.

`coroutine.running` is used to judge if there is a coroutine running now.

__CAUTION:__ coroutine should not be created or running inside another coroutine.

__We will explain how resume and yield work here:__

When `op_callb` is called, the stack frame is like this:

```C++
+----------------------+(main stack)
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

In `op_callb`'s progress, next step the stack frame is:

```C++
+----------------------+(main stack)
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

Then we call `resume`, this function will change stack.
As we can see, coroutine stack already has some values on it,
but if we first enter it, the stack top will be `vm_ret`, and the return `pc` is `0`.

So for safe running, when first calling the coroutine, `resume` will return `gc.top[0]`.
`op_callb` will do `top[0]=resume()`, so the value does not change.

```C++
+----------------------+(coroutine stack)
| pc:0(vm_ret)         | <- now gc.top[0]
+----------------------+
```

When we call `yield`, the function will do like this.
And we find that `op_callb` has put the `nil` at the top.
but where is the returned `local[1]` sent?

```C++
+----------------------+(coroutine stack)
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

When `builtin_coyield` is finished, the stack is set to main stack,
and the returned `local[1]` in fact is set to the top of the main stack by `op_callb`:

```C++
+----------------------+(main stack)
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

so the main progress feels the value on the top is the returned value of `resume`.
but in fact the `resume`'s returned value is set on coroutine stack.
so we conclude this:

```C++
resume (main->coroutine) return coroutine.top[0]. coroutine.top[0] = coroutine.top[0];
yield  (coroutine->main) return a vector.         main.top[0]      = vector;
```

## __Release Notes__

### __version 8.0 release__

I made a __big mistake__ in `v8.0` release:

in __`nasal_dbg.h:215`__: `auto canary=gc.stack+STACK_MAX_DEPTH-1;`

this will cause incorrect `stackoverflow` error.
please change it to:

`canary=gc.stack+STACK_MAX_DEPTH-1;`

If do not change this line, only the debugger runs abnormally. this bug is fixed in `v9.0`.

Another bug is that in `nasal_err.h:class nasal_err`, we should add a constructor for this class:

```C++
    nasal_err(): error(0) {}
```

This bug is fixed in `v9.0`. So we suggest that do not use `v8.0`.

### __version 11.0 release__

1. Use C++ `std=c++17`.

2. Change framework of ast, using visitor pattern.

3. New ast structure dump info format.

4. Change the way of module export, split library into different modules. Symbols begin with `_` will not be exported.

5. Change `stl` to `std`.

6. Add REPL interpreter.

7. Improve structure of virtual machine, split global symbol stack(stores global symbols' values) and value stack(using in process).

8. Delete operand `op_intg`, add operand `op_repl`.

9. Add `CMakeLists.txt` for cmake user(including `Visual Studio`).

10. New ghost type register process.

### __version 11.1 release__

1. Bug fix: debugger in v11.0 is malfunctional.

2. Bug fix: symbol_finder does not check definition in foreach/forindex loop.

3. Change extension syntax `import.xx.xx` to `use xx.xx`.
