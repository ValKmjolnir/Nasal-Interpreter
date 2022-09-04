# padding.nas
# ValKmjolnir 2022/9/4

var leftpad=func(str,len,char=" "){
    var strlen=size(str);
    for(var i=strlen;i<len;i+=1)
        str=char~str;
    return str;
}

var rightpad=func(str,len,char=" "){
    var strlen=size(str);
    for(var i=strlen;i<len;i+=1)
        str~=char;
    return str;
}