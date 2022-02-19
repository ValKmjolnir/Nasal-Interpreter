import("lib.nas");
import("module/libkey.nas");

var brick=[
    [
        [1,1],
        [1,0],
        [1,0]
    ],
    [
        [0,1],
        [1,1],
        [1,0]
    ],
    [
        [1,1],
        [0,1],
        [0,1],
    ],
    [
        [1,0],
        [1,1],
        [0,1]
    ],
    [
        [1,1,1,1]
    ],
    [
        [1,1],
        [1,1]
    ],
    [
        [0,1,0],
        [1,1,1]
    ]
];

func(){
    libkey.init();
    print('\ec');
    var cnt=0;
    while(1){
        var ch=libkey.nonblock();
        ch=ch?chr(ch)~chr(ch):"[]";
        var s="\ec+--------------+\n";
        var normal="|              |\n";
        var character="|      "~ch~"      |\n";
        for(var i=0;i<6;i+=1)
            s~=(i==cnt)?character:normal;
        s~="+--------------+\n";
        print(s);
        unix.sleep(0.3);
        cnt+=1;
        if(cnt==6)
            cnt=0;
    }
    libkey.close();
}();