use std.runtime;

# do nothing if platform is not windows
runtime.windows.set_utf8_output();

var unicode测试 = func() {
    var 输出=print;
    var 测试成功=[
        "unicode: utf-8 支持测试成功",
        "目前仅支持 utf-8 以及 ascii 格式文件",
        "注意: windows 系统请开启 chcp 65001 代码页"
    ];
    foreach (var 内容; 测试成功)
        输出(内容~"\n");
}

var emoji测试 = func() {
    var 💻=print;
    var 🎤="\n";
    var 🤣="🤣 笑嘻了";
    var 😅="😅 差不多得了";
    var 🤤="🤤 收收味";
    var 🥵="🥵 太烧啦";
    var 🥶="🥶 我草！冰！";
    var 🤢="🤢 老八秘制小汉堡 🍔";
    var 🤓="🤓 我是傻逼";
    var 😭="😭 你带我走吧 😭😭😭";
    var 👿="👿 密麻麻石蜡";
    var 🤡="🤡 居然就是你";
    var 💩="💩 奥利给干了兄弟们";
    var 🍾="🍾 好似，开 🍾 咯";
    var 🐘="🐘 太 🚬🐘 了兄弟们";
    var 📁=[🤣,😅,🤤,🥵,🥶,🤢,🤓,😭,👿,🤡,💩,🍾,🐘];
    var 🗄️={
        🤣:🤣,
        😅:😅,
        🤤:🤤,
        🥵:🥵,
        🥶:🥶,
        🤢:🤢,
        🤓:🤓,
        😭:😭,
        👿:👿,
        🤡:🤡,
        💩:💩,
        🍾:🍾,
        🐘:🐘
    };
    foreach (var 📄; 📁)
        💻(📄, 🎤);
    foreach (var 📄; keys(🗄️))
        💻(📄, 🗄️[📄], 🎤);
}

var dotsgen = func() {
    var dots=[];
    var s="⠀";
    for (var i=0;i<256;i+=1) {
        var v0=s[0];
        var v1=s[1]+int(i/64);
        var v2=s[2]+i;
        if (v2>191) {
            v2-=int(i/64)*64;
        }
        var tmp=char(v0)~char(v1)~char(v2);
        append(dots,tmp);
    }

    forindex (var i;dots) {
        print(dots[i],(i+1-int((i+1)/32)*32==0)?"\n":"");
    }
    return dots;
};

unicode测试();
emoji测试();
dotsgen();