# this file includes functions:
#   append
#   subvec
var append(vector,elements...)
{
    return __call_special_inline_function(vector,elements);
};

var subvec(vector,begin,length)
{
    var new_vector=[];
    for(var i=begin;i<begin+length;i+=1)
    {
        append(new_vector,vector[i]);
    }
    return new_vector;
};

var int(value)
{
    return __call_Cpp_int(value);
};