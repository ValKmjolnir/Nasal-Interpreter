use std.runtime;

runtime.windows.set_utf8_output();

var table_character_set = [
    "─", "━", "│", "┃", "╌", "╍", "╎", "╏", "┄", "┅",
    "┆", "┇", "┈", "┉", "┊", "┋", "┌", "┍", "┎", "┏",
    "┐", "┑", "┒", "┓", "└", "┕", "┖", "┗", "┘", "┙",
    "┚", "┛", "├", "┝", "┞", "┟", "┠", "┡", "┢", "┣",
    "┤", "┥", "┦", "┧", "┨", "┩", "┪", "┫", "┬", "┭",
    "┮", "┯", "┰", "┱", "┲", "┳", "┴", "┵", "┶", "┷",
    "┸", "┹", "┺", "┻", "┼", "┽", "┾", "┿", "╀", "╁",
    "╂", "╃", "╄", "╅", "╆", "╇", "╈", "╉", "╊", "╋",
    "╪", "╫", "╬", "═", "║", "╒", "╓", "╔", "╕", "╖",
    "╗", "╘", "╙", "╚", "╛", "╜", "╝", "╞", "╟", "╠",
    "╡", "╢", "╣", "╤", "╥", "╦", "╧", "╨", "╩", "╔",
    "╗", "╝", "╚", "╬", "═", "╓", "╩", "┠", "┨", "┯",
    "┷", "┏", "┓", "┗", "┛", "┳", "⊥", "﹃", "﹄",
    "╮", "╭", "╯", "╰", "╳"
];

var road_enum = {
    null: 0,
    narrow: 1,
    wide: 2
};

var block = {
    "┌": [road_enum.null, road_enum.narrow, road_enum.narrow, road_enum.null],
    "└": [road_enum.narrow, road_enum.narrow, road_enum.null, road_enum.null],
    "┐": [road_enum.null, road_enum.null, road_enum.narrow, road_enum.narrow],
    "┘": [road_enum.narrow, road_enum.null, road_enum.null, road_enum.narrow],
    "╪": [road_enum.narrow, road_enum.wide, road_enum.narrow, road_enum.wide],
    "│": [road_enum.narrow, road_enum.null, road_enum.narrow, road_enum.null],
    "─": [road_enum.null, road_enum.narrow, road_enum.null, road_enum.narrow],
    "┼": [road_enum.narrow, road_enum.narrow, road_enum.narrow, road_enum.narrow],
    "╫": [road_enum.wide, road_enum.narrow, road_enum.wide, road_enum.narrow],
    "╬": [road_enum.wide, road_enum.wide, road_enum.wide, road_enum.wide],
    "═": [road_enum.null, road_enum.wide, road_enum.null, road_enum.wide],
    "║": [road_enum.wide, road_enum.null, road_enum.wide, road_enum.null],
    # "╒": [road_enum.null, road_enum.wide, road_enum.narrow, road_enum.null],
    # "╓": [road_enum.null, road_enum.narrow, road_enum.wide, road_enum.null],
    "╔": [road_enum.null, road_enum.wide, road_enum.wide, road_enum.null],
    # "╕": [road_enum.null, road_enum.null, road_enum.narrow, road_enum.wide],
    # "╖": [road_enum.null, road_enum.null, road_enum.wide, road_enum.narrow],
    "╗": [road_enum.null, road_enum.null, road_enum.wide, road_enum.wide],
    # "╘": [road_enum.narrow, road_enum.wide, road_enum.null, road_enum.null],
    # "╙": [road_enum.wide, road_enum.narrow, road_enum.null, road_enum.null],
    "╚": [road_enum.wide, road_enum.wide, road_enum.null, road_enum.null],
    # "╛": [road_enum.narrow, road_enum.null, road_enum.null, road_enum.wide],
    # "╜": [road_enum.wide, road_enum.null, road_enum.null, road_enum.narrow],
    "╝": [road_enum.wide, road_enum.null, road_enum.null, road_enum.wide],
    "╞": [road_enum.narrow, road_enum.wide, road_enum.narrow, road_enum.null],
    "╟": [road_enum.wide, road_enum.narrow, road_enum.wide, road_enum.null],
    "╠": [road_enum.wide, road_enum.wide, road_enum.wide, road_enum.null],
    "╡": [road_enum.narrow, road_enum.null, road_enum.narrow, road_enum.wide],
    "╢": [road_enum.wide, road_enum.null, road_enum.wide, road_enum.narrow],
    "╣": [road_enum.wide, road_enum.null, road_enum.wide, road_enum.wide],
    "╤": [road_enum.null, road_enum.wide, road_enum.narrow, road_enum.wide],
    "╥": [road_enum.null, road_enum.narrow, road_enum.wide, road_enum.narrow],
    "╦": [road_enum.null, road_enum.wide, road_enum.wide, road_enum.wide],
    "╧": [road_enum.narrow, road_enum.wide, road_enum.null, road_enum.wide],
    "╨": [road_enum.wide, road_enum.narrow, road_enum.null, road_enum.narrow],
    "╩": [road_enum.wide, road_enum.wide, road_enum.null, road_enum.wide],
    " ": [road_enum.null, road_enum.null, road_enum.null, road_enum.null]
};

var choice = func(above_block_char,
                  left_block_char,
                  flag_no_road_at_right,
                  flag_no_road_at_bottom) {
    var above_block = block[above_block_char];
    var left_block = block[left_block_char];
    var block_char_set = keys(block);

    var possible = [];
    foreach(var bcs; block_char_set) {
        var this_block = block[bcs];
        if (this_block[0] == above_block[2] and
            this_block[3] == left_block[1]) {
            if (flag_no_road_at_right and this_block[1]!=road_enum.null) {
                continue;
            }
            if (flag_no_road_at_bottom and this_block[2]!=road_enum.null) {
                continue;
            }
            append(possible, bcs);
        }
    }
    if (!size(possible)) {
        return " ";
    }

    func() {
        if (vecindex(possible, " ")!=nil) {
            for(var i = 0; i < 64; i+= 1) {
                append(possible, " ");
            }
        }
    }();
    return possible[rand()*size(possible)];
}

var print_whole_line = func(this_line) {
    var res = "";
    foreach(var i; this_line) {
        res ~= i;
    }
    println(res);
}

srand();
var last_line = [];
for(var i = 0; i<100; i+=1) {
    append(last_line, " ");
}

for(var iteration_times = 0; iteration_times < 25; iteration_times += 1) {
    var this_line = [];
    var left_block = " ";
    for(var i = 0; i < 100; i += 1) {
        var this_block = choice(
            last_line[i],
            left_block,
            i==99,
            iteration_times==24
        );
        left_block = this_block;
        append(this_line, this_block);
    }
    print_whole_line(this_line);
    last_line = this_line;
}
