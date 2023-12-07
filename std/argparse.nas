
var new = func(description) {
    var _arg = globals.arg;
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
            return _parse(parser, _arg);
        }
    };
    parser.add_command("--help", "-h", "Get help info and exit");
    return parser;
}

var _new_sub_parser = func(description) {
    var parser = {
        description: description,
        subparser: [],
        command_list: [],
        add_command: func(long, short, help) {
            return _add_command(parser, long, short, help);
        },
        add_subparser: func(name, help) {
            return _add_subparser(parser, name, help);
        }
    };
    parser.add_command("--help", "-h", "Get help info and exit");
    return parser;
}

var _help = func(parser) {
    println(parser.description, "\n");
    if (size(parser.subparser)>0) {
        println("Subcommand:");
        foreach(var cmd; parser.subparser) {
            println("  ", cmd.name, "\t", cmd.parser.description);
        }
        println();
    }
    if (size(parser.command_list)>0) {
        println("Options:");
        foreach(var cmd; parser.command_list) {
            println("  ", cmd.full_name, " ", cmd.short_name, "\t", cmd.help);
        }
    }
}

var _parse = func(parser, args) {
    if (size(args)==0) {
        _help(parser);
        return;
    }
    var first_arg = args[0];
    foreach(var subparser; parser.subparser) {
        if (subparser.name==first_arg) {
            return _parse(subparser.parser, size(args)>1? args[1:]:[]);
        }
    }
    foreach(var cmd; parser.command_list) {
        if (first_arg=="--help" or first_arg=="-h") {
            _help(parser);
            exit(0);
        }
        if (first_arg==cmd.full_name or first_arg==cmd.short_name) {
            return "[WIP]";
        }
    }
    # unreachable
    println("Invalid command `", first_arg, "`\n");
    _help(parser);
    return;
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
        parser: _new_sub_parser(help)
    };
    append(parser.subparser, new_subparser);
    return new_subparser.parser;
}