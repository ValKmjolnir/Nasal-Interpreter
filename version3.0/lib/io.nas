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
        return nasal_call_builtin_c_fopen(filename,mode);
    },
    # Closes the specified file as per ANSI fclose(). 
    close:func(filehandle)
    {
        nasal_call_builtin_c_fclose(filehandle);
        return;
    },
    # Attempts to read length bytes from the filehandle into the beginning of the mutable string buf.
    # Failures (including overruns when length > size(buf)) are thrown as runtime errors as per die().
    # Returns the number of bytes successfully read. 
    read:func(filehandle,buf,length)
    {
        return nasal_call_builtin_c_read(filehandle,buf,length);
    },
    # Attempts to write the entirety of the specified string to the filehandle.
    # Failures are thrown as runtime errors as per die().
    # Returns the number of bytes successfully written. 
    write:func(filehandle,str)
    {
        return nasal_call_builtin_c_write(filehandle,str);
    },
    # As ANSI fseek().
    # Attempts to seek to the specified position based on the whence value
    # (which must be one of io.SEEK_SET,io.SEEK_END, or io.SEEK_CUR)
    SEEK_SET:1,
    SEEK_CUR:2,
    SEEK_END:3,
    seek:func(filehandle,position,whence)
    {
        nasal_call_builtin_c_seek(filehandle,position,whence);
        return;
    },
    # Returns the current seek position of the filehandle. 
    tell:func(filehandle)
    {
        return nasal_call_builtin_c_tell(filehandle);
    },
    # Reads and returns a single text line from the filehandle.
    # Interprets both "\n" and "\r\n" as end of line markers,
    # and does not include the "\r" or "\n" bytes in the returned string.
    # End offile or error is signaled by returning nil. 
    readln:func(filehandle)
    {
        return nasal_call_builtin_builtin_c_getline(filehandle);
    },
    # Calls unix or win32 stat() on the specified file name and
    # returns a seven element array whose contents are,
    # in order: dev, ino, mode,nlink, uid, gid, rdef, size, atime, mtime, ctime.
    # Errors are signaled as exceptions as per die().
    stat:func(filename)
    {
        return nasal_call_builtin_builtin_stat(filename);
    },
};

var print=func(dyn...)
{
    nasal_call_builtin_std_cout(dyn);
    return nil;
};