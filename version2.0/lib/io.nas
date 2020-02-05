# nasal lib io.nas
# 2020/2/4
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var input=func(filename="")
{
    if(filename=="")
        return __system_call_cpp_istream();
    else
        return __system_call_cpp_ifstream(filename);
};

var print=func(dyn...)
{
    forindex(var i;dyn)
        __system_call_c_puts(dyn[i]);
    return nil;
};

var fprint=func(arr,filename="")
{
    if(filename=="")
    {
        print("__fprint didn't get a correct file name\n");
        return false;
    }
    else
    {
        __system_call_cpp_ofstream(arr,filename);
        return true;
    }
    return;
}