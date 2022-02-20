import("lib.nas");

var libkey=func(){
    var lib=dylib.dlopen("./module/libkey"~(os.platform()=="windows"?".dll":".so"));
    var kb=dylib.dlsym(lib,"nas_kbhit");
    var gt=dylib.dlsym(lib,"nas_getch");
    var nb=dylib.dlsym(lib,"nas_noblock");
    var init=dylib.dlsym(lib,"nas_init");
    var cls=dylib.dlsym(lib,"nas_close");
    var call=dylib.dlcall;
    var is_init=0;
    return {
        init:func(){
            # change io mode to no echo
            call(init);
            is_init=1;
        },
        kbhit:func(){
            # check if kerboard is hit
            # if keyboard is hit this function will return 1
            # until getch() gets all the input characters
            # and the input flow becomes empty
            if(!is_init)
                me.init();
            return call(kb);
        },
        getch:func(){
            # get input one character without echo
            # block until get one input
            if(!is_init)
                me.init();
            return call(gt);
        },
        nonblock:func(){
            # nonblock input without echo
            if(!is_init)
                me.init();
            return call(nb);
        },
        close:func(){
            # must call this function before exiting the program
            # this will change terminal mode to normal io mode
            call(cls);
            dylib.dlclose(lib);
        }
    }
}();