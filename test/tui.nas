use std.utils;
use std.unix;

var screen_state = {
    width: 0,
    height: 0
};

screen_state.update = func() {
    var res = utils.terminal_size();
    me.width = res.cols;
    me.height = res.rows;
}

screen_state.clear_screen = func() {
    me.update();

    var screen = "\e[0;0H";
    for (var i = 0; i < me.height; i+=1) {
        for (var j = 0; j < me.width; j+=1) {
            screen ~= " ";
        }
        if (i != me.height - 1)
            screen ~= "\n";
    }
    print(screen, "\e[0;0H");
}

screen_state.put_pixel = func(x, y, c) {
    x = int(x);
    y = int(y);
    if (x < 0 or x >= me.width or y < 0 or y >= me.height)
        return;
    var coord = "\e[" ~ y ~ ";" ~ x ~ "H";
    print(coord, c, "\e[0;0H");
}

var test_flush = func() {
    screen_state.clear_screen();
    for (var i=0; i<1e6; i+=1) {
        unix.sleep(0.001);
        screen_state.update();
        screen_state.put_pixel(
            rand()*screen_state.width,
            rand()*screen_state.height,
            "\e[38;5;" ~ int(256*rand()) ~ "m" ~ char(65 + 26*rand()) ~ "\e[0m"
        );
        print("\e[0;0H", i);
    }

    screen_state.clear_screen();
    println();
}