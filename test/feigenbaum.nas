use std.process_bar;

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

var width = 1600;
var height = 900;
var bar = (os.platform()=="windows")?
    process_bar.bar(front:"sharp", back:"point", sep:"line", length:50):
    process_bar.high_resolution_bar(50);

var RGB = func(h, w) {
    var r = 2+w*2/width;
    var x = (height-h)/height;

    var res = 0;
    var tmp = 0.5;
    for(var i = 0; i<50; i+=1) {
        tmp = r*tmp*(1-tmp);
    }
    for(var i = 0; i<100; i+=1) {
        tmp = r*tmp*(1-tmp);
        if (math.abs(tmp-x)<0.001) {
            res = 255;
            break;
        }
    }

    var progress = (h*width+w+1)/(width*height);
    if(progress*100-int(progress*100)==0){
        print(bar.bar(progress), " ", progress*100, "%          \r");
    }

    var c = char(res);
    return c~c~c;
}
ppm("feigenbaum.ppm", width, height, RGB);
println();