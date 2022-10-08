import.stl.json;

var ss=JSON.stringify({
    vec:[0,1,2],
    hash:{
        m1:0,
        m2:"str",
        m3:[114514],
        m4:{year:1919,month:8,date:10}
    },
    emptyhash:{},
    emptyvec:[],
    empty:[{}],
    empty_an:[[[[[[{}]]]]]],
    function:func(){}
});
println(ss);
println(JSON.parse(ss));