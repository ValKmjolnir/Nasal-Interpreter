use std.process_bar;
use std.os;
use std.io;
use std.math;

var ppm = func(filename, width, height, RGB) {
    # P3 use ASCII number
    # P6 use binary character
    var fd = io.open(filename, "wb");
    io.write(fd, "P6\n"~width~" "~height~"\n255\n");
    for(var i = 0; i<height; i+=1) {
        for(var j = 0; j<width; j+=1) {
            io.write(fd, RGB(i, j));
        }
    }
    io.close(fd);
}

var width = 1920 * 2;
var height = 1080 * 2;
var bar = (os.platform()=="windows")?
    process_bar.bar(front:"sharp", back:"point", sep:"line", length:50):
    process_bar.high_resolution_bar(50);
var abs = math.abs; # alias

var RGB = func(h, w) {
    var r = 2+w*2/width;
    var x = (height-h)/height;

    var (R, G, B) = (0, 0, 0);

    var tmp = 0.5;
    for(var i = 0; i<50; i+=1) {
        tmp = r*tmp*(1-tmp);
    }
    for(var i = 0; i<150; i+=1) {
        tmp = r*tmp*(1-tmp);
        if (abs(tmp-x)<0.0005) {
            R = int(255*(150 - i)/150);
            G = int(255*(150 - i)/150);
            B = 255;
            break;
        }
    }

    var progress = (h*width+w+1)/(width*height);
    if (progress*100-int(progress*100)==0) {
        print(bar.bar(progress), " ", progress*100, "%          \r");
    }

    return char(R) ~ char(G) ~ char(B);
}

ppm("feigenbaum.ppm", width, height, RGB);
println();