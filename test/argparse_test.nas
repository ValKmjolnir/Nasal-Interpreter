use std.argparse;

var args = argparse.new("Nasal ArgParse Test CLI");
args.add_command("--what", "-wa", "what-command");
args.add_command("--who", "-wo", "who-command");
args.add_command("--where", "-we", "where-command");
var subparser0 = args.add_subparser("subcommand0", "Nasal ArgParse Test CLI Sub-command 0");
subparser0.add_command("--sub0-what", "-s0w", "sub0-what-command");
var subparser1 = args.add_subparser("subcommand1", "Nasal ArgParse Test CLI Sub-command 1");
subparser1.add_command("--sub1-what", "-s1w", "sub1-what-command");
var result = args.parse_args();
println(result);
