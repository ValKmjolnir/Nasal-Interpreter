import("lib.nas");

var table=[
    ['q0','0','1','R','q1'],
    ['q1','1','1','R','q1'],
    ['q1','0','0','S','q2'],
    ['q2','0','1','R','q3'],
    ['q3',nil,nil,'S','q3']
];
var prt=func(state,pointer,paper,act=nil){
    print(state,':',pointer,':',act!=nil?act:'','\n\t');
    var s='';
    foreach(var i;paper)
        s~=i;
    s~='\n\t';
    for(var i=0;i<pointer;i+=1)
        for(var j=0;j<size(paper[i]);j+=1)
            s~=' ';
    print(s,'^\n');
}
var run=func(table,node,start,stop){
    var paper=['0','1','1','1','0','1','0','a'];
    var pointer=0;
    foreach(var action;table){
        if(!contains(node,action[0]))
            node[action[0]]=nil;
        if(!contains(node,action[4]))
            node[action[4]]=nil;
    }
    print("nodes: ",keys(node),'\n');
    if(!contains(node,start))
        die(start~" is not a valid node");
    if(!contains(node,stop))
        die(stop~" is not a valid node");
    var state=start;
    prt(state,pointer,paper);
    while(state!=stop){
        foreach(var action;table)
            if(action[0]==state and (action[1]==paper[pointer] or action[1]==' ')){
                paper[pointer]=action[2]==nil?paper[pointer]:action[2];
                if(action[3]=='L') pointer-=1;
                elsif(action[3]=='R') pointer+=1;
                elsif(action[3]!='S') die("invalid action <"~action[3]~'>');
                state=action[4];
                break;
            }
        prt(state,pointer,paper,action);
    }
}

run(table,{},'q0','q3');