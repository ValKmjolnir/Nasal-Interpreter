# argparse.nas
# 2023/12/7 by ValKmjolnir

use std.padding;

var new = func(description) {
    var _arg = globals.arg;
    var parser = {
        description: description,
        subparser: [],
        command_list: [],
        add_command: func(long, short, help, need_arg = false, need_nargs = false) {
            return _add_command(parser, long, short, help, need_arg, need_nargs);
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
        add_command: func(long, short, help, need_arg = false, need_nargs = false) {
            return _add_command(parser, long, short, help, need_arg, need_nargs);
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
        var max_pad_length = 0;
        var info_pairs = [];
        foreach(var cmd; parser.subparser) {
            var info = "  "~cmd.name;
            append(info_pairs, {info: info, help: cmd.parser.description});
            info_length = size(info);
            max_pad_length = max_pad_length>info_length? max_pad_length:info_length;
        }
        foreach(var pair; info_pairs) {
            println(padding.rightpad(pair.info, max_pad_length), "    ", pair.help);
        }
        println();
    }
    if (size(parser.command_list)>0) {
        println("Options:");
        var max_pad_length = 0;
        var info_pairs = [];
        foreach(var cmd; parser.command_list) {
            if (cmd.need_nargs) {
                var info = "  "~cmd.full_name~" [args...] "~cmd.short_name~" [args...]";
                append(info_pairs, {info: info, help: cmd.help});
            } elsif (cmd.need_arg) {
                var info = "  "~cmd.full_name~" arg "~cmd.short_name~" arg";
                append(info_pairs, {info: info, help: cmd.help});
            } else {
                var info = "  "~cmd.full_name~" "~cmd.short_name;
                append(info_pairs, {info: info, help: cmd.help});
            }
            var info_length = size(info);
            max_pad_length = max_pad_length>info_length? max_pad_length:info_length;
        }
        foreach(var pair; info_pairs) {
            println(padding.rightpad(pair.info, max_pad_length), "    ", pair.help);
        }
    }
}

var _in_list = func(arginfo, command_list) {
    foreach(var cmd; command_list) {
        if (arginfo==cmd.full_name or arginfo==cmd.short_name) {
            return true;
        }
    }
    return false;
}

var _parse = func(parser, args, result_hash) {
    if (size(args)==0) {
        println("Require more command, use \"--help\" to get help.\n");
        _help(parser);
        exit(0);
    }
    foreach(var subparser; parser.subparser) {
        if (subparser.name==args[0]) {
            result_hash[subparser.name] = true;
            _parse(subparser.parser, size(args)>1? args[1:]:[], result_hash);
            return;
        }
    }
    for(var i = 0; i<size(args); i += 1) {
        var this_arg = args[i];
        var find_command_flag = false;
        foreach(var cmd; parser.command_list) {
            if (this_arg=="--help" or this_arg=="-h") {
                _help(parser);
                exit(0);
            }
            if (this_arg==cmd.full_name or this_arg==cmd.short_name) {
                find_command_flag = true;
                if (cmd.need_nargs) {
                    i += 1;
                    var args_collect = [];
                    while (i<size(args) and !_in_list(args[i], parser.command_list)) {
                        append(args_collect, args[i]);
                        i += 1;
                    }
                    i -= 1;
                    if (!size(args_collect)) {
                        println("Require argument(s) after command `", this_arg, "`.\n");
                        _help(parser);
                        exit(0);
                    }
                    result_hash[cmd.full_name] = args_collect;
                } elsif (cmd.need_arg) {
                    i += 1;
                    if (i<size(args) and !_in_list(args[i], parser.command_list)) {
                        result_hash[cmd.full_name] = args[i];
                    } else {
                        println("Require argument after command `", this_arg, "`.\n");
                        _help(parser);
                        exit(0);
                    }
                } else {
                    result_hash[cmd.full_name] = true;
                }
            }
        }
        if (!find_command_flag) {
            println("Invalid command `", this_arg, "`.\n");
            _help(parser);
            exit(0);
        }
    }
    return;
}

var _add_command = func(parser, long, short, help, need_arg , need_nargs) {
    var new_command = {
        full_name: long,
        short_name: short,
        help: help,
        need_arg: need_arg,
        need_nargs: need_nargs
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