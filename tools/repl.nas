# experimental repl
# 2023/8/19 by ValKmjolnir

var help = func() {
    println(" .help   | show help");
    println(" .exit   | quit the REPL");
    println(" .quit   | quit the REPL");
    println(" .clear  | clear the screen");
    println();
}

var content = [];
var log_cache = "";

println("Nasal: This is experimental REPL");
help();

var count_bracket = func(line) {
    var len = size(line);
    var count = 0;
    for(var i = 0; i < len; i += 1) {
        if (line[i] == "{"[0]) {
            count += 1;
        } elsif (line[i] == "}"[0]) {
            count -= 1;
        }
    }
    return count;
}

while(1) {
    var line = readline(">>> ");
    if (line == ".exit" or line == ".quit") {
        break;
    } elsif (line == ".help") {
        println();
        help();
        continue;
    } elsif (line == ".clear") {
        print("\ec");
        continue;
    } elsif (line[0] == "."[0]) {
        println("no such command \"", line, "\", input \".help\" for help\n");
        continue;
    }
    var in_bracket_level = count_bracket(line);
    while(in_bracket_level > 0) {
        var temp_line = readline("... ");
        in_bracket_level += count_bracket(temp_line);
        line ~= temp_line ~ "\n";
    }

    append(content, line);

    var source = "";
    foreach(var i; content) {
        source ~= i ~ ";\n";
    }

    io.fout(".temp.nas", source);
    var result = system("nasal .temp.nas > .temp.log");
    if (result != 0) {
        pop(content);
        continue;
    }

    var log = io.readfile(".temp.log");
    if (size(log) and size(log) != size(log_cache)) {
        println(substr(log, size(log_cache), size(log)), "\n");
        log_cache = log;
    }
}