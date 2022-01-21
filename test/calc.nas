import("lib.nas");
var filename=[
    "main.cpp",
    "nasal_err.h",
    "nasal_ast.h",
    "nasal_builtin.h",
    "nasal_codegen.h",
    "nasal_opt.h",
    "nasal_gc.h",
    "nasal_import.h",
    "nasal_lexer.h",
    "nasal_parse.h",
    "nasal_vm.h",
    "nasal_dbg.h",
    "nasal.h"
];
var space=[
    "       ",
    "    ",
    "    ",
    "",
    "",
    "    ",
    "     ",
    " ",
    "  ",
    "  ",
    "     ",
    "    ",
    "        "
];
var count=func(s,c){
    var (cnt,len,ch)=(0,size(s),c[0]);
    for(var i=0;i<len;i+=1)
        cnt+=(s[i]==ch);
    return cnt;
}
func(){
    var (bytes,line,semi)=(0,0,0);
    forindex(var i;filename){
        var s=io.fin(filename[i]);
        var (line_cnt,semi_cnt)=(count(s,'\n'),count(s,';'));
        println(filename[i],space[i],'| ',line_cnt,' \tline | ',semi_cnt,' \tsemi');
        bytes+=size(s);
        line+=line_cnt;
        semi+=semi_cnt;
    }
    println('total:         | ',line,' \tline | ',semi,' \tsemi');
    println('bytes:         | ',bytes,' bytes| ',int(bytes/1024),' \tkb');
}();