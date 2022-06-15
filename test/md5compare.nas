import("test/md5.nas");
import("stl/process_bar.nas");
srand();

var compare=func(){
    var ch=[
        "0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","+",
        "_","*","/","\'","\"",".",",",";",":","<",">","!","@","#","$","%",
        "^","&","*","(",")","-","=","\\","|","[","]","{","}","`"," ","\t","?"
    ];
    return func(begin,end){
        var total=end-begin;
        var bar=process_bar.bar(front:os.platform()=="windows"?"sharp":"block",back:"point",sep:"line",length:50);
        for(var i=begin;i<end;i+=1){
            var s="";
            for(var j=0;j<i;j+=1){
                s~=ch[rand()*size(ch)];
            }
            var res=md5(s);
            if(cmp(res,_md5(s))){
                die("error: "~str(i));
            }
            print(" ",bar.bar((i-begin+1)/total)," (",i-begin+1,"/",total,")\t",res," max byte: ",end-1,"    \r");
        }
        print('\n');
    };
}();

var filechecksum=func(){
    var getname=func(s){
        var (len,ch)=(size(s),' '[0]);
        for(var i=0;i<len and s[i]!=ch;i+=1);
        return substr(s,0,i);
    }
    var files=[
        "./stl/fg_env.nas        ",
        "./stl/file.nas          ",
        "./stl/lib.nas           ",
        "./stl/list.nas          ",
        "./stl/log.nas           ",
        "./stl/module.nas        ",
        "./stl/process_bar.nas   ",
        "./stl/queue.nas         ",
        "./stl/result.nas        ",
        "./stl/sort.nas          ",
        "./stl/stack.nas         ",
        "./test/ascii-art.nas    ",
        "./test/auto_crash.nas   ",
        "./test/bf.nas           ",
        "./test/bfcolored.nas    ",
        "./test/bfconvertor.nas  ",
        "./test/bfs.nas          ",
        "./test/bigloop.nas      ",
        "./test/bp.nas           ",
        "./test/calc.nas         ",
        "./test/choice.nas       ",
        "./test/class.nas        ",
        "./test/coroutine.nas    ",
        "./test/diff.nas         ",
        "./test/exception.nas    ",
        "./test/fib.nas          ",
        "./test/filesystem.nas   ",
        "./test/hexdump.nas      ",
        "./test/httptest.nas     ",
        "./test/json.nas         ",
        "./test/leetcode1319.nas ",
        "./test/lexer.nas        ",
        "./test/life.nas         ",
        "./test/loop.nas         ",
        "./test/mandel.nas       ",
        "./test/mandelbrot.nas   ",
        "./test/md5.nas          ",
        "./test/md5compare.nas   ",
        "./test/module_test.nas  ",
        "./test/nasal_test.nas   ",
        "./test/occupation.nas   ",
        "./test/pi.nas           ",
        "./test/prime.nas        ",
        "./test/qrcode.nas       ",
        "./test/quick_sort.nas   ",
        "./test/scalar.nas       ",
        "./test/snake.nas        ",
        "./test/tetris.nas       ",
        "./test/trait.nas        ",
        "./test/turingmachine.nas",
        "./test/utf8chk.nas      ",
        "./test/wavecollapse.nas ",
        "./test/ycombinator.nas  ",
        "LICENSE                 ",
        "main.cpp                ",
        "makefile                ",
        "nasal_ast.h             ",
        "nasal_builtin.h         ",
        "nasal_codegen.h         ",
        "nasal_dbg.h             ",
        "nasal_err.h             ",
        "nasal_gc.h              ",
        "nasal_import.h          ",
        "nasal_lexer.h           ",
        "nasal_opt.h             ",
        "nasal_parse.h           ",
        "nasal_vm.h              ",
        "nasal.ebnf              ",
        "nasal.h                 ",
        "README.md               "
    ];
    foreach(var i;files){
        var f=io.fin(getname(i));
        var (res0,res1)=(md5(f),_md5(f));
        println(i,'   ',res0,'   ',!cmp(res0,res1),'   ',size(f),' byte');
    }
}

var randomchecksum=func(){
    for(var i=0;i<4096;i+=512)
        compare(i,i+512);
}

filechecksum();
randomchecksum();