# process_bar.nas
# ValKmjolnir 2022/6/14
# this file is inspired by a Python lib: alive_progress

import("stl/log.nas");

var generate_bar=func(){
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
                for(var i=0;i<other-1;i+=1)
                    s~=back;
                return sep[0]~s~sep[1];
            }
        };
    };
}();

var generate_spinner=func(){
    var spinner={
        rise:["▁","▂","▃","▄","▅","▆","▇","█","█","▇","▆","▅","▄","▃","▂","▁"],
        vertical:["▏","▎","▍","▌","▋","▊","▉","▇","▇","▉","▊","▋","▌","▍","▎","▏"],
        dot:["⠁","⠈","⠐","⠠","⢀","⡀","⠄","⠂"],
        dots:["⣾","⣷","⣯","⣟","⡿","⢿","⣻","⣽"],
        arrow:["↑","↗","→","↘","↓","↙","←","↖"],
        classic:["/","-","\\","-"],
        balls:["●...",".●..","..●.","...●",],
        dots_wave:[
            "⠁⠂⠄⡀⢀⠠⠐⠈",
            "⠂⠄⡀⢀⠠⠐⠈⠁",
            "⠄⡀⢀⠠⠐⠈⠁⠂",
            "⡀⢀⠠⠐⠈⠁⠂⠄",
            "⢀⠠⠐⠈⠁⠂⠄⡀",
            "⠠⠐⠈⠁⠂⠄⡀⢀",
            "⠐⠈⠁⠂⠄⡀⢀⠠",
            "⠈⠁⠂⠄⡀⢀⠠⠐"
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
        fish:[
            "                ",
            ">               ",
            "`>              ",
            ")`>             ",
            "))`>            ",
            ")))`>           ",
            "))))`>          ",
            "<))))`>         ",
            "><))))`>        ",
            " ><))))`>       ",
            "  ><))))`>      ",
            "   ><))))`>     ",
            "    ><))))`>    ",
            "     ><))))`>   ",
            "      ><))))`>  ",
            "       ><))))`> ",
            "        ><))))`>",
            "         ><))))`",
            "          ><))))",
            "           ><)))",
            "            ><))",
            "             ><)",
            "              ><",
            "               >"
        ]
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
        "classic    ":generate_bar(front:"sharp",back:"point",sep:"bracket",length:40),
        "classic2   ":generate_bar(front:"equal",back:"point",sep:"bracket",length:40),
        "triangle   ":generate_bar(front:"solid_triangle_right",back:"hollow_triangle_right",sep:"angle_bracket",length:40),
        "dots       ":generate_bar(front:"solid_circle",back:"hollow_circle",sep:"curve",length:40),
        "ticks      ":generate_bar(front:"tick",back:"space",sep:"line",length:40),
        "deep_shadow":generate_bar(front:"deep_shadow",back:"light_shadow",sep:"line",length:40),
        "block      ":generate_bar(front:"block",back:"light_shadow",sep:"line",length:40)
    };
    var key=keys(bars);
    for(var i=0;i<40;i+=1){
        forindex(var j;key){
            var k=key[j];
            print("\e["~(j+1)~";1H["~k~"] "~bars[k].bar(i/40));
        }
        unix.sleep(1/15);
    }
    var spinners={
        "rise       ":generate_spinner(type:"rise",repeat:16),
        "vertical   ":generate_spinner(type:"vertical",repeat:16),
        "dot        ":generate_spinner(type:"dot",repeat:16),
        "dots       ":generate_spinner(type:"dots",repeat:16),
        "arrow      ":generate_spinner(type:"arrow",repeat:16),
        "classic    ":generate_spinner(type:"classic",repeat:16),
        "balls      ":generate_spinner(type:"balls",repeat:4),
        "dots_wave  ":generate_spinner(type:"dots_wave",repeat:2),
        "pulse      ":generate_spinner(type:"pulse",repeat:1),
        "wave       ":generate_spinner(type:"wave",repeat:2),
        "short_wave ":generate_spinner(type:"short_wave",repeat:4),
        "fish       ":generate_spinner(type:"fish",repeat:1)
    };
    var key=keys(spinners);
    for(var i=0;i<60;i+=1){
        forindex(var j;key){
            var k=key[j];
            var tmp=spinners[k];
            print("\e["~(j+1+size(bars))~";1H["~k~"] |"~tmp.next()~"|");
        }
        unix.sleep(1/15);
    }
    print("\n");
}

show();
