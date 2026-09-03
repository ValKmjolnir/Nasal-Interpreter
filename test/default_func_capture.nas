# known bug: a default param whose value is a function that captures a
# sibling param (or enclosing local) segfaults the VM (exit 139).
var f1 = func(a, g = func() { return a; }) { return g(); };
println(f1(42));
