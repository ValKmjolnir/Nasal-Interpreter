# process_bar.nas
# ValKmjolnir 2022/6/14
# this file is inspired by a Python lib: alive_progress
use std.os;
use std.unix;

var bar = func() {
    var bar = {
        solid_triangle_right: "▶",
        hollow_triangle_right: "▷",
        solid_triangle_left: "◀",
        hollow_triangle_left: "◁",
        solid_circle: "●",
        hollow_circle: "○",
        tick: "✔",
        cross: "✘",
        light_shadow: "░",
        medium_shadow: "▒",
        deep_shadow: "▓",
        block: "█",
        sharp: "#",
        square: "√",
        equal: "=",
        space: " ",
        point: ".",
        line: "━"
    };
    var separator = {
        angle_bracket: ["<",">"],
        line: ["|","|"],
        bracket: ["[","]"],
        space: [" "," "],
        curve: ["(",")"]
    };
    return func(front = "sharp", back = "space", sep = "line", length = 20) {
        if (typeof(front)!="str" or !contains(bar,front))
            front="sharp";
        if (typeof(back)!="str" or !contains(bar,back))
            back="space";
        if (typeof(sep)!="str" or !contains(separator,sep))
            sep="line";
        front=bar[front];
        back=bar[back];
        sep=separator[sep];
        return {
            bar: func(number) {
                if (number>1)
                    number=1;
                if (number<0)
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
# for(var i=0;i<=1;i+=0.001) {
#     print(bar.bar(i,40),'\r');
#     unix.sleep(0.001);
# }
# println();
var high_resolution_bar = func() {
    var block=["▏","▎","▍","▌","▋","▊","▉","█"];
    return func(length) {
        return {
            bar: func(number) {
                if (number>1)
                    number=1;
                if (number<0)
                    number=0;
                var block_len=number*length;
                var complete_block=int(block_len);
                var decimal=block_len-complete_block;
                var progress=complete_block+(decimal!=0);
                var s="|";
                for(var i=0;i<complete_block;i+=1) {
                    s~="█";
                }
                if (decimal!=0) {
                    s~=block[int(decimal*10)/10*size(block)];
                }
                for(var i=0;i<length-progress;i+=1) {
                    s~=" ";
                }
                s~="|";
                return s;
            }
        };
    };
}();

var spinner = func() {
    var generate_scrolling_spinner = func(s) {
        if (typeof(s)!="str")
            s="****";
        if (size(s)>16)
            s="****";
        var vec=split("",s);
        var res=[];
        var len=size(vec);
        var tmp="";
        for(var i=0;i<len;i+=1) {
            tmp=pop(vec)~tmp;
            append(res,tmp);
            while(size(res[-1])!=16)
                res[-1]~=" ";
        }
        tmp=res[-1];
        while(tmp!="                ") {
            tmp=" "~substr(tmp,0,15);
            append(res,tmp);
        }
        return res;
    }
    var spinner = {
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
    return func(type = "classic", repeat = 1) {
        if (typeof(type)!="str" or !contains(spinner,type))
            type="classic";
        type=spinner[type];
        var counter=0;
        return {
            next: func() {
                var s="";
                for(var i=0;i<repeat;i+=1)
                    s~=type[counter];
                counter+=1;
                if (counter>=size(type))
                    counter=0;
                return s;
            }
        }
    };
}();

var default_bar = func(name = "classic", length = 20) {
    if (typeof(name)!="str")
        name="classic";
    if (name=="classic")
        return bar("sharp","point","bracket",length);
    elsif (name=="classic2")
        return bar("equal","point","bracket",length);
    elsif (name=="classic3")
        return bar("sharp","point","line",length);
    elsif (name=="classic4")
        return bar("equal","point","line",length);
    elsif (name=="triangle")
        return bar("solid_triangle_right","hollow_triangle_right","angle_bracket",length);
    elsif (name=="dots")
        return bar("solid_circle","hollow_circle","curve",length);
    elsif (name=="ticks")
        return bar("tick","space","line",length);
    elsif (name=="deep_shadow")
        return bar("deep_shadow","light_shadow","line",length);
    elsif (name=="block")
        return bar("block","light_shadow","line",length);
    elsif (name=="oneline")
        return bar("line","space","space",length);
    else
        return bar("sharp","point","bracket",length);
}

var default_spinner = func(name = "classic", repeat = 1) {
    if (typeof(name)!="str")
        name="classic";
    if (name=="rise")
        return spinner("rise",repeat);
    elsif (name=="vertical")
        return spinner("vertical",repeat);
    elsif (name=="dot")
        return spinner("dot",repeat);
    elsif (name=="dots")
        return spinner("dots",repeat);
    elsif (name=="arrow")
        return spinner("arrow",repeat);
    elsif (name=="classic")
        return spinner("classic",repeat);
    elsif (name=="balls")
        return spinner("balls",repeat);
    elsif (name=="dots_wave")
        return spinner("dots_wave",repeat);
    elsif (name=="pulse")
        return spinner("pulse",repeat);
    elsif (name=="wave")
        return spinner("wave",repeat);
    elsif (name=="short_wave")
        return spinner("short_wave",repeat);
    elsif (name=="fish")
        return spinner("fish",repeat);
    elsif (name=="happy")
        return spinner("happy",repeat);
    elsif (name=="wait")
        return spinner("wait",repeat);
    elsif (name=="stars")
        return spinner("stars",repeat);
    else
        return spinner("classic",repeat);
}

var show = func() {
    print("\ec");
    var bars={
        "classic    ":default_bar("classic",40),
        "classic2   ":default_bar("classic2",40),
        "classic3   ":default_bar("classic3",40),
        "classic4   ":default_bar("classic4",40),
        "triangle   ":default_bar("triangle",40),
        "dots       ":default_bar("dots",40),
        "ticks      ":default_bar("ticks",40),
        "deep_shadow":default_bar("deep_shadow",40),
        "block      ":default_bar("block",40),
        "oneline    ":default_bar("oneline",40)
    };
    var spinners={
        "rise       ":default_spinner("rise",16),
        "vertical   ":default_spinner("vertical",16),
        "dot        ":default_spinner("dot",16),
        "dots       ":default_spinner("dots",16),
        "arrow      ":default_spinner("arrow",16),
        "classic    ":default_spinner("classic",16),
        "balls      ":default_spinner("balls",4),
        "dots_wave  ":default_spinner("dots_wave",2),
        "pulse      ":default_spinner("pulse",1),
        "wave       ":default_spinner("wave",2),
        "short_wave ":default_spinner("short_wave",4),
        "fish       ":default_spinner("fish",1),
        "happy      ":default_spinner("happy",1),
        "wait       ":default_spinner("wait",1),
        "stars      ":default_spinner("stars",1)
    };
    var bar_key=keys(bars);
    var spin_key=keys(spinners);
    for(var i=0; i<40; i+=1) {
        forindex(var j; bar_key) {
            var k=bar_key[j];
            print("\e["~(j+1)~";1H["~k~"] "~bars[k].bar((i+1)/40));
        }
        forindex(var j; spin_key) {
            var k=spin_key[j];
            print("\e["~(j+1+size(bars))~";1H["~k~"] |"~spinners[k].next()~"|");
        }
        unix.sleep(1/20);
    }
    print("\n");
}