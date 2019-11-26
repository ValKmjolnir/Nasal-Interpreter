# the function print is written by Cpp: std::cout
# the function input is written by Cpp: std::cin
# so the code is fake XD but the definitions are of great use
# the definitions are set to avoid redefinition
# made by github user: ValKmjolnir

# print function use dynamic parameters
var print=func(args...)
{
    return __call_cpp_ostream_std_cout(args);
};

# the return type of input is __var_string
var input=func()
{
    return __call_cpp_istream_std_cin();
};