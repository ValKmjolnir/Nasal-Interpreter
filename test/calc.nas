import("lib.nas");
var filename=["main.cpp","nasal_ast.h","nasal_builtin.h","nasal_codegen.h","nasal_gc.h","nasal_import.h","nasal_lexer.h","nasal_parse.h","nasal_vm.h","nasal.h"];
var space=["       ","    ","","","     "," ","  ","  ","     ","        "];
var enter_cnt=func(s)
{
    var (cnt,len,enter)=(0,size(s),'\n'[0]);
    for(var i=0;i<len;i+=1)
        cnt+=(s[i]==enter);
    return cnt;
}
var semic_cnt=func(s)
{
    var (cnt,len,semi)=(0,size(s),';'[0]);
    for(var i=0;i<len;i+=1)
        cnt+=(s[i]==semi);
    return cnt;
}
func(){
    var (bytes,line,semi)=(0,0,0);
    forindex(var i;filename)
    {
        var s=io.fin(filename[i]);
        bytes+=size(s);
        var line_cnt=enter_cnt(s);
        var semi_cnt=semic_cnt(s);
        println(filename[i],space[i],'| ',line_cnt,' \tline | ',semi_cnt,' \tsemi');
        line+=line_cnt;
        semi+=semi_cnt;
    }
    println('total:         | ',line,' \tline | ',semi,' \tsemi');
    println('bytes:         | ',bytes,' bytes| ',int(bytes/1024),' \tkb');
}();