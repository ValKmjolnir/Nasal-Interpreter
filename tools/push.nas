
println("[",os.time(),"] (=.=) auto push, please wait...");

while(system("git push")!=0) {
    println("[",os.time(),"] (ToT) failed to push, retrying...");
    unix.sleep(0.5);
}

println("[",os.time(),"] (^o^) auto push complete.");
