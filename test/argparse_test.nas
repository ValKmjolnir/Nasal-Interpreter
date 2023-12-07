use std.argparse;

var args = argparse.new("test cli");
args.add_command("--what", "-w", "what-what");
var subparser0 = args.add_subparser("subcommand0", "what-what");
subparser0.add_command("--sub-what", "-sw", "sub-what-what");
args.parse();
