use std.json;
use module.libjson;
use std.process_bar;

var ss = json.stringify({
    vec:[0,1,2],
    hash:{
        m1:0,
        m2:"str\\\"test\\\"",
        m3:[114514],
        m4:{year:1919,month:8,date:10}
    },
    emptyhash:{},
    emptyvec:[],
    empty:[{}],
    empty_an:[[[[[[{}]]]]]],
    function:func() {}
});

println(ss, "\n");
println(json.parse(ss), "\n");
println(libjson.parse(ss), "\n");

var ss = json.stringify([{
    vec:[0,1,2,3],
    hash:{
        m1:0,
        m2:"str\\\"test\\\"",
        m3:[114514,1919810],
        m4:{year:1919,month:8,date:10}
    },
    emptyhash:{},
    emptyvec:[],
    empty_an:[[[[[{}]]]]],
    function:func() {}
}]);

println(ss, "\n");
println(json.parse(ss), "\n");
println(libjson.parse(ss), "\n");

var test_json = func(json) {
    var bar = process_bar.high_resolution_bar(30);
    var tmp = [
        {t0:nil},
        {t1:nil},
        {t2:nil},
        {t3:nil},
        {t4:nil},
        {t5:nil},
        {t6:nil},
        {t7:nil}
    ];

    srand();
    foreach(var hash; tmp) {
        var name = keys(hash)[0];
        hash[name] = [];
        print("\e[1000D", bar.bar(0));
        for(var i = 0; i<500; i+=1) {
            append(hash[name], {id:i, content:int(rand()*1e7)});
            print("\e[1000D", bar.bar((i+1)/500));
        }
        print("\e[1000D", bar.bar(1), " executing...\n");
    }
    print("\e[1000D", "\e["~str(size(tmp))~"A");
    foreach(var hash; json.parse(json.stringify(tmp))) {
        println("\e[1000D", bar.bar(1), " parse done ", keys(hash)[0], " ", size(hash[keys(hash)[0]]));
    }
};

var stamp = maketimestamp();
test_json(libjson);
println("time usage: ", stamp.elapsedUSec()/1000, " ms");
stamp.stamp();
test_json(json);
println("time usage: ", stamp.elapsedUSec()/1000, " ms");
