import.test.md5;
import.stl.process_bar;
import.stl.file;
srand();

var compare=func() {
    var ch=[
        "0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","+",
        "_","*","/","\'","\"",".",",",";",":","<",">","!","@","#","$","%",
        "^","&","*","(",")","-","=","\\","|","[","]","{","}","`"," ","\t","?"
    ];
    return func(begin,end) {
        var byte=0;
        var total=end-begin;
        var timestamp=maketimestamp();
        timestamp.stamp();
        var bar=process_bar.high_resolution_bar(40);
        for(var i=begin;i<end;i+=1) {
            var s="";
            for(var j=0;j<i;j+=1) {
                s~=ch[rand()*size(ch)];
            }
            byte+=size(s);
            var res=md5(s);
            if(cmp(res,_md5(s))) {
                die("error: "~str(i));
            }
            if (i-begin-int((i-begin)/4)*4==0) {
                print(
                    "\e[1000D ",bar.bar((i-begin+1)/total),
                    " (",i-begin+1,"/",total,")\t",
                    res," byte: ",int(byte/1024),"k"
                );
            }
        }
        print(
            "\e[1000D ",bar.bar((i-begin)/total),
            " (",i-begin,"/",total,")\t",
            res," byte: ",int(byte/1024),"k",
            " time: ",timestamp.elapsedMSec()
        );
        print("\n");
    };
}();

var filechecksum=func(){
    var files=[];
    foreach(var p;find_all_files_with_extension("./test","nas")) {
        append(files,"./test/"~p);
    }
    foreach(var p;find_all_files_with_extension("./stl","nas")) {
        append(files,"./stl/"~p);
    }
    foreach(var p;find_all_files_with_extension("./module","nas","cpp")) {
        append(files,"./module/"~p);
    }
    foreach(var p;find_all_files_with_extension(".","cpp","h","md")) {
        append(files,"./"~p);
    }
    foreach(var p;find_all_files_with_extension("./doc","md")) {
        append(files,"./doc/"~p);
    }

    var byte=0;
    var total=size(files);
    var timestamp=maketimestamp();
    timestamp.stamp();
    var bar=process_bar.high_resolution_bar(40);
    forindex(var i;files){
        var f=io.readfile(files[i]);
        var res=md5(f);
        byte+=size(f);
        if(cmp(res,_md5(f))){
            die("error: "~files[i]);
        }
        print(
            "\e[1000D ",bar.bar((i+1)/total),
            " (",i+1,"/",total,")\t",res,
            " byte: ",int(byte/1024),"k",
            " time: ",timestamp.elapsedMSec()
        );
    }
    print("\n");
}

var randomchecksum=func(){
    for(var i=0;i<2048;i+=256)
        compare(i,i+256);
}

if(os.platform()=="windows")
    system("chcp 65001");
filechecksum();
randomchecksum();