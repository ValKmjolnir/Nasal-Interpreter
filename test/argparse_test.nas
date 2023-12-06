# use module.libjson;

# libjson.stringify(math.tan);

# println(libjson.get_error());
use std.argparse;

var args = argparse.new("test cli");
args.add_command("--what", "-w", "what-what");
args.add_subparser("what", "what-what");
args.parse();
