var unicode测试=func(){
    var 输出=print;
    var 测试成功=[
        "unicode: utf-8支持测试成功",
        "目前仅支持utf-8以及ascii格式文件",
        "注意: windows系统请开启chcp 65001代码页"
    ];
    foreach(var 内容;测试成功)
        输出(内容~"\n");
}
var emoji测试=func(){
    var 💻=print;
    var 🎤="\n";
    var 🤣="🤣笑嘻了";
    var 😅="😅差不多得了";
    var 🤤="🤤收收味";
    var 🥵="🥵太烧啦";
    var 🥶="🥶捏麻麻滴冷死了";
    var 🤢="🤢老八秘制小汉堡🍔";
    var 🤓="🤓我是傻逼";
    var 😭="😭你带我走吧😭😭😭";
    var 👿="👿密麻麻石蜡";
    var 🤡="🤡居然就是你";
    var 💩="💩奥利给干了兄弟们";
    var 🍾="🍾好似，开🍾咯";
    var 🐘="🐘太🚬🐘了兄弟们";
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
    foreach(var 📄;📁)
        💻(📄,🎤);
    foreach(var 📄;keys(🗄️))
        💻(📄,🗄️[📄],🎤);
}
unicode测试();
emoji测试();