
var table=[
    ['q0','0','1','R','q1'],
    ['q1','1','1','R','q1'],
    ['q1','0','0','S','q2'],
    ['q2','0','1','R','q3'],
    ['q3',nil,nil,'S','q3']
];

var operand={
    new:func(symbol,changed_symbol,move,next_state){
        if(move!='L' and move!='R' and move!='S')
            die("invalid move type:"+move);
        return {
            symbol:symbol,
            changed_symbol:changed_symbol,
            move:move,
            next_state:next_state
        };
    }
};

var machine={
    states:{},
    add:func(state,operand){
        if(!contains(me.states,state))
            me.states[state]=[operand];
        else{
            foreach(var i;me.states[state])
                if(i.symbol==operand.symbol or i.symbol==nil){
                    println(i);
                    die("conflict operand");
                }
            append(me.states[state],operand);
        }
    },
    load:func(data){
        foreach(var opr;data){
            var (nstat,sym,csym,move,nextstat)=opr;
            me.add(nstat,operand.new(sym,csym,move,nextstat));
        }
    }
};

var prt=func(state,pointer,paper,act=nil){
    print(act!=nil?act:'','\n\t');
    var s='';
    foreach(var i;paper)
        s~=i;
    s~='\n\t';
    for(var i=0;i<pointer;i+=1)
        for(var j=0;j<size(paper[i]);j+=1)
            s~=' ';
    print(s,'^\n',state," ");
}
var run=func(table,start,stop){
    var paper=['0','1','1','1','0','1','0','a'];
    var pointer=0;

    machine.load(table);

    print("states: ",keys(machine.states),'\n');
    if(!contains(machine.states,start))
        die(start~" is not a valid node");
    if(!contains(machine.states,stop))
        die(stop~" is not a valid node");
    
    var (state,pointer)=(start,0);

    prt(state,pointer,paper);
    while(state!=stop){
        if(!contains(machine.states,state))
            die("no matching function for state:"~state);
        var found=0;
        foreach(var action;machine.states[state]){
            var (sym,csym,move,next)=(
                action.symbol,
                action.changed_symbol,
                action.move,
                action.next_state
            );
            if(sym==paper[pointer] or sym==nil){
                if(sym!=nil)
                    paper[pointer]=csym;
                if(move=='L') pointer-=1;
                elsif(move=='R') pointer+=1;
                (state,found)=(next,1);
                break;
            }
        }

        if(!found)
            die("no matching function for state:"~state);

        prt(state,pointer,paper,[sym,csym,move,next]);
    }
}

run(table,'q0','q3');
print('\n');