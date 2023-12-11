use std.argparse;

var test_cli = func(args) {
    globals.arg = args;
    var args = argparse.new("Nasal ArgParse Test CLI");
    args.add_command(long: "--what", short: "-wa", help: "What-command");
    args.add_command(long: "--who", short: "-wo", help: "Who-command");
    args.add_command(long: "--where", short: "-we", help: "Where-command");
    args.add_command(long: "--test_arg", short: "-ta", help: "Test-single-arg", need_arg: true);
    args.add_command(long: "--test_args", short: "-tas", help: "Test-multiple-args", need_nargs: true);
    var subparser0 = args.add_subparser(name: "subcommand0", help: "Nasal ArgParse Test CLI Sub-Command 0");
    subparser0.add_command(long: "--sub0-what", short: "-s0w", help: "Sub0-what-command");
    subparser0.add_command(long: "--test_arg", short: "-ta", help: "Test-single-arg", need_arg: true);
    subparser0.add_command(long: "--test_args", short: "-tas", help: "Test-multiple-args", need_nargs: true);
    var subparser1 = args.add_subparser(name: "subcommand1", help: "Nasal ArgParse Test CLI Sub-Command 1");
    subparser1.add_command(long: "--sub1-what", short: "-s1w", help: "Sub1-what-command");
    subparser1.add_command(long: "--test_arg", short: "-ta", help: "Test-single-arg", need_arg: true);
    subparser1.add_command(long: "--test_args", short: "-tas", help: "Test-multiple-args", need_nargs: true);
    var subparser_test = args.add_subparser(name: "subtest", help: "Test Subcommand");
    var result = args.parse_args();
    println(result);
}

# should cause error or exit
# test_cli(["--help"]);
# test_cli(["subtest"]);
# test_cli(["--test_args", "--what", "--who", "--where"]);
# test_cli(["--test_arg", "a", "b", "--what", "--who", "--where"]);

# should be correct
test_cli(["--test_arg", "a", "--what", "--who", "--where"]);
test_cli(["--test_args", "a", "--what", "--who", "--where"]);
test_cli(["--test_args", "a", "b", "c", "--what", "--who", "--where"]);
test_cli(["subcommand0", "--test_arg", "a", "-s0w"]);
test_cli(["subcommand0", "--test_args", "a", "-s0w"]);
test_cli(["subcommand0", "--test_args", "a", "b", "c", "-s0w"]);
test_cli(["subcommand1", "--test_arg", "a", "-s1w"]);
test_cli(["subcommand1", "--test_args", "a", "-s1w"]);
test_cli(["subcommand1", "--test_args", "a", "b", "c", "-s1w"]);
