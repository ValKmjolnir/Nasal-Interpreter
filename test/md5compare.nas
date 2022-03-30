import("lib.nas");
import("module/libmd5.nas");
import("test/md5.nas");

rand(time(0));

var compare=func(total){
    var ch=[
        "0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","+",
        "_","*","/","\'","\"",".",",",";",":","<",">","!","@","#","$","%",
        "^","&","*","(",")","-","=","\\","|","[","]","{","}","`"," ","\t","?"
    ];
    var (prt,lastpercent,percent)=("",0,0);
    for(var i=1;i<=total;i+=1){
        var s="";
        for(var j=0;j<i;j+=1){
            s~=ch[int(rand()*size(ch))];
        }
        if(cmp(md5(s),_md5(s))){
            die("error: "~str(i));
        }
        percent=int(i/total*100);
        if(percent-lastpercent>=2){
            prt~="#";
            lastpercent=percent;
        }
        var tmp=prt;
        for(var spc=size(prt);spc<50;spc+=1)
            tmp~=" ";
        print(" |",tmp,"| ",percent,"% (",i,"/",total,")\r");
    }
    print('\n');
}

var filechecksum=func(){
    var getname=func(s){
        var (len,ch)=(size(s),' '[0]);
        for(var i=0;i<len and s[i]!=ch;i+=1);
        return substr(s,0,i);
    }
    var filewithoututf8=[
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
        "./test/exception.nas    ",
        "./test/fib.nas          ",
        "./test/filesystem.nas   ",
        "./test/hexdump.nas      ",
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
        "./test/pi.nas           ",
        "./test/prime.nas        ",
        "./test/props_sim.nas    ",
        "./test/props.nas        ",
        "./test/quick_sort.nas   ",
        "./test/scalar.nas       ",
        "./test/trait.nas        ",
        "./test/turingmachine.nas",
        "./test/ycombinator.nas  "
    ];
    foreach(var i;filewithoututf8){
        var f=io.fin(getname(i));
        var (res0,res1)=(md5(f),_md5(f));
        println(i,'   ',res0,'   ',!cmp(res0,res1),'   ',size(f),' byte');
    }
}

var randomchecksum=func(){
    for(var i=0;i<8;i+=1)
        compare(512);
    for(var i=0;i<4;i+=1)
        compare(1024);
    for(var i=0;i<2;i+=1)
        compare(2048);
    compare(4096);
}

filechecksum();
randomchecksum();