import.stl.padding;
import.stl.process_bar;

var char_ttf=[
    ["    ","    ","    ","    ","    ","    "],
    [" █████╗ ","██╔══██╗","███████║","██╔══██║","██║  ██║","╚═╝  ╚═╝"],
    ["██████╗ ","██╔══██╗","██████╔╝","██╔══██╗","██████╔╝","╚═════╝ "],
    [" ██████╗","██╔════╝","██║     ","██║     ","╚██████╗"," ╚═════╝"],
    ["██████╗ ","██╔══██╗","██║  ██║","██║  ██║","██████╔╝","╚═════╝ "],
    ["███████╗","██╔════╝","█████╗  ","██╔══╝  ","███████╗","╚══════╝"],
    ["███████╗","██╔════╝","█████╗  ","██╔══╝  ","██║     ","╚═╝     "],
    [" ██████╗ ","██╔════╝ ","██║  ███╗","██║   ██║","╚██████╔╝"," ╚═════╝ "],
    ["██╗  ██╗","██║  ██║","███████║","██╔══██║","██║  ██║","╚═╝  ╚═╝"],
    ["██╗","██║","██║","██║","██║","╚═╝"],
    ["     ██╗","     ██║","     ██║","██   ██║","╚█████╔╝"," ╚════╝ "],
    ["██╗  ██╗","██║ ██╔╝","█████╔╝ ","██╔═██╗ ","██║  ██╗","╚═╝  ╚═╝"],
    ["██╗     ","██║     ","██║     ","██║     ","███████╗","╚══════╝"],
    ["███╗   ███╗","████╗ ████║","██╔████╔██║","██║╚██╔╝██║","██║ ╚═╝ ██║","╚═╝     ╚═╝"],
    ["███╗   ██╗","████╗  ██║","██╔██╗ ██║","██║╚██╗██║","██║ ╚████║","╚═╝  ╚═══╝"],
    [" ██████╗ ","██╔═══██╗","██║   ██║","██║   ██║","╚██████╔╝"," ╚═════╝ "],
    ["██████╗ ","██╔══██╗","██████╔╝","██╔═══╝ ","██║     ","╚═╝     "],
    [" ██████╗ ","██╔═══██╗","██║   ██║","██║▄▄ ██║","╚██████╔╝"," ╚══▀▀═╝ "],
    ["██████╗ ","██╔══██╗","██████╔╝","██╔══██╗","██║  ██║","╚═╝  ╚═╝"],
    ["███████╗","██╔════╝","███████╗","╚════██║","███████║","╚══════╝"],
    ["████████╗","╚══██╔══╝","   ██║   ","   ██║   ","   ██║   ","   ╚═╝   "],
    ["██╗   ██╗","██║   ██║","██║   ██║","██║   ██║","╚██████╔╝"," ╚═════╝ "],
    ["██╗   ██╗","██║   ██║","██║   ██║","╚██╗ ██╔╝"," ╚████╔╝ ","  ╚═══╝  "],
    ["██╗    ██╗","██║    ██║","██║ █╗ ██║","██║███╗██║","╚███╔███╔╝"," ╚══╝╚══╝ "],
    ["██╗  ██╗","╚██╗██╔╝"," ╚███╔╝ "," ██╔██╗ ","██╔╝╚██╗","╚═╝  ╚═╝"],
    ["██╗   ██╗","╚██╗ ██╔╝"," ╚████╔╝ ","  ╚██╔╝  ","   ██║   ","   ╚═╝   "],
    ["███████╗","╚══███╔╝","  ███╔╝ "," ███╔╝  ","███████╗","╚══════╝"],
];

var trans_ttf=func(string){
    var str=["","","","","",""];
    for(var i=0;i<size(string);i+=1){
        var number=string[i];
        if(97<=number and number<=122)
            for(var j=0;j<6;j+=1)
                str[j]~=char_ttf[number-96][j];
        elsif(65<=number and number<=90)
            for(var j=0;j<6;j+=1)
                str[j]~=char_ttf[number-64][j];
        else
            for(var j=0;j<6;j+=1)
                str[j]~=char_ttf[0][j];
    }
    foreach(var i;str)
        println(i);
    return;
}

var curve1=func(line=4){
    var table=["╚","═","╝","╔","║","╗"];
    rand(100);
    var s="";
    for(var i=0;i<line;i+=1){
        for(var j=0;j<90;j+=1)
            s~=table[int(6*rand())];
        s~='\n';
    }
    print(s);
}

var curve2=func(line=2){
    var shadow=["░","▒","▓","█","▀","▄","▐","▌"];
    rand(100);
    var s="";
    for(var i=0;i<line;i+=1){
        for(var j=0;j<90;j+=1)
            s~=shadow[int(8*rand())];
        s~='\n';
    }
    print(s);
}

