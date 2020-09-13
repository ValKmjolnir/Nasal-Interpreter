var system=func(str)
{
    nasal_call_builtin_system(str);
    return;
}
var sleep=func(duration)
{
    nasal_call_builtin_sleep(duration);
    return;
}
var input=func()
{
    return nasal_call_builtin_input();
}