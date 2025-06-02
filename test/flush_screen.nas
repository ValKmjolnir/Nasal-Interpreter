use module.libkey;
use std.unix;

srand();

var chars = "abcdefghijklmnopqrstuvwxyz" ~
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ~
            "1234567890" ~
            "!@#$%^&*()_+-=~`[]{}\\|'\";:,.<>/?";
chars = split("", chars);

print("\ec");
while (1) {
    var key = libkey.nonblock();
    if (key!=nil and chr(key)=="q") {
        break;
    }
    var res = "\e[1;1H";
    for (var i = 0; i<20; i+=1) {
        for (var j = 0; j<40; j+=1) {
            res ~= "\e[38;5;" ~ int(rand()*256) ~ ";1m";
            res ~= chars[int(rand()*size(chars))];
            res ~= "\e[0m ";
        }
        res ~= "\n";
    }
    print(res);
    unix.sleep(1/15);
}
