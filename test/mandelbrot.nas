var (yMin,yMax,xMin,xMax,line)=(-0.2,0.2,-1.5,-1.0,"");
var (yDel,xDel)=(yMax-yMin,xMax-xMin);
for(var yPixel=0;yPixel<24;yPixel+=1){
    var y=(yPixel/24)*yDel+yMin;
    for(var xPixel=0;xPixel<80;xPixel+=1){
        var x=(xPixel/80)*xDel+xMin;
        var pixel=" ";
        var (x0,y0)=(x,y);
        for(var iter=0;iter<80;iter+=1){
            var x1=(x0*x0)-(y0*y0)+x;
            var y1=2*x0*y0+y;
            (x0,y0)=(x1,y1);
            if((x0*x0)+(y0*y0)>4){
                pixel=chr(" .:;+=xX$&"[iter/8]);
                break;
            }
        }
        line~=pixel;
    }
    line~='\n';
}
print(line);