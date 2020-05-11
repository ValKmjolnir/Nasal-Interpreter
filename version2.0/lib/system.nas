var system=
{
    # print the type of thing on the screen
    type:func(thing)
    {
        nasal_call_builtin_scalar_type(thing);
        return;
    }
};