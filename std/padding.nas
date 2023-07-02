# padding.nas
# ValKmjolnir 2022/9/4

var leftpad=func(s,len,char=" "){
    if(typeof(s)=="num")
        s=str(s);
    var strlen=size(s);
    for(var i=strlen;i<len;i+=1)
        s=char~s;
    return s;
}

var rightpad=func(s,len,char=" "){
    if(typeof(s)=="num")
        s=str(s);
    var strlen=size(s);
    for(var i=strlen;i<len;i+=1)
        s~=char;
    return s;
}