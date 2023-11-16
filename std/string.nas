# string.nas
# ValKmjolnir 2022/10/5

var join = func(sep, vec) {
    var len = size(vec);
    var res = "";
    for(var i = 0; i<len; i += 1) {
        res ~= vec[i];
        res ~= (i==len-1? "":sep);
    }
    return res;
}