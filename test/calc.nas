import("lib.nas");
var source=[
    "main.cpp              ",
    "nasal_err.h           ",
    "nasal_ast.h           ",
    "nasal_builtin.h       ",
    "nasal_codegen.h       ",
    "nasal_opt.h           ",
    "nasal_gc.h            ",
    "nasal_import.h        ",
    "nasal_lexer.h         ",
    "nasal_parse.h         ",
    "nasal_vm.h            ",
    "nasal_dbg.h           ",
    "nasal.h               "
];

var lib=[
    "stl/lib.nas           ",
    "stl/list.nas          ",
    "stl/queue.nas         ",
    "stl/result.nas        ",
    "stl/sort.nas          ",
    "stl/stack.nas         "
];

var testfile=[
    "test/ascii-art.nas    ",
    "test/auto_crash.nas   ",
    "test/bf.nas           ",
    "test/bfconvertor.nas  ",
    "test/bfs.nas          ",
    "test/bigloop.nas      ",
    "test/bp.nas           ",
    "test/calc.nas         ",
    "test/choice.nas       ",
    "test/class.nas        ",
    "test/exception.nas    ",
    "test/fib.nas          ",
    "test/filesystem.nas   ",
    "test/hexdump.nas      ",
    "test/json.nas         ",
    "test/leetcode1319.nas ",
    "test/lexer.nas        ",
    "test/life.nas         ",
    "test/loop.nas         ",
    "test/mandel.nas       ",
    "test/mandelbrot.nas   ",
    "test/module_test.nas  ",
    "test/nasal_test.nas   ",
    "test/pi.nas           ",
    "test/prime.nas        ",
    "test/props.nas        ",
    "test/quick_sort.nas   ",
    "test/scalar.nas       ",
    "test/trait.nas        ",
    "test/turingmachine.nas",
    "test/ycombinator.nas  "
];

var module=[
    "module/fib.cpp        "
];

var getname=func(s){
    var (len,ch)=(size(s),' '[0]);
    for(var i=0;i<len and s[i]!=ch;i+=1);
    return substr(s,0,i);
}

var count=func(s,c){
    var (cnt,len,ch)=(0,size(s),c[0]);
    for(var i=0;i<len;i+=1)
        cnt+=(s[i]==ch);
    return cnt;
}

var calc=func(codetype,files){
    println(codetype);
    var (bytes,line,semi,line_cnt,semi_cnt)=(0,0,0,0,0);
    forindex(var i;files){
        var s=io.fin(getname(files[i]));
        (line_cnt,semi_cnt)=(count(s,'\n'),count(s,';'));
        println(files[i],'| ',line_cnt,' \tline | ',semi_cnt,' \tsemi');
        bytes+=size(s);
        line+=line_cnt;
        semi+=semi_cnt;
    }
    println('total:                | ',line,' \tline | ',semi,' \tsemi');
    println('                      | ',bytes,'\tbytes| ',int(bytes/1024),' \tkb');
}

calc("source code:",source);
calc("lib:",lib);
calc("test file:",testfile);
calc("module:",module);