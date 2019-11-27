# this file includes basic operation functions:
# append
# subvec
# int
# num
# made by github user: ValKmjolnir

# append is used to add a new var into an array/vector
var append=func(vector,elements...)
{
    return __call_special_inline_function(vector,elements);
};

# subvec is used to get a new array from an exist array
var subvec=func(vector,begin,length)
{
    var new_vector=[];
    for(var i=begin;i<begin+length;i+=1)
    {
        append(new_vector,vector[i]);
    }
    return new_vector;
};

# int function is used to transfer double var into int var
# the normal type of number in balloon script is double
var int=func(value)
{
    return __Cpp_inline_var_to_int(value);
};

# num function is used to translate a string into a number
var num=func(value)
{
    return __call_Cpp_type_trans_num(value);
};

# size function is used to count the number of members in an array
var size=func(value)
{
    return __call_Cpp_value_count(value);
};

# rand function is used to make random float numbers between 0 and 1 ([0,1))
# written by C: srand(unsigned(time(NULL))) rand()
var rand=func()
{
    return __call_Cpp_rand();
};