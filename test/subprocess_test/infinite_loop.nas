use std.unix;

var count = 0;
while (1) {
    unix.sleep(0.1);
    count += 0.1;
    println("process running time: ", count);
    if (count > 2) {
        die("test failed");
    }
}