var curve3=func(line=2){
    var arr=[
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8,
        0,1,2,3,4,5,6,7,8
    ];
    for(var loop=0;loop<line;loop+=1){
        for(var i=size(arr)-1;i>=0;i-=1){
            var rand_index=int(i*rand());
            (arr[i],arr[rand_index])=(arr[rand_index],arr[i]);
        }
        #            0   1   2   3   4   5   6   7   8
        var shadow=[" ","░","▒","▓","█","▀","▄","▐","▌"];
        var s="";
        for(var i=0;i<size(arr);i+=1)
            s~=shadow[arr[i]];
        println(s);
    }
    return;
}

var curve4=func(line=4){
    var shadow=["m░\e[0m","m▒\e[0m","m▓\e[0m","m█\e[0m","m▀\e[0m","m▄\e[0m","m▐\e[0m","m▌\e[0m"];
    var front=[
        "30","31","32","33","34","35","36","37",
        "90","91","92","93","94","95","96","97",
    ];
    var back=[
        "40","41","42","43","44","45","46","47",
        "100","101","102","103","104","105","106","107"
    ];
    rand(time(0));
    for(var i=0;i<line;i+=1){
        for(var j=0;j<90;j+=1)
            print("\e["~front[16*rand()]~";"~back[16*rand()]~shadow[8*rand()]);
        print('\n');
    }
}

var curve5=func(line=4){
    var vec=["▀▄─","▄▀─","▀─▄","▄─▀"];
    for(var (y,p)=(0,0);y!=line;y+=1){
        for(var x=0;x!=30;x+=1)
            print(vec[p]);
        print("\n");
        p+=1;
        p=p>=4?0:p;
    }
}

var ansi_escape_sequence=func(){
    # decoration
    for(var i=0;i<10;i+=1)
        print("\e["~i~"m",rightpad(i,4),"\e[0m");
    print("\n");

    # 8/16 color
    for(var i=30;i<38;i+=1)
        print("\e["~i~"m ",rightpad(i,4),"\e[0m");
    for(var i=30;i<38;i+=1)
        print("\e["~i~";1m ",rightpad(i,4),"\e[0m");
    print("\n");
    for(var i=40;i<48;i+=1)
        print("\e["~i~"m ",rightpad(i,4),"\e[0m");
    for(var i=40;i<48;i+=1)
        print("\e["~i~";1m ",rightpad(i,4),"\e[0m");
    print("\n");
    for(var i=90;i<98;i+=1)
        print("\e["~i~"m ",rightpad(i,4),"\e[0m");
    for(var i=90;i<98;i+=1)
        print("\e["~i~";1m ",rightpad(i,4),"\e[0m");
    print("\n");
    for(var i=100;i<108;i+=1)
        print("\e["~i~"m ",rightpad(i,4),"\e[0m");
    for(var i=100;i<108;i+=1)
        print("\e["~i~";1m ",rightpad(i,4),"\e[0m");
    print("\n");

    # 256 color
    for(var i=0;i<16;i+=1) {
        for(var j=0;j<16;j+=1) {
            var code=str(i*16+j);
            print("\e[38;5;"~code~"m ",rightpad(code,4),"\e[0m");
        }
        print("\n");
    }
    for(var i=0;i<16;i+=1) {
        for(var j=0;j<16;j+=1) {
            var code=str(i*16+j);
            print("\e[48;5;"~code~"m ",rightpad(code,4),"\e[0m");
        }
        print("\n");
    }

    # move curser left and up
    var bar=process_bar.default_bar("classic3",30);
    var progress=[0,0,0,0,0,0,0,0];
    var increase=[0.03,0.06,0.04,0.094,0.08,0.09,0.05,0.032];
    foreach(var i;progress) {
        print("\e[1000D",bar.bar(i)," ",rightpad(str(int(i*100)),3)," % \n");
    }
    for(var i=0;i<1/0.03;i+=1) {
        print("\e[1000D","\e["~str(size(progress))~"A");
        forindex(var j;progress) {
            progress[j]+=increase[j];
            progress[j]=progress[j]>1?1:progress[j];
            print("\e[1000D",bar.bar(progress[j])," ",rightpad(str(int(progress[j]*100)),3)," % \n")
        }
        unix.sleep(0.01);
    }

}

# enable unicode
if(os.platform()=="windows"){
    system("chcp 65001");
}

trans_ttf("just for test");
trans_ttf(" ValKmjolnir ");
curve1();
curve2();
curve3();
curve4();
curve5();
ansi_escape_sequence();
