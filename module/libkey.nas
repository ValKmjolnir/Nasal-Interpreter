import("lib.nas");

var libkey=func(){
    var lib=dylib.dlopen("./module/libkey"~(os.platform()=="windows"?".dll":".so"));
    var kb=dylib.dlsym(lib,"nas_kbhit");
    var gt=dylib.dlsym(lib,"nas_getch");
    var nb=dylib.dlsym(lib,"nas_noblock");
    var init=dylib.dlsym(lib,"nas_init");
    var cls=dylib.dlsym(lib,"nas_close");
    var is_init=0;
    return {
        init:func(){
            dylib.dlcall(init);
            is_init=1;
        },
        kbhit:func(){
            if(!is_init)
                me.init();
            return dylib.dlcall(kb);
        },
        getch:func(){
            if(!is_init)
                me.init();
            return dylib.dlcall(gt);
        },
        nonblock:func(){
            if(!is_init)
                me.init();
            return dylib.dlcall(nb);
        },
        close:func(){
            # must call this function before exiting the program
            dylib.dlcall(cls);
            dylib.dlclose(lib);
        }
    }
}();