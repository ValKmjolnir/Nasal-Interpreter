# process_bar.nas
# ValKmjolnir 2022/6/14
# this file is inspired by a Python lib: alive_progress

var process_bar={
    bar:nil,
    high_resolution_bar:nil,
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
        point:".",
        line:"━"
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

# return a high resolution progress bar
# example:
# var bar=process_bar.high_resolution_bar(40);
# for(var i=0;i<=1;i+=0.001){
#     print(bar.bar(i,40),'\r');
#     unix.sleep(0.001);
# }
# println();
process_bar.high_resolution_bar=func(){
    var block=["▏","▎","▍","▌","▋","▊","▉","█"];
    return func(length){
        return {
            bar: func(number){
                if(number>1)
                    number=1;
                if(number<0)
                    number=0;
                var block_len=number*length;
                var complete_block=int(block_len);
                var decimal=block_len-complete_block;
                var progress=complete_block+(decimal!=0);
                var s="|";
                for(var i=0;i<complete_block;i+=1){
                    s~="█";
                }
                if(decimal!=0){
                    s~=block[int(decimal*10)/10*size(block)];
                }
                for(var i=0;i<length-progress;i+=1){
                    s~=" ";
                }
                s~="|";
                return s;
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
process_bar.default_bar=func(name="classic",length=20){
    if(typeof(name)!="str")
        name="classic";
    if(name=="classic")
        return process_bar.bar("sharp","point","bracket",length);
    elsif(name=="classic2")
        return process_bar.bar("equal","point","bracket",length);
    elsif(name=="classic3")
        return process_bar.bar("sharp","point","line",length);
    elsif(name=="classic4")
        return process_bar.bar("equal","point","line",length);
    elsif(name=="triangle")
        return process_bar.bar("solid_triangle_right","hollow_triangle_right","angle_bracket",length);
    elsif(name=="dots")
        return process_bar.bar("solid_circle","hollow_circle","curve",length);
    elsif(name=="ticks")
        return process_bar.bar("tick","space","line",length);
    elsif(name=="deep_shadow")
        return process_bar.bar("deep_shadow","light_shadow","line",length);
    elsif(name=="block")
        return process_bar.bar("block","light_shadow","line",length);
    elsif(name=="oneline")
        return process_bar.bar("line","space","space",length);
    else
        return process_bar.bar("sharp","point","bracket",length);
}
process_bar.default_spinner=func(name="classic",repeat=1){
    if(typeof(name)!="str")
        name="classic";
    if(name=="rise")
        return process_bar.spinner("rise",repeat);
    elsif(name=="vertical")
        return process_bar.spinner("vertical",repeat);
    elsif(name=="dot")
        return process_bar.spinner("dot",repeat);
    elsif(name=="dots")
        return process_bar.spinner("dots",repeat);
    elsif(name=="arrow")
        return process_bar.spinner("arrow",repeat);
    elsif(name=="classic")
        return process_bar.spinner("classic",repeat);
    elsif(name=="balls")
        return process_bar.spinner("balls",repeat);
    elsif(name=="dots_wave")
        return process_bar.spinner("dots_wave",repeat);
    elsif(name=="pulse")
        return process_bar.spinner("pulse",repeat);
    elsif(name=="wave")
        return process_bar.spinner("wave",repeat);
    elsif(name=="short_wave")
        return process_bar.spinner("short_wave",repeat);
    elsif(name=="fish")
        return process_bar.spinner("fish",repeat);
    elsif(name=="happy")
        return process_bar.spinner("happy",repeat);
    elsif(name=="wait")
        return process_bar.spinner("wait",repeat);
    elsif(name=="stars")
        return process_bar.spinner("stars",repeat);
    else
        return process_bar.spinner("classic",repeat);
}

var show=func(){
    print("\ec");
    var bars={
        "classic    ":process_bar.default_bar("classic",40),
        "classic2   ":process_bar.default_bar("classic2",40),
        "classic3   ":process_bar.default_bar("classic3",40),
        "classic4   ":process_bar.default_bar("classic4",40),
        "triangle   ":process_bar.default_bar("triangle",40),
        "dots       ":process_bar.default_bar("dots",40),
        "ticks      ":process_bar.default_bar("ticks",40),
        "deep_shadow":process_bar.default_bar("deep_shadow",40),
        "block      ":process_bar.default_bar("block",40),
        "oneline    ":process_bar.default_bar("oneline",40)
    };
    var spinners={
        "rise       ":process_bar.default_spinner("rise",16),
        "vertical   ":process_bar.default_spinner("vertical",16),
        "dot        ":process_bar.default_spinner("dot",16),
        "dots       ":process_bar.default_spinner("dots",16),
        "arrow      ":process_bar.default_spinner("arrow",16),
        "classic    ":process_bar.default_spinner("classic",16),
        "balls      ":process_bar.default_spinner("balls",4),
        "dots_wave  ":process_bar.default_spinner("dots_wave",2),
        "pulse      ":process_bar.default_spinner("pulse",1),
        "wave       ":process_bar.default_spinner("wave",2),
        "short_wave ":process_bar.default_spinner("short_wave",4),
        "fish       ":process_bar.default_spinner("fish",1),
        "happy      ":process_bar.default_spinner("happy",1),
        "wait       ":process_bar.default_spinner("wait",1),
        "stars      ":process_bar.default_spinner("stars",1)
    };
    var bar_key=keys(bars);
    var spin_key=keys(spinners);
    for(var i=0;i<40;i+=1){
        forindex(var j;bar_key){
            var k=bar_key[j];
            print("\e["~(j+1)~";1H["~k~"] "~bars[k].bar((i+1)/40));
        }
        forindex(var j;spin_key){
            var k=spin_key[j];
            print("\e["~(j+1+size(bars))~";1H["~k~"] |"~spinners[k].next()~"|");
        }
        unix.sleep(1/20);
    }
    print("\n");
}