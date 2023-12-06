
var _arg = globals.arg;

var new = func(description) {
    var parser = {
        description: description,
        subparser: [],
        command_list: [],
        add_command: func(long, short, help) {
            return _add_command(parser, long, short, help);
        },
        add_subparser: func(name, help) {
            return _add_subparser(parser, name, help);
        },
        parse: func() {
            return _parse(parser);
        }
    };
    return parser;
}

var _help = func(parser) {
    println(parser.description);
    println("Subcommand:");
    foreach(var cmd; parser.subparser) {
        println(cmd.name, "\t", cmd.parser.description);
    }
    println("Options:");
    foreach(var cmd; parser.command_list) {
        println(cmd.full_name, " ", cmd.short_name, "\t", cmd.help);
    }
}

var _parse = func(parser) {
    if (size(_arg)==0) {
        _help(parser);
    }
}

var _add_command = func(parser, long, short, help) {
    var new_command = {
        full_name: long,
        short_name: short,
        help: help
    };
    append(parser.command_list, new_command);
}

var _add_subparser = func(parser, name, help) {
    var new_subparser = {
        name: name,
        parser: new(help)
    };
    append(parser.subparser, new_subparser);
}