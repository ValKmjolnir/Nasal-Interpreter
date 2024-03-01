use std.regex;

println(regex.match("aaa", "[a]*"));
println(regex.search("aabcaa", "abc"));

var s = "aaaaa";
println(regex.replace(s, "a", "[$&]"));
println(s);

println(regex.match_all("a,b,c,d,e,f,g,h,i", "[a-z]"));