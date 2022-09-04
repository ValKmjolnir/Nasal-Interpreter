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
    "lib.nas",
    "list.nas",
    "log.nas",
    "module.nas",
    "padding.nas",
    "process_bar.nas",
    "queue.nas",
    "result.nas",
    "sort.nas",
    "stack.nas"
];

var testfile=[
    "ascii-art.nas",
    "auto_crash.nas",
    "bf.nas",
    "bfcolored.nas",
    "bfconvertor.nas",
    "bfs.nas",
    "bigloop.nas",
    "bp.nas",
    "calc.nas",
    "choice.nas",
    "class.nas",
    "coroutine.nas",
    "diff.nas",
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
    "ycombinator.nas"
];

var module=[
    "fib.cpp",
    "keyboard.cpp",
    "nasocket.cpp",
    "libfib.nas",
    "libkey.nas",
    "libsock.nas"
];

var longest=func(vec...){
    var len=0;
    foreach(var v;vec)
        foreach(var f;v)
            len=size(f)>len?size(f):len;
    return len;
}

var getname=func(s){
    var (len,ch)=(size(s),' '[0]);
    for(var i=0;i<len and s[i]!=ch;i+=1);
    return substr(s,0,i);
}

var count=func(s,c){
    var cnt=0;
    foreach(var i;split(c,s))
        cnt+=(size(i)!=0 and i!="\r")
    return cnt;
}

var calc=func(codetype,files,path=""){
    println(codetype);
    var (bytes,line,semi,line_cnt,semi_cnt)=(0,0,0,0,0);
    var padding_length=longest(source,lib,testfile,module);
    forindex(var i;files){
        var s=io.fin(getname(path~files[i]));
        (line_cnt,semi_cnt)=(count(s,'\n'),count(s,';'));
        println(rightpad(files[i],padding_length),
            '| ',line_cnt,'\tline | ',semi_cnt,' \tsemi');
        bytes+=size(s);
        line+=line_cnt;
        semi+=semi_cnt;
    }
    println('total:           | ',line,'\tline | ',semi,' \tsemi');
    println('                 | ',int(bytes/1024),'\tkb');
    return int(bytes/1024);
}

var all=calc("source code:",source)
    +calc("lib:",lib,"stl/")
    +calc("test file:",testfile,"test/")
    +calc("module:",module,"module/");
println('\ntotal:           | ',all,'\tkb');