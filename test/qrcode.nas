use std.os;
var code=[
    [1,1,1,1,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,1,0,0,1,1,1,1,1,1,1],
    [1,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,1,1,1,0,1,0,0,0,0,0,1],
    [1,0,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,1,1,0,1],
    [1,0,1,1,1,0,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1],
    [1,0,1,1,1,0,1,0,0,1,0,1,0,1,1,0,1,0,1,1,0,0,1,0,1,1,1,0,1],
    [1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1],
    [1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1],
    [0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0],
    [1,1,0,1,1,0,1,0,0,1,1,1,1,0,1,1,1,1,0,1,0,0,1,0,0,0,0,0,1],
    [1,1,0,1,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,1,0,1,0,1,1,0,1,1,0],
    [1,0,1,1,0,1,1,1,0,0,1,1,1,1,0,1,1,0,1,0,1,0,0,1,1,0,1,0,0],
    [0,0,0,0,1,0,0,1,0,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1,0,1,0,0,1],
    [0,0,0,0,0,1,1,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,1],
    [1,1,0,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1],
    [1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,1,0,1,1,1,1,0,1,0,1,0,1],
    [0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,0,1],
    [0,1,0,0,0,0,1,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0],
    [1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0],
    [1,1,1,1,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,0,0,1],
    [1,1,1,1,1,1,0,0,0,1,1,0,0,1,0,1,1,0,1,1,0,0,1,0,0,1,1,0,0],
    [1,1,1,0,0,1,1,0,1,0,0,0,0,0,1,0,1,1,0,0,1,1,1,1,1,1,1,1,0],
    [0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0],
    [1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,0],
    [1,0,0,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,1,0,0,0,1,0,0,0,1],
    [1,0,1,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,1,1,1,1,1,1,1,0,1,1],
    [1,0,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,0,0,1,1],
    [1,0,1,1,1,0,1,0,0,0,0,0,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,1],
    [1,0,0,0,0,0,1,0,1,0,0,1,1,0,1,1,1,0,0,0,0,0,1,1,0,1,1,0,1],
    [1,1,1,1,1,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,1,0,1,0,1,0,0,0,0],
];

# enable unicode
if (os.platform()=="windows")
    system("chcp 65001");
var texture=["  ","██"];
for(var i=0;i<size(code);i+=1) {
    for(var j=0;j<size(code[i]);j+=1)
        print(texture[code[i][j]]);
    print('\n');
}