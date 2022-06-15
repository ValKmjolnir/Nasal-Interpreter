# process_bar.nas
# ValKmjolnir 2022/6/14
# this file is inspired by a Python lib: alive_progress

var process_bar={
    bar:nil,
    spinner:nil
};
process_bar.bar=func(){
    var bar={
        solid_triangle_right:"▶",
        hollow_triangle_right:"▷",
        solid_triangle_left:"◀",
        hollow_triangle_left:"◁",
        solid_circle:"●",
        hollow_circle:"○",
        tick:"✔",
        cross:"✘",
        light_shadow:"░",
        medium_shadow:"▒",
        deep_shadow:"▓",
        block:"█",
        sharp:"#",
        square:"√",
        equal:"=",
        space:" ",
        point:"."
    };
    var separator={
        angle_bracket:["<",">"],
        line:["|","|"],
        bracket:["[","]"],
        space:[" "," "],
        curve:["(",")"]
    };
    return func(front="sharp",back="space",sep="line",length=20){
        if(typeof(front)!="str" or !contains(bar,front))
            front="sharp";
        if(typeof(back)!="str" or !contains(bar,back))
            back="space";
        if(typeof(sep)!="str" or !contains(separator,sep))
            sep="line";
        front=bar[front];
        back=bar[back];
        sep=separator[sep];
        return {
            bar:func(number){
                if(number>1)
                    number=1;
                if(number<0)
                    number=0;
                var finish_length=int(number*length);
                var other=length-finish_length;
                var s="";
                for(var i=0;i<finish_length;i+=1)
                    s~=front;
                for(var i=0;i<other;i+=1)
                    s~=back;
                return sep[0]~s~sep[1];
            }
        };
    };
}();
process_bar.spinner=func(){
    var generate_scrolling_spinner=func(s){
        if(typeof(s)!="str")
            s="****";
        if(size(s)>16)
            s="****";
        var vec=split("",s);
        var res=[];
        var len=size(vec);
        var tmp="";
        for(var i=0;i<len;i+=1){
            tmp=pop(vec)~tmp;
            append(res,tmp);
            while(size(res[-1])!=16)
                res[-1]~=" ";
        }
        tmp=res[-1];
        while(tmp!="                "){
            tmp=" "~substr(tmp,0,15);
            append(res,tmp);
        }
        return res;
    }
    var spinner={
        rise:["▁","▂","▃","▄","▅","▆","▇","█","█","▇","▆","▅","▄","▃","▂","▁"],
        vertical:["▏","▎","▍","▌","▋","▊","▉","▇","▇","▉","▊","▋","▌","▍","▎","▏"],
        dot:["⠁","⠈","⠐","⠠","⢀","⡀","⠄","⠂"],
        dots:["⣾","⣷","⣯","⣟","⡿","⢿","⣻","⣽"],
        arrow:["↑","↗","→","↘","↓","↙","←","↖"],
        classic:["/","-","\\","-"],
        balls:["●...",".●..","..●.","...●",],
        dots_wave:[
            "⠈⠁⠂⠄⡀⢀⠠⠐",
            "⠐⠈⠁⠂⠄⡀⢀⠠",
            "⠠⠐⠈⠁⠂⠄⡀⢀",
            "⢀⠠⠐⠈⠁⠂⠄⡀",
            "⡀⢀⠠⠐⠈⠁⠂⠄",
            "⠄⡀⢀⠠⠐⠈⠁⠂",
            "⠂⠄⡀⢀⠠⠐⠈⠁",
            "⠁⠂⠄⡀⢀⠠⠐⠈"
        ],
        pulse:[
            "----------------",
            "●---------------",
            "-●--------------",
            "--●-------------",
            "---●------------",
            "----●-----------",
            "-----●----------",
            "------●---------",
            "-------√--------",
            "-------√\\-------",
            "-------√\\/------",
            "-------√\\/●-----",
            "--------\\/-●----",
            "---------/--●---",
            "-------------●--",
            "--------------●-",
            "---------------●"
        ],
        wave:[
            "▁▂▃▄▅▆▇█",
            "▂▃▄▅▆▇█▇",
            "▃▄▅▆▇█▇▆",
            "▄▅▆▇█▇▆▅",
            "▅▆▇█▇▆▅▄",
            "▆▇█▇▆▅▄▃",
            "▇█▇▆▅▄▃▂",
            "█▇▆▅▄▃▂▁",
            "▇▆▅▄▃▂▁▂",
            "▆▅▄▃▂▁▂▃",
            "▅▄▃▂▁▂▃▄",
            "▄▃▂▁▂▃▄▅",
            "▃▂▁▂▃▄▅▆",
            "▂▁▂▃▄▅▆▇"
        ],
        short_wave:[
            "▅▄█▇",
            "▄▃▇▆",
            "▃▂▆▅",
            "▂▁▅▄",
            "▁▂▄▃",
            "▂▃▃▂",
            "▃▄▂▁",
            "▄▅▁▂",
            "▅▆▂▃",
            "▆▇▃▄",
            "▇█▄▅",
            "█▇▅▆",
            "▇▆▆▇",
            "▆▅▇█"
        ],
        happy:[
            "                ",
            "ᕗ               ",
            " ᕗ              ",
            ") ᕗ             ",
            "ᐛ ) ᕗ           ",
            " ᐛ ) ᕗ          ",
            "( ᐛ ) ᕗ         ",
            " ( ᐛ ) ᕗ        ",
            " ᕕ ( ᐛ ) ᕗ      ",
            "  ᕕ ( ᐛ ) ᕗ     ",
            "   ᕕ ( ᐛ ) ᕗ    ",
            "    ᕕ ( ᐛ ) ᕗ   ",
            "     ᕕ ( ᐛ ) ᕗ  ",
            "      ᕕ ( ᐛ ) ᕗ ",
            "       ᕕ ( ᐛ ) ᕗ",
            "        ᕕ ( ᐛ ) ",
            "         ᕕ ( ᐛ )",
            "          ᕕ ( ᐛ ",
            "           ᕕ ( ᐛ",
            "            ᕕ ( ",
            "             ᕕ (",
            "              ᕕ ",
            "               ᕕ",
        ],
        fish:generate_scrolling_spinner("><))))`>"),
        wait:generate_scrolling_spinner("please wait"),
        stars:generate_scrolling_spinner("********")
    };
    return func(type="classic",repeat=1){
        if(typeof(type)!="str" or !contains(spinner,type))
            type="classic";
        type=spinner[type];
        var counter=0;
        return {
            next:func(){
                var s="";
                for(var i=0;i<repeat;i+=1)
                    s~=type[counter];
                counter+=1;
                if(counter>=size(type))
                    counter=0;
                return s;
            }
        }
    };
}();

