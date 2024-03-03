# regex.nas
# 2024/3/1 by ValKmjolnir

var match = func(src, reg) {
    return __regex_match(src, reg);
}

var search = func(src, reg) {
    return __regex_search(src, reg);
}

var replace = func(src, reg, fmt) {
    return __regex_replace(src, reg, fmt);
}

var match_all = func(src, reg) {
    return __regex_match_all(src, reg);
}