#nasal-strict-lib thread

var thread=func(__function)
{
    __system_call_cpp_new_thread(__function);
    return 0;
}