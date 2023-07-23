# os.nas
# 2023 by ValKmjolnir

# os is used to use or get some os-related info/functions.
# windows/macOS/linux are supported.

# get a string that tell which os it runs on.
var platform = func() {
    return __platform;
}

var time = func() {
    return __logtime;
}

var arch = func() {
    return __arch;
}
