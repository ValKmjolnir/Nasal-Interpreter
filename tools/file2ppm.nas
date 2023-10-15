
var ppm = func(filename, buffer) {
    # P3 use ASCII number
    # P6 use binary character
    var width = 256;
    var height = int(size(buffer)/3/width); # ppm use 3 chars for one pixel
    println("width ", width, ", height ", height);

    var fd = io.open(filename, "wb");
    io.write(fd, "P6\n"~width~" "~height~"\n255\n");
    io.write(fd, buffer);
    io.close(fd);
}

if (size(arg)<1) {
    println("need input file and output file");
    exit(-1);
}

var content = io.readfile(arg[0], "r");
var tail_len = 0;
while(math.mod(size(content), 256*3)!=0) {
    content ~= "A";
    tail_len += 1;
}
println("filled ", tail_len);
println("size ", size(content));
ppm(size(arg)==2? arg[1]:"out.ppm", content);