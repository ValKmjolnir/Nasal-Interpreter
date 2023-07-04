
println("[",os.time(),"] auto push, please wait...");

while(system("git push")!=0) {
    println("[",os.time(),"] failed to push, retrying...");
    unix.sleep(0.5);
}
