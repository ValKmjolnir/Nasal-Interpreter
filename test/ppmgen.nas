var RD=func(i,j){
    return chr(bits.u32_and(255,j*0.25+i*0.001));
}
var GR=func(i,j){
    return chr(bits.u32_and(255,j*0.15));
}
var BL=func(i,j){
    return chr(bits.u32_and(255,j*0.05));
}
var pixel_write=func(fd,i,j){
    var color=RD(i,j)~GR(i,j)~BL(i,j);
    io.write(fd,color);
}

var fd=io.open("a.ppm","wb");
io.write(fd,"P6\n512 512\n255\n");
for(var i=0;i<512;i+=1)
    for(var j=0;j<512;j+=1)
        pixel_write(fd,i,j);
io.close(fd);