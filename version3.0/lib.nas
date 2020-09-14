var print=func(elements...)
{
    nasal_call_builtin_std_cout(elements);
    return nil;
};
var append=func(vector,elements...)
{
    nasal_call_builtin_push_back(vector,elements);
    return nil;
}
var setsize=func(vector,size)
{
    nasal_call_builtin_set_size(vector,size);
    return nil;
}
var system=func(str)
{
    nasal_call_builtin_system(str);
    return;
}
var input=func()
{
    return nasal_call_builtin_input();
}
var sleep=func(duration)
{
    nasal_call_builtin_sleep(duration);
    return;
}
var io=
{
    fin:func(filename)
    {
        return nasal_call_builtin_finput(filename);
    },
    fout:func(filename,str)
    {
        nasal_call_builtin_foutput(filename,str);
        return;
    }
};