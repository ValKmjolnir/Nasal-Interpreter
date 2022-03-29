import("lib.nas");
import("module/libmd5.nas");
import("test/md5.nas");

rand(time(0));

var compare=func(total){
    var ch="0123456789abcdef+_*/\'\".,;:<>!@#$%^&*()-=\\|[]{}";
    var (prt,lastpercent,percent)=("",0,0);
    for(var i=1;i<=total;i+=1){
        var s="";
        for(var j=0;j<i;j+=1){
            s~=chr(ch[int(rand()*size(ch))]);
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

compare(512);
compare(1024);
compare(2048);