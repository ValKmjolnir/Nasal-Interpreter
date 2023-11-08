use std.phi;

var tips = func() {
    println("usage:");
    println("  nasal <this_file> <hostname> <port>");
}

if (size(arg)<2) {
    println("require hostname and port.");
    tips();
    exit(-1);
}
if (size(arg)>2) {
    println("too many arguments, only require hostname and port.");
    tips();
    exit(-1);
}

var connect = phi.new(arg[0], num(arg[1]));
var count = 0;
var recursive_get_prop = func(path = "/") {
    count += 1;
    if (math.mod(count, 50)==0) {
        println("get ", count," nodes, now: \"", path, "\"");
    }
    var props = connect.getprop(path);
    var tree = {};
    foreach(var key; keys(props)) {
        if (key=="children") {
            tree.children = [];
            continue;
        }
        tree[key] = props[key];
    }
    if (!contains(props, "children")) {
        return tree;
    }

    foreach(var child; props.children) {
        var node = recursive_get_prop(child.path);
        append(tree.children, node);
    }
    return tree;
}

# takes about 5 min to get whole tree
var props = recursive_get_prop("/");
phi.dump(props);
