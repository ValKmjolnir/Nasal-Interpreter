use std.process_bar;
use std.os;
use std.io;
use std.math;

var ppm = func(filename, width, height, RGB) {
    # P3 use ASCII number
    # P6 use binary character
    var fd = io.open(filename, "wb");
    io.write(fd, "P6\n"~width~" "~height~"\n255\n");
    for(var i = 0; i<height; i += 1) {
        for(var j = 0; j<width; j += 1) {
            io.write(fd, RGB(i, j));
        }
    }
    io.close(fd);
}

var width = 1920;
var height = 1080;
var bar = (os.platform()=="windows")?
    process_bar.bar(front:"sharp", back:"point", sep:"line", length:50):
    process_bar.high_resolution_bar(50);

var f = func(i, j) {
    var (yMin, yMax, xMin, xMax) = (-2, 1.1, -3.2, 2);
    var (yDel, xDel) = (yMax-yMin, xMax-xMin);
    var (y, x) = ((i/height)*yDel+yMin, (j/width)*xDel+xMin);
    var (x0, y0) = (x, y);
    for(var iter = 0; iter<64; iter += 1) {
        (x0, y0) = (math.abs(x0), math.abs(y0));
        var (x1, y1) = ((x0*x0)-(y0*y0)+x, 2*x0*y0+y);
        (x0, y0) = (x1, y1);
        if ((x0*x0)+(y0*y0)>8) {
            break;
        }
    }
    var progress = (i*width+j+1)/(width*height);
    if (progress*100-int(progress*100)==0) {
        print(bar.bar(progress), " ", progress*100, "%          \r");
    }
    iter = iter>=25? 255:int(iter/25*255);
    var c = char(iter);
    return c~c~c;
}

ppm("burningship.ppm", width, height, f);
println();
