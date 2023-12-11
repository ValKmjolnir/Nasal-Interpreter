# padding.nas
# ValKmjolnir 2022/9/4

var leftpad = func(input_string, length, char=" ") {
    if (typeof(input_string)=="num") {
        input_string = str(input_string);
    }
    var strlen = size(input_string);
    for(var i = strlen; i<length; i += 1) {
        input_string = char~input_string;
    }
    return input_string;
}

var rightpad = func(input_string, length, char=" ") {
    if (typeof(input_string)=="num") {
        input_string = str(input_string);
    }
    var strlen = size(input_string);
    for(var i = strlen; i<length; i += 1) {
        input_string ~= char;
    }
    return input_string;
}