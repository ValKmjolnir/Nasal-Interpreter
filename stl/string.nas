# string.nas
# ValKmjolnir 2022/10/5

var join=func(vec){
    var res="";
    foreach(var i;vec)
        res~=i;
    return res;
}