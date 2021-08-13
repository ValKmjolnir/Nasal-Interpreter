# hexdump.nas by ValKmjolnir
# 2021/8/13

import("lib.nas");

# init
var hex_num=[
    '0','1','2','3',
    '4','5','6','7',
    '8','9','a','b',
    'c','d','e','f'
];
var hex=[];
foreach(var i;hex_num)
    foreach(var j;hex_num)
        append(hex,i~j);

# read file
var s=func()
{
    var filename=[
        "nasal.h",
        "main.cpp",
        "nasal_ast.h",
        "nasal_builtin.h",
        "nasal_codegen.h",
        "nasal_gc.h",
        "nasal_import.h",
        "nasal_lexer.h",
        "nasal_parse.h",
        "nasal_vm.h",
    ];
    var ret="";
    foreach(var elem;filename)
        ret~=io.fin(elem);
    return ret;
}();

# used to change line and control the spaces
var cnt=0;

# used to print hex index
var hex_index=[0,0,0,0];

# print binary in text format
var textprint=func(index)
{
    print(' |');
    for(var i=index-cnt;i<index;i+=1)
        print((0<=s[i] and s[i]<32)?'.':chr(s[i]));
    for(var i=cnt;i<16;i+=1)
        print('.');
    print('|\n');
}

# print index
var indexprint=func(index)
{
    forindex(var i;hex_index)
    {
        hex_index[i]=index-int(index/256)*256;
        index=int(index/256);
    }
    for(var i=3;i>=0;i-=1)
        print(hex[hex_index[i]]);
    print('  ');
    return;
}

# main
func()
{
    indexprint(0);
    for(var i=0;i<size(s);i+=1)
    {
        if(cnt==16){
            textprint(i);
            cnt=0;
            indexprint(i);
        }elsif(cnt==8){
            print(' ');
        }
        cnt+=1;
        print(hex[s[i]],' ');
    }
    for(var l=cnt;l<16;l+=1)
        print('   ');
    if(cnt<8)
        print(' ');
    textprint(i);
}();