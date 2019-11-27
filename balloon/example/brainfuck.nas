
var resource=[];
var data=[];
var ptr=0;
for(var i=0;i<1024;i+=1){append(data,0);}

var input_file=func(str)
{
    resource=[];
    var res=read(str);
    for(var i=0;i<size(res);i+=1)
    {
        var ch=chars(res[i]);
        for(var j=0;j<size(ch);j+=1){append(resource,ch[j]);}
    }
    return;
};
var runloop=func(times,begin)
{
    var ends=begin;
    for(var t=0;t<times;t+=1)
    {
        for(var i=begin;i<size(resource);i+=1)
        {
            if(resource[i]=='+'){data[ptr]+=1;}
            else if(resource[i]=='-'){data[ptr]-=1;}
            else if(resource[i]=='>'){ptr+=1;}
            else if(resource[i]=='<'){ptr-=1;}
            else if(resource[i]=='.'){print(data[ptr],' ');}
            else if(resource[i]==','){data[ptr]=num(input());}
            else if(resource[i]=='[')
            {
                var t_ends=runloop(data[ptr],i+1);
                i=t_ends;
            }
            else if(resource[i]==']'){ends=i;break;}
            else{print('error token: ',resource[i],'\n');}
        }
    }
    return ends;
};
var runtime=func()
{
    for(var i=0;i<1024;i+=1){data[i]=0;}
    ptr=0;
    for(var i=0;i<size(resource);i+=1)
    {
        if(resource[i]=='+'){data[ptr]+=1;}
        else if(resource[i]=='-'){data[ptr]-=1;}
        else if(resource[i]=='>'){ptr+=1;}
        else if(resource[i]=='<'){ptr-=1;}
        else if(resource[i]=='.'){print(data[ptr],' ');}
        else if(resource[i]==','){data[ptr]=num(input());}
        else if(resource[i]=='[')
        {
            var ends=runloop(data[ptr],i+1);
            i=ends;
        }
        else{print('error token: ',resource[i],'\n');}
    }
    return;
};
while(1)
{
    print("bf> ");
    var filename=input();
    if(filename=='exit'){break;}
    input_file(filename);
    runtime();
    print('\n');
}
