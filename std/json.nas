# lib json.nas
# 2021 ValKmjolnir

var _json = func() {
    return _json_new();
}();

var _stringify = func(json_object, object) {
    return _json_stringify(json_object, object);
}

var _parse = func(json_object, input_string) {
    return _json_parse(json_object, input_string);
}

var _get_error = func(json_object) {
    return _json_get_error(json_object);
}

var stringify = func(object) {
    return _stringify(_json, object);
}

var parse = func(input_string) {
    return _parse(_json, input_string);
}

var get_error = func() {
    return _get_error(_json);
}