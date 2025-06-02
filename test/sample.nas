# This file is written by Andy Ross, and is protected by GPLv2.0

# A no-op function used below to get this file to run.  Ignore and read on...
var dummyFunc = func { 1 }

#
# Literal numbers can be decimal, exponential, or hex constants.  All
# numbers are stored internally as IEEE double-precision values.
#
var n1 = 3;
var n2 = 3.14;
var n3 = 6.023e23;
var n3 = 0x123456;

#
# Two identical string literals with different quotes.  Double quotes
# use typical escape sequences.  Single quotes treat everything as
# literal except for embedded single quotes (including embedded
# whitespace like newlines).  Double quotes handle the following
# C-like escapes: \n \r \t \xnn \"
#
var s1 = 'Andy\'s "computer" has a C:\righteous\newstuff directory.';
var s2 = "Andy's \"computer\" has a C:\\righteous\\newstuff directory.";

#
# Literal lists use square brackets with a comma-separated expression
# list.
#
var list1 = ["a", "b", 1, 2];

#
# Literal hashes (or objects -- same thing) use curlies and colons to
# separate the key/value pairs.  Note that the key can be either a
# symbol or a literal scalar.  Object definitions will typically want
# to use symbols, lookup tables of other types will be more
# comfortable with literals.
#
var hash1 = { name : "Andy", job : "Hacker" };
var EnglishEspanol = { "one" : "uno", "two": "dos", "blue" : "azul" };

#
# Both vectors and hashes use square brackets for the lookup operation:
#
list1[0] == "a";
hash1["name"] == "Andy";

#
# A simple function.  By default, arguments are passed in the "arg"
# array, not unlike perl.  Note that this is just an assignment of an
# (anonymous) function argument to the local "log_message" variable.
# There is no function declaration syntax in Nasal.
#
var log_message = func {
    print(arg[0]);
}

#
# You can also pass named arguments to a function, thus saving the
# typing and performance costs of extracting them from the arg array.
#
var sqrt = dummyFunc;
var dist = func(x1, y1, x2, y2) {
    var dx = x2-x1;
    var dy = y2-y1;
    return sqrt(dx*dx + dy*dy);
}
dist(0,0,1,1); # == sqrt(2)

#
# These arguments can have default values, as in C++.  Note that the
# default value must be a scalar (number, string, function, nil) and
# not a mutable composite object (list, hash).
#
var read = func(bytes, flags=0) { }

#
# Any extra arguments after the named list are placed in the "arg"
# vector as above.  You can rename this to something other than "arg"
# by specifying a final argument name with an ellipsis:
#
var listify = func(elements...) { return elements; }
listify(1, 2, 3, 4); # returns a list: [1, 2, 3, 4]

#
# You can use a "var" qualifier to make sure that an assigment is made
# to a local variable, and not one from the enclosing scope.  This is
# good practice in general, although it is not required.  Note that
# this is not a "declaration", just a qualifier on the "=" operator.
#
var innerFunc = func {
    for (var dist=0; dist<100; dist += 1) {
        # Does not interfere with the "dist" symbol defined above
    }
}

#
# Nasal has no "statements", which means that any expression can appear
# in any context.  This means that you can use an if/else clause to do
# what the ?: does in C.  The last semicolon in a code block is
# optional, to make this prettier.
#
var abs = func(n) { if (n<0) { -n } else { n } }

#
# But for those who don't like typing, the ternary operator works like
# you expect:
#
var abs = func(n) { n < 0 ? -n : n }

#
# Nasal supports a "nil" value for use as a null pointer equivalent.
# It can be tested for equality, matching only other nils.
#
var listNode = { data : ["what", "ever"], next : nil };

#
# Nasal's binary boolean operators are "and" and "or", unlike C.
# unary not is still "!" however.  They short-circuit like you expect
#
var toggle = 0;
var a = nil;
if (a and a.field == 42) {
    toggle = !toggle; # doesn't crash when a is nil
}

#
# Looping constructs are mostly C-like.  The differences are that
# there is no do {} while (); construct, and there is a foreach, which
# takes a local variable name as its first argument and a vector as
# its second.
#
var doSomething = dummyFunc;

var stillGoing = 0;
while (stillGoing) { doSomething(); }

for (var i=0; i < 3; i = i+1) {
    elem = list1[i];
    doSomething(elem);
}

foreach(var elem; list1) { doSomething(elem) }  # Shorthand for above

#
# There is also a "forindex", which is like foreach except that it
# assigns the index of each element, instead of the value, to the loop
# variable.
#
forindex(var i; list1) { doSomething(list1[i]); }

#
# Define a class object with one method, one field and one "new"
# contstructor.  The "new" function is not special in any way; it
# simply returns an initialized object with the "parents" field set
# appropriately.  Member functions can get their local object (the
# equivalent of the "this" pointer in C++) as the "me" variable.
#
var Class1 = {};

Class1.new = func {
    var obj = { parents : [Class1],
            count : 0 };
    return obj;
}

Class1.getcount = func {
    me.count = me.count + 1;
    return me.count;
}

var c = Class1.new();
print(c.getcount(), "\n"); # prints 1
print(c.getcount(), "\n"); # prints 2
print(c.getcount(), "\n"); # prints 3

# Note that member *get* operations recurse into the parents array.
# But *set* operations always go to the local object.  You can't
# corrupt a parent class via OOP operations on its instances (but you
# *can* get to it via hand-inspection of the parents arrays).
var c2 = Class1.new();
c2.getcount = func { return 12345 }; # custom "derived" function!

print(c2.getcount(), "\n"); # prints 12345
print(c.getcount(), "\n"); # prints 4, Class1.getcount is unchanged

