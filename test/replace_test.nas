use std.string;

var test_set = [
    ["{}", "{}", "a", "a"],
    ["{}", "a{}", "a", "aa"],
    ["{}", "{}a", "a", "aa"],
    ["{}", "a{}a", "a", "aaa"],
    ["{}", "{}a{}", "a", "aaa"],
    ["{}", "{{}}", "a", "{a}"],
    ["{}", "{}{}{}", "a", "aaa"]
];

foreach(var i; test_set) {
    if (string.replace(i[0], i[1], i[2]) != i[3]) {
        println("Error: string.replace(",
                i[0], ", ",
                i[1], ", ",
                i[2], ") != ",
                i[3]);
        exit(-1);
    }
}