var show=func(){
    print("\ec");
    var bars={
        "classic    ":process_bar.bar(front:"sharp",back:"point",sep:"bracket",length:40),
        "classic2   ":process_bar.bar(front:"equal",back:"point",sep:"bracket",length:40),
        "triangle   ":process_bar.bar(front:"solid_triangle_right",back:"hollow_triangle_right",sep:"angle_bracket",length:40),
        "dots       ":process_bar.bar(front:"solid_circle",back:"hollow_circle",sep:"curve",length:40),
        "ticks      ":process_bar.bar(front:"tick",back:"space",sep:"line",length:40),
        "deep_shadow":process_bar.bar(front:"deep_shadow",back:"light_shadow",sep:"line",length:40),
        "block      ":process_bar.bar(front:"block",back:"light_shadow",sep:"line",length:40)
    };
    var key=keys(bars);
    for(var i=0;i<40;i+=1){
        forindex(var j;key){
            var k=key[j];
            print("\e["~(j+1)~";1H["~k~"] "~bars[k].bar((i+1)/40));
        }
        unix.sleep(1/25);
    }
    var spinners={
        "rise       ":process_bar.spinner(type:"rise",repeat:16),
        "vertical   ":process_bar.spinner(type:"vertical",repeat:16),
        "dot        ":process_bar.spinner(type:"dot",repeat:16),
        "dots       ":process_bar.spinner(type:"dots",repeat:16),
        "arrow      ":process_bar.spinner(type:"arrow",repeat:16),
        "classic    ":process_bar.spinner(type:"classic",repeat:16),
        "balls      ":process_bar.spinner(type:"balls",repeat:4),
        "dots_wave  ":process_bar.spinner(type:"dots_wave",repeat:2),
        "pulse      ":process_bar.spinner(type:"pulse",repeat:1),
        "wave       ":process_bar.spinner(type:"wave",repeat:2),
        "short_wave ":process_bar.spinner(type:"short_wave",repeat:4),
        "fish       ":process_bar.spinner(type:"fish",repeat:1),
        "happy      ":process_bar.spinner(type:"happy",repeat:1),
        "wait       ":process_bar.spinner(type:"wait",repeat:1),
        "stars      ":process_bar.spinner(type:"stars",repeat:1)
    };
    var key=keys(spinners);
    for(var i=0;i<80;i+=1){
        forindex(var j;key){
            var k=key[j];
            var tmp=spinners[k];
            print("\e["~(j+1+size(bars))~";1H["~k~"] |"~tmp.next()~"|");
        }
        unix.sleep(1/20);
    }
    print("\n");
}
