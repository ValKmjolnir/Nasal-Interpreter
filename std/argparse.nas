
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
        parse_args: func() {
            var result_hash = {};
            _parse(parser, _arg, result_hash);
            return result_hash;
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

var _parse = func(parser, args, result_hash) {
    if (size(args)==0) {
        println("Require more command, use \"--help\" to get help.\n");
        _help(parser);
        exit(0);
    }
    var first_arg = args[0];
    foreach(var subparser; parser.subparser) {
        if (subparser.name==first_arg) {
            result_hash[subparser.name] = true;
            _parse(subparser.parser, size(args)>1? args[1:]:[], result_hash);
            return;
        }
    }
    foreach(var this_arg; args) {
        var find_command_flag = false;
        foreach(var cmd; parser.command_list) {
            if (this_arg=="--help" or this_arg=="-h") {
                _help(parser);
                exit(0);
            }
            if (this_arg==cmd.full_name or this_arg==cmd.short_name) {
                result_hash[cmd.full_name] = true;
                find_command_flag = true;
            }
        }
        if (!find_command_flag) {
            println("Invalid command `", this_arg, "`.\n");
            _help(parser);
            exit(0);
        }
    }
    # unreachable
    println("Invalid command `", first_arg, "`.\n");
    _help(parser);
    exit(0);
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