use std.unix;

srand();

var ant = {
    new: func(color, pos_x, pos_y) {
        return {
            move: int(rand()*4),
            color: color,
            pos_x: pos_x,
            pos_y: pos_y
        };
    }
};

# generate agents
var ants = [
    ant.new(10, 30, 15),
    ant.new(11, 31, 16),
    ant.new(4, 29, 13),
    ant.new(9, 30, 14),
    ant.new(13, 30, 12),
    ant.new(99, 25, 18)
];

# initialize game map
var map = [];
var map_color = [];
setsize(map, 60*30);
setsize(map_color, 60*30);
forindex(var i; map) {
    map[i] = 0;
    map_color[i] = 0;
}
foreach(var a; ants) {
    map_color[a.pos_x + a.pos_y*60] = a.color;
}

var print_map = func {
    var pics = ["  ", "[]"];
    var res = "\e[1;1H";
    for(var y = 0; y<30; y += 1) {
        for(var x = 0; x<60; x += 1) {
            res ~= "\e[38;5;"~map_color[x + y*60]~";1m";
            res ~= pics[map[x + y*60]] ~ "\e[0m";
        }
        res ~= "\n";
    }
    print(res);
}

var move = func {
    var move_step = [
        [0, -1],
        [1, 0],
        [0, 1],
        [-1, 0]
    ];
    var temp_map = [];
    setsize(temp_map, 60*30);
    forindex(var i; map) {
        temp_map[i] = map[i];
    }
    foreach(var a; ants) {
        var map_state = map[a.pos_x + a.pos_y*60];
        temp_map[a.pos_x + a.pos_y*60] = map_state==0? 1:0;
        if (map_state==1) {
            a.move -= 1;
            if (a.move < 0) {
                a.move = 3;
            }
        } else {
            a.move += 1;
            if (a.move > 3) {
                a.move = 0;
            }
        }
        a.pos_x += move_step[a.move][0];
        a.pos_y += move_step[a.move][1];
        if (a.pos_x < 0) { a.pos_x = 59; }
        if (a.pos_x > 59) { a.pos_x = 0; }
        if (a.pos_y < 0) { a.pos_y = 29; }
        if (a.pos_y > 29) { a.pos_y = 0; }

        if (map_color[a.pos_x + a.pos_y*60]!=a.color) {
            map_color[a.pos_x + a.pos_y*60] = a.color;
        } else {
            map_color[a.pos_x + a.pos_y*60] = 0;
        }
    }
    map = temp_map;
}

while(1) {
    print_map();
    move();
    unix.sleep(1/24);
}