
var trait={
    get:func{return me.val;},
    set:func(x){me.val=x;}
};

var class={
    new:func(){
        return {
            val:nil,
            parents:[trait]
        };
    }
};

var class2={
    new:func(){
        return {
            val:nil,
            parents:[trait],
            set:func(x){me.val=typeof(x);}
        };
    }
};

var class_obj=[];
for(var i=0;i<10;i+=1){
    append(class_obj,class.new());
    class_obj[i].set(i);
}
for(var i=0;i<10;i+=1){
    append(class_obj,class2.new());
    class_obj[10+i].set(i);
}

foreach(var object;class_obj){
    println(object.get(),' ',keys(object));
}