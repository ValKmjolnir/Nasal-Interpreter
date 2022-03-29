import("lib.nas");
import("module/libmd5.nas");
import("test/md5.nas");

var ch="0123456789abcdef+_*/\'\".,;:<>!@#$%^&*()-=\\|[]{}";

for(var i=1;i<1024;i+=1){
    var s="";
    for(var j=0;j<i;j+=1){
        s~=chr(ch[int(rand()*size(ch))]);
    }
    var res=!cmp(md5(s),_md5(s));
    if(!res){
        println("error: ",i);
    }
}