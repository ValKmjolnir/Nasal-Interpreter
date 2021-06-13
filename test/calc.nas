import("lib.nas");
func(){
    var filename=[
        "main.cpp",
        "nasal_ast.h",
        "nasal_builtin.h",
        "nasal_codegen.h",
        "nasal_gc.h",
        "nasal_import.h",
        "nasal_lexer.h",
        "nasal_parse.h",
        "nasal_vm.h",
        "nasal.h"
    ];
    var max_size=size('nasal_codegen.h');
    var (cnt,semi)=[0,0];
    foreach(var file;filename)
    {
        var s=io.fin(file);
        var name=file;
        for(var i=size(name);i<max_size;i+=1)
            name~=' ';
        var line_cnt=size(split('\n',s));
        var semi_cnt=size(split(';' ,s))-1;
        println(name,'| ',line_cnt,' \tline | ',semi_cnt,' \tsemi');
        cnt +=line_cnt;
        semi+=semi_cnt;
    }
    println('total:         | ',cnt,' \tline | ',semi,' \tsemi');
}();