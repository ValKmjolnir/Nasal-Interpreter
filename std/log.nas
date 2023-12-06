# log.nas
# ValKmjolnir 2022/6/14
use std.os;

var log = func() {
    var (log_date,log_time,prefix)=(1,1,"");
    var os_time="";
    var prt_core = func(elem) {
        os_time=os.time();
        print(prefix," ");
        if (log_date and log_time)
            print(os_time," ");
        elsif (log_date or log_time) {
            var s=split(" ",os_time);
            if (log_date)
                print(s[0]," ");
            if (log_time)
                print(s[1]," ");
        }
        foreach(var i;elem)
            print(i);
        print("\n");
    }
    return {
        setflags:func(date,time) {
            log_date=!!date;
            log_time=!!time;
        },
        setprefix:func(s) {
            if (typeof(s)!="str")
                println("[log.nas] must use string as the prefix.");
            prefix=s;
        },
        println:func(elem...) {
            prt_core(elem);
        },
        fatalln:func(elem...) {
            prt_core(elem);
            die("log:fatal error");
        }
    };
}();
