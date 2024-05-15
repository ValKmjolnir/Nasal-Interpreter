use std.math;

var REPS = 10;
var COUNT = 16384;

print("Initializing...\n");
var v = [];
for(var i=0; i<COUNT; i=i+1) { append(v, {}); }


print("Starting test\n");
func {
    for(var rep=0; rep<REPS; rep+=1) {
        print(rep, "...\n");
        for(var i=0; i<COUNT; i=i+1) {
            var obj = v[i];
            obj.fielda = i;
            obj.fieldb = i;
            obj.fieldc = i;
            obj.fieldd = i;
        }
        for(var i=0; i<COUNT; i=i+1) {
            var obj = v[i];
            if (obj.fielda != i) { print("Ack!\n"); return; }
            if (obj.fieldb != i) { print("Ack!\n"); return; }
            if (obj.fieldc != i) { print("Ack!\n"); return; }
            if (obj.fieldd != i) { print("Ack!\n"); return; }
            
            if (obj.fielda != i) { print("Ack!\n"); return; }
            if (obj.fieldb != i) { print("Ack!\n"); return; }
            if (obj.fieldc != i) { print("Ack!\n"); return; }
            if (obj.fieldd != i) { print("Ack!\n"); return; }
            
            if (obj.fielda != i) { print("Ack!\n"); return; }
            if (obj.fieldb != i) { print("Ack!\n"); return; }
            if (obj.fieldc != i) { print("Ack!\n"); return; }
            if (obj.fieldd != i) { print("Ack!\n"); return; }
            
            if (obj.fielda != i) { print("Ack!\n"); return; }
            if (obj.fieldb != i) { print("Ack!\n"); return; }
            if (obj.fieldc != i) { print("Ack!\n"); return; }
            if (obj.fieldd != i) { print("Ack!\n"); return; }
            
            if (obj.fielda != i) { print("Ack!\n"); return; }
            if (obj.fieldb != i) { print("Ack!\n"); return; }
            if (obj.fieldc != i) { print("Ack!\n"); return; }
            if (obj.fieldd != i) { print("Ack!\n"); return; }
            
            if (obj.fielda != i) { print("Ack!\n"); return; }
            if (obj.fieldb != i) { print("Ack!\n"); return; }
            if (obj.fieldc != i) { print("Ack!\n"); return; }
            if (obj.fieldd != i) { print("Ack!\n"); return; }
            
            if (obj.fielda != i) { print("Ack!\n"); return; }
            if (obj.fieldb != i) { print("Ack!\n"); return; }
            if (obj.fieldc != i) { print("Ack!\n"); return; }
            if (obj.fieldd != i) { print("Ack!\n"); return; }
        }
}
}();

print("exp(1) = ", math.exp(1), "\n");

print("e = ", math.e, "\n");
print("pi = ", math.pi, "\n");

var angle = 32 * math.pi/180;
print("sin(32) = ", math.sin(angle), "\n");
print("cos(32) = ", math.cos(angle), "\n");
print("s^2 + c^s = ",
       math.sin(angle)*math.sin(angle)+math.cos(angle)*math.cos(angle), "\n");
    
func {for(var j=0; j<10; j=j+1) {
    print(j, "/10\n");

    # Make some tables to store stuff.  This will clobber the contents
    # from the previous loop, making them available for garbage
    # collection.
    var v = [];   var h1 = {};   var h2 = {};

    # Fill them
    for(var i=0; i<65536; i=i+1) {
        var str = "i" ~ i;
        append(v, str);
        h1[str] = i;
        h2[i~""] = [str];
    }

    # Check that we get back what we put in
    for(i=0; i<65536; i=i+1) {
        if (v[i] != h2[h1[v[i]~""]~""][0]) {
            print("Ack!\n");
            return;
        }
    }
}}();