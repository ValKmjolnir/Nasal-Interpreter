# nasal lib io.nas
# 2020/2/8
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var io=
{
    # Opens the file with the specified mode (as per ANSI fopen()) and returns a ghost object representing the filehandle.
    # Failures are thrown as runtime errors as per die(). 
    open:func(filename,mode="r")
    {
        var call_c_fopen=func(__file,__mode){};
        return call_c_fopen(filename,mode);
    },
    # Closes the specified file as per ANSI fclose(). 
    close:func(filehandle)
    {
        var call_c_fclose=func(__filehandle){};
        call_c_fclose(filehandle);
        return;
    },
    # Attempts to read length bytes from the filehandle into the beginning of the mutable string buf.
    # Failures (including overruns when length > size(buf)) are thrown as runtime errors as per die().
    # Returns the number of bytes successfully read. 
    read:func(filehandle,buf,length)
    {
        var call_c_read=func(__filehandle,__buf,__len){};
        return call_c_read(filehandle,buf,length);
    },
    # Attempts to write the entirety of the specified string to the filehandle.
    # Failures are thrown as runtime errors as per die().
    # Returns the number of bytes successfully written. 
    write:func(filehandle,str)
    {
        var call_c_write=func(__filehandle,__str){};
        return call_c_write(filehandle,str);
    },
    # As ANSI fseek().
    # Attempts to seek to the specified position based on the whence value
    # (which must be one of io.SEEK_SET,io.SEEK_END, or io.SEEK_CUR)
    SEEK_SET:1,
    SEEK_CUR:2,
    SEEK_END:3,
    seek:func(filehandle,position,whence)
    {
        var call_c_seek=func(__filehandle,__position,__whence){};
        call_c_seek(filehandle,position,whence);
        return;
    },
    # Returns the current seek position of the filehandle. 
    tell:func(filehandle)
    {
        var call_c_tell=func(__filehandle){};
        return call_c_tell(filehandle);
    },
    # Reads and returns a single text line from the filehandle.
    # Interprets both "\n" and "\r\n" as end of line markers,
    # and does not include the "\r" or "\n" bytes in the returned string.
    # End offile or error is signaled by returning nil. 
    readln:func(filehandle)
    {
        var call_builtin_c_getline=func(__filehandle){};
        return call_builtin_c_getline(filehandle);
    },
    # Calls unix or win32 stat() on the specified file name and
    # returns a seven element array whose contents are,
    # in order: dev, ino, mode,nlink, uid, gid, rdef, size, atime, mtime, ctime.
    # Errors are signaled as exceptions as per die().
    stat:func(filename)
    {
        var call_builtin_stat=func(__filename){};
        return call_builtin_stat(filename);
    },
};

var print=func(dyn...)
{
    var __system_call_c_std_puts=func(__str){};
    forindex(var i;dyn)
        __system_call_c_std_puts(dyn[i]);
    return nil;
};