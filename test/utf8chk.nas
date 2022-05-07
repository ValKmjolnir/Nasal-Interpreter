var 输出=print;
var 这是unicode测试=func(){
    var 测试成功=[
        "unicode: utf-8支持测试成功",
        "目前仅支持utf-8以及ascii格式文件",
        "注意: windows系统请开启chcp 65001代码页"
    ];
    foreach(var 内容;测试成功)
        输出(内容~"\n");
}
这是unicode测试();