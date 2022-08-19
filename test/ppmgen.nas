var RD=func(i,j){
    return bits.u32_and(255,j/255*255.999);
}
var GR=func(i,j){
    return bits.u32_and(255,i/255*255.999);
}
var BL=func(i,j){
    return bits.u32_and(255,0.25*255.999);
}
var pixel_write=func(fd,i,j){
    var color=RD(i,j)~" "~GR(i,j)~" "~BL(i,j)~" ";
    io.write(fd,color);
}

var fd=io.open("a.ppm","wb");
# P3 use ASCII number
# P6 use binary character
io.write(fd,"P3\n256 256\n255\n");
for(var i=255;i>=0;i-=1){
    for(var j=0;j<256;j+=1)
        pixel_write(fd,i,j);
    io.write(fd,"\n");
}
io.close(fd);