import("lib.nas");
var yMin=-0.2;
var yMax=0.2;
var xMin=-1.5;
var xMax=-1.0;
for(var yPixel=0;yPixel<24;yPixel+=1)
{
    var y=(yPixel/24)*(yMax-yMin)+yMin;
    for(var xPixel=0;xPixel<80;xPixel+=1)
    {
        var x=(xPixel/80)*(xMax-xMin)+xMin;
        var pixel=" ";
        var x0=x;
        var y0=y;
        for(var iter=0;iter<80;iter+=1)
        {
            var x1=(x0*x0)-(y0*y0)+x;
            var y1=2*x0*y0+y;
            x0=x1;
            y0=y1;
            var d=(x0*x0)+(y0*y0);
            if(d>4)
            {
                pixel=chr(" .:;+=xX$&"[int(iter/8)]);
                break;
            }
        }
        print(pixel);
    }
    print('\n');
}