#
# This creates an identical class using alternative syntax.
#
var Class2 = {
    new : func {
        var obj = {};
        obj.parents = [Class2];
        obj.count = 0;
        return obj;
    },
    getcount : func {
        me.count = me.count + 1;
        return me.count;
    }
};

#
# Advanced strings: Nasal strings are always arrays of bytes (never
# characters: see the utf8 library if you want character-based
# equivalents of substr() et. al.).  They can be indexed just like in
# C (although note that there is no nul termination -- get the length
# with size()):
#
var string = "abcdefghijklmnopqrstuvwxyz";
var ascii_sum = 0;
for (var i=0; i<size(string); i+=1) { ascii_sum += string[i]; }

#
# You can use backquotes to write UTF8 character constants
#
if (`A` != 65) { print("ASCII violation bug!\n"); }
if (`©` != 169) { print("Unicode violation bug!\n"); }

#
# And you can mutate strings by assigning to their indices, as long as
# they are not constants.  You
# can make a mutable string either with the append operator or the
# bits.buf() function.
#
var ascii_lc = func(string) {
    var mutable = string ~ "";
    for (var i=0; i<size(mutable); i+=1) {
        if (mutable[i] >= `A` and mutable[i] <= `Z`) {
            mutable[i] += (`a` - `A`);
        }
    }
    return mutable;
}
print(ascii_lc("ABCDEFG"), "\n"); # prints "abcdefg"

#
# Advanced vectors: The lookup index can be negative, where -1
# indicates the last element in the vector (or string).
#
var next_to_last = list1[-2];

#
# Remember that strings look syntactically like vectors of bytes; so
# conversely, the "~" concatenation operator works equally well to
# concatenate vectors:
#
var joined_list = [1, 2, 3] ~ [4, 5, 6];
print(joined_list, "\n");

###
### Now some fun examples:
###

#
# Make a "inverted index" hash out of a vector that returns the index
# for each element.
#
var invert = func(vec) {
    var hash = {};
    for (var i=0; i<size(vec); i = i+1) {
        hash[vec[i]] = i;
    }
    return hash;
}

#
# Use the return value of the above function to do an "index of"
# lookup on a vector
#
var vecfind = func(vec, elem) { return invert(vec)[elem]; }

#
# Joins its arguments with the empty string and returns a scalar.
# Note use of "~" operator to do string concatenation (Nasal's only
# funny syntax).
#
var join = func { 
    var s = "";
    foreach(var elem; arg) { s = s ~ elem; }
    return s;
}

#
# Labeled break/continue syntax puts the label in as an extra first
# argument to the for/while/foreach.
#
var doneWithInnerLoopEarly = dummyFunc;
var completelyDone = dummyFunc;
# not supported now
for (#OUTER;
    var i=0; i<100; i = i+1) {
    for (var j=0; j<100; j = j+1) {
        if (doneWithInnerLoopEarly()) {
            break;
        } elsif (completelyDone()) {
            break #OUTER;
        }
    }
}

##
## A rockin' metaprogramming hack.  Generates and returns a deep copy
## of the object in valid Nasal syntax.  A warning to those who might
## want to use this: it ignores function objects (which cannot be
## inspected from Nasal) and replaces them with nil references.  It
## also makes no attempt to escape special characters in strings, which
## can break re-parsing in strange (and possibly insecure!) ways.
##
var dump = func(o) {
    var result = "";
    if (typeof(o) == "scalar") {
        var n = num(o);
        if (n == nil) { result = result ~ '"' ~ o ~ '"'; }
        else { result = result ~ o; }
    } elsif (typeof(o) == "vector") {
        result = result ~ "[ ";
        if (size(o) > 0) { result = result ~ dump(o[0]); }
        for (i=1; i<size(o); i=i+1) {
            result = result ~ ", " ~ dump(o[i]);
        }
        result = result ~ " ]";
    } elsif (typeof(o) == "hash") {
        var ks = keys(o);
        result = result ~ "{ ";
        if (size(o) > 0) {
            var k = ks[0];
            result = result ~ k ~ ":" ~ dump(o[k]);
        }
        for (i=1; i<size(o); i=i+1) {
            var k = ks[i];
            result = result ~ ", " ~ k ~ " : " ~ dump(o[k]);
        }
        result = result ~ " }";
    } else {
        result = result ~ "nil";
    }
    return result;
}

#
# Functional programming A: All function expressions are inherently
# anonymous lambdas and can be used and evaluated mid-expression:
#
# (Note the parens around the function expression.  This is necessary
# because otherwise the parser would read a func following an
# assignment as a "code block" instead of a subexpression.  This is
# the rule that allows you to omit the semicolon at the end of a
# normal function definition.  Oh well, every language has a syntactic
# quirk or two...)
#
var a = (func(n) { n + 1 })(232);  # "a" now equals 233

#
# Functional programming B.  All expressions have a value, the last
# expression in a code block is the return value of that code block.
# There are no "statements" in Nasal, although some expressions
# (assignment, duh) have side effects.  e.g. The "if" expression works
# both for code flow and as the ?: expression in C/C++.
#
var factorial = func(n) { if (n == 0) { 1 }
                      else       { n * factorial(n-1) } }
print(factorial(10), "\n");

#
# Functional programming C: Lexical closures.  Functions "remember"
# the context in which they were defined, and can continue to use the
# local variables in the outer scope even after their creator has
# returned.
#
var getcounter = func { var count = 0; return func { count = count + 1 } }
var mycounter = getcounter();
print(mycounter(), "\n"); # prints 1
print(mycounter(), "\n"); # prints 2
print(mycounter(), "\n"); # prints 3
print(mycounter(), "\n"); # prints 4...

#
# Functional programming D: see the documentation on the functions
# bind(), call(), closure() and caller().  Basically, every piece of
# the symbol lookup environment is inspectable and mutable at runtime.
#
