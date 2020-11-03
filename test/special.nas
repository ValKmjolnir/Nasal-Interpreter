var hash={str:'hello',f:func{return me.str;}};
var tmp_f=hash.f;
hash=1;
print(tmp_f());
# undefined symbol 'me'
# this means that 
# when generating local_scope for function f,
# nasal_gc will not count 'me' as one reference of this hash

var h1={str:'hello',f:func{return me.str;}};
var h2={str:'world',f:func{return nil;}};
h2.f=h1.f;
print(h2.f());
# print 'world'
# this means that 'me' in hash's functions
# only points to the hash this function belongs to

var f1=func(){print(1);return 1;}
var f2=func(){print(2);return 0;}
f1() or f2();
# print '1'
# this means that when using 'or' or 'and',
# if the result is clear when calculating,
# objects behind will not be calculated