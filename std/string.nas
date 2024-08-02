# string.nas
# ValKmjolnir 2022/10/5

var join = func(sep, vec) {
    var len = size(vec);
    var res = "";
    for(var i = 0; i<len; i += 1) {
        res ~= vec[i];
        res ~= (i==len-1? "":sep);
    }
    return res;
}

var __temp_char = func(number) {
    return __char(number);
}

var __temp_append = func(vec, elem...) {
    return __append(vec, elem);
}

var __temp_contains = func(hash, key) {
    return __contains(hash, key);
}

var __num_to_char = [];
var __char_to_num = {};
func() {
    for(var i = 0; i<256; i += 1) {
        __temp_append(__num_to_char, __temp_char(i));
        __char_to_num[__temp_char(i)] = i;
    }
}();

var to_char = func(number) {
    return 0<=number and number<256? __num_to_char[number]:"";
}

var to_num = func(character) {
    return __temp_contains(__char_to_num, character)? __char_to_num[character]:-1;
}

var __string_split_with_empty_substr = func(separator, str) {
    return __split_with_empty_substr(separator, str);
}

var replace = func(needle, haystack, replacement) {
    var needle_size = size(needle);
    var haystack_size = size(haystack);
    var replacement_size = size(replacement);

    if (needle_size == 0 or
        haystack_size == 0 or
        replacement_size == 0 or
        needle_size > haystack_size
    ) {
        return haystack;
    }
    if (needle == haystack) {
        return replacement;
    }

    var vec = __string_split_with_empty_substr(needle, haystack);

    return join(replacement, vec);
}