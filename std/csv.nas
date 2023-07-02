# lib csv.nas
# ValKmjolnir 2022/10/15
var read_csv=func(path,delimeter=",",endline="\n"){
    var context=io.readfile(path);
    context=split(endline,context);
    forindex(var i;context){
        context[i]=split(delimeter,context[i]);
    }
    if(size(context)<=1){
        die("incorrect csv file <"~path~">: "~size(context)~" line(s).");
    }
    return {
        property:context[0],
        data:context[1:]
    };
}