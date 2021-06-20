import("lib.nas");
var filename=["main.cpp","nasal_ast.h","nasal_builtin.h","nasal_codegen.h","nasal_gc.h","nasal_import.h","nasal_lexer.h","nasal_parse.h","nasal_vm.h","nasal.h"];
var space=["       ","    ","","","     "," ","  ","  ","     ","        "];
func(){
    var (bytes,cnt,semi)=(0,0,0);
    forindex(var i;filename)
    {
        var s=io.fin(filename[i]);
        bytes+=size(s);
        var line_cnt=size(split('\n',s));
        var semi_cnt=size(split(';' ,s))-1;
        println(filename[i],space[i],'| ',line_cnt,' \tline | ',semi_cnt,' \tsemi');
        cnt +=line_cnt;
        semi+=semi_cnt;
    }
    println('total:         | ',cnt,' \tline | ',semi,' \tsemi');
    println('bytes:         | ',bytes,' bytes| ',int(bytes/1024),' \tkb');
}();