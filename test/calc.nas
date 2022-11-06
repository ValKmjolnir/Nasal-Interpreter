import.stl.padding;

var source=[
    "main.cpp",
    "nasal_ast.h",
    "nasal_builtin.h",
    "nasal_codegen.h",
    "nasal_dbg.h",
    "nasal_err.h",
    "nasal_gc.h",
    "nasal_import.h",
    "nasal_lexer.h",
    "nasal_opt.h",
    "nasal_parse.h",
    "nasal_vm.h",
    "nasal.h"
];

var lib=[
    "fg_env.nas",
    "file.nas",
    "json.nas",
    "lib.nas",
    "list.nas",
    "log.nas",
    "module.nas",
    "padding.nas",
    "process_bar.nas",
    "queue.nas",
    "result.nas",
    "sort.nas",
    "stack.nas",
    "string.nas"
];

var testfile=[
    "ascii-art.nas",
    "auto_crash.nas",
    "bf.nas",
    "bfconvertor.nas",
    "bfs.nas",
    "bigloop.nas",
    "bp.nas",
    "calc.nas",
    "choice.nas",
    "class.nas",
    "console3D.nas",
    "coroutine.nas",
    "diff.nas",
    "donuts.nas",
    "exception.nas",
    "fib.nas",
    "filesystem.nas",
    "hexdump.nas",
    "httptest.nas",
    "json.nas",
    "leetcode1319.nas",
    "lexer.nas",
    "life.nas",
    "loop.nas",
    "mandel.nas",
    "mandelbrot.nas",
    "mcpu.nas",
    "md5.nas",
    "md5compare.nas",
    "module_test.nas",
    "nasal_test.nas",
    "occupation.nas",
    "pi.nas",
    "ppmgen.nas",
    "prime.nas",
    "qrcode.nas",
    "quick_sort.nas",
    "scalar.nas",
    "snake.nas",
    "tetris.nas",
    "trait.nas",
    "turingmachine.nas",
    "utf8chk.nas",
    "watchdog.nas",
    "wavecollapse.nas",
    "word_collector.nas",
    "ycombinator.nas"
];

var module=[
    "fib.cpp",
    "keyboard.cpp",
    "matrix.cpp",
    "nasocket.cpp",
    "libfib.nas",
    "libkey.nas",
    "libmat.nas",
    "libsock.nas"
];

var longest=func(vec...){
    var len=0;
    foreach(var v;vec)
        foreach(var f;v)
            len=size(f)>len?size(f):len;
    return len;
}
var padding_length=longest(source,lib,testfile,module);

var count=func(s,c){
    var cnt=0;
    foreach(var i;split(c,s))
        cnt+=(size(i)!=0 and i!="\r")
    return cnt;
}

var column=func(number){
    number=number>=1000?substr(str(number/1000),0,3)~'k':str(number);
    return rightpad(number,6);
}

var calc=func(codetype,files,path=""){
    println(codetype);
    var (bytes,ctx,line,semi,line_cnt,semi_cnt)=(0,"",0,0,0,0);
    forindex(var i;files){
        var s=io.exists(path~files[i])?io.fin(path~files[i]):"";
        (line_cnt,semi_cnt)=(count(s,'\n'),count(s,';'));
        println(rightpad(files[i],padding_length),'| ',
            column(line_cnt),'line | ',
            column(semi_cnt),'semi | ',
            rightpad(str(int(size(s)/1024)),6),'kb | ',
            md5(s),' |');
        bytes+=size(s);
        ctx~=s;
        line+=line_cnt;
        semi+=semi_cnt;
    }
    println(rightpad("total:",padding_length),'| ',
        column(line),'line | ',
        column(semi),'semi | ',
        rightpad(str(int(bytes/1024)),6),'kb | ',
        md5(ctx),' |\n');
    return int(bytes/1024);
}

var all=calc("source code:",source)
    +calc("lib:",lib,"stl/")
    +calc("test file:",testfile,"test/")
    +calc("module:",module,"module/");
println(rightpad("total:",padding_length),'| ',rightpad(str(all),6),'kb');