use self_ref_c;
use self_ref_a;
use self_ref_b;

println(a);
println(b);
println(c);


println(id(a));
println(id(b), " ", id(b.a));
println(id(c), " ", id(c.b), " ", id(c.b.a));