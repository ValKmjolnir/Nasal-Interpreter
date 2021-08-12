import("lib.nas");

var hex_num=[
    '0','1','2','3',
    '4','5','6','7',
    '8','9','a','b',
    'c','d','e','f'
];
var hex=[];
foreach(var i;hex_num)
    foreach(var j;hex_num)
        append(hex,'0x'~i~j);
var s=io.fin("lib.nas");

var cnt=0;
print(0,'\t');
for(var i=0;i<size(s);i+=1){
    if(cnt==8){
        cnt=0;
        print('\n',i,'\t');
    }
    cnt+=1;
    print(hex[s[i]],' ');
}
print('\n');