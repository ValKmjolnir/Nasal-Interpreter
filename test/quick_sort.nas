import.stl.sort;
var vec=[];
rand(time(0));
for(var i=0;i<1e4;i+=1)
    append(vec,int(rand()*1e5));
sort(vec);
println(vec);