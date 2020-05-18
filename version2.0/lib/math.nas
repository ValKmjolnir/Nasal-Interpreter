# nasal lib math.nas
# 2020/2/8
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var math=
{
    # Euler's constant
    e:2.7182818284590452354,
    pi:3.14159265358979323846,
    # Returns the sine of the single argument 
    sin:func(x)
    {
        return nasal_call_builtin_sin(x);
    },
    # Returns the cosine of the single argument
    cos:func(x)
    {
        return nasal_call_builtin_cos(x);
    },
    # you know what the f*ck this is
    tan:func(x)
    {
        return nasal_call_builtin_tan(x);
    },
    # Returns e (Euler's constant) raised to the power specified by the single argument 
    exp:func(x)
    {
        return nasal_call_builtin_exp(x);
    },
    # Returns the natural logarithm of the single argument. 
    ln:func(x)
    {
        return nasal_call_builtin_cpp_math_ln(x);
    },
    # Returns the square root of the single argument. 
    sqrt:func(x)
    {
        return nasal_call_builtin_cpp_math_sqrt(x);
    },
    # Returns the arctangent of y/x, with the correct sign for the quadrant.
    # Wraps the ANSI C function of the same name. 
    atan2:func(x,y)
    {
        return nasal_call_builtin_cpp_atan2(x,y);
    },
};