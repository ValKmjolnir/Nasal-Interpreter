# nasal lib base.nas
# 2020/2/4
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

# append
# The first argument specifies a vector.
# Appends the remaining arguments to the end of the vector. 
var append=func(vector,elements...)
{
    var call_inline_push_back=func(vector,elements){};
    call_inline_push_back(vector,elements);
    return;
}

# setsize
# Sets the size of a vector.
# The first argument specifies a vector, the second a number representing the desired size of that vector.
# If the vector is currently larger than the specified size,it is truncated.
# If it is smaller, it is padded with nil entries.Returns the vector operated upon. 
var setsize=func(vector,__size)
{
    var call_inline_push_null=func(vector,__size){};
    call_inline_push_null(vector,__size);
    return;
}

# subvec
# Returns a sub-range of a vector.
# The first argument specifies a vector, the second a starting index,
# and the optional third argument indicates a length (the default is to the end of the vector).
var subvec=func(vector,start,length=nil)
{
    var call_inline_subvec=func(vector,start,length=nil){};
    return call_inline_subvec(vector,start,length);
}

# contains
# The first argument specifies a hash, the second must be a scalar.
# Returns 1 if the hash contains the scalar as a key, 0 if not.
var contains=func(hash,key)
{
    var call_inline_contains=func(hash,key){};
    return call_inline_contains(hash,key);
}

# delete
# The first argument specifies a hash, the second must be a scalar key.
# Deletes the key from the hash if it exists.
# Operationally, this is identical to setting the hash value specified by the key to nil,
# but this variant potentially frees storage by deleting the reference to the key and by shrinking the hash.
var delete=func(hash,key)
{
    var call_inline_delete=func(hash,key){};
    call_inline_delete(hash,key);
    return;
}

# int
# Returns the integer part of the numeric value of the single argument, or nil if none exists.
# Truncates towards zero, not negative infinity (i.e. it's implemented in C as a double tointeger typecast).
var int=func(value)
{
    var inline_trans_int=func(value){};
    return inline_trans_int(value);
}

# num
# Returns the numeric value of the single argument, or nil if none exists.
var num=func(value)
{
    var inline_trans_num=func(value){};
    return inline_trans_num(value);
}

# keys
# Returns a vector containing the list of keys found in the single hash argument.
var keys=func(hash)
{
    var inline_get_keys=func(hash){};
    return inline_get_keys(hash);
}

# pop
# Removes and returns the last element of the single vector argument.
var pop=func(vector)
{
    var inline_get_back=func(vector){};
    var inline_pop_back=func(vector){};
    var ret=inline_get_back(vector);
    inline_pop_back(vector);
    return ret;
}

# size
# Returns the size of the single argument.
# For strings, this is the length in bytes.
# For vectors, this is the number of elements.
# For hashes, it is the number of key/value pairs.
# Returns nil for number and nil arguments.
var size=func(object)
{
    var call_inline_sizeof=func(object){};
    return call_inline_sizeof(object);
}

# streq
# Tests the string values of the two arguments for equality.
# Needed because the == operator in Nasal tests for numeric equality, as in perl.
# So "0" == "0.0" is true,but streq("0", "0.0") is false.
# This is rarely required in typical code. 
var streq=func(__a,__b)
{
    var inline_str_cmp_equal=func(str1,str2){};
    return inline_str_cmp_equal(__a,__b);
}

# cmp
# Compares two strings, returning -1 if a is less than b, 0 if theyare identical, and 1 if a is greater than b. 
var cmp=func(__a,__b)
{
    var inline_cmp=func(var1,var2){};
    return inline_cmp(__a,__b);
}

# sort
# Creates a new vector containing the elements in the input vector sorted in ascending order according to the rule givenby function,
# which takes two arguments (elements of the input vector) and should return less than zero, zero, or greater than zero if the first argument is,
# respectively, less than, equal to, or greater than the second argument. Despite being implemented with ANSI C qsort(),
# the sort is stable; "equal" elements in the output vector will appear in the same relative order as they do in the input. 
var sort=func(vector,function)
{
    var call_cpp_sort=func(vector,function){};
    call_cpp_sort(vector,function);
    return;
}

# substr
# Computes a substring.
# The first argument specifes a string, the second is an integer index of the start of a substring,
# the optional third argument specifies a length (the default is to return the remaining string).
# Example: substr("abcde", 1, 3) returns "bcd". 
var substr=func(__string,start,length=nil)
{
    var call_inline_substr=func(__string,start,length){};
    return call_inline_substr(__string,start,length);
}

# sprintf
# Creates and returns a string formatted as per ANSI C sprintf(). 
var sprintf=func(__format,var_args...)
{
    var call_inline_sprintf=func(__format,var_args){};
    return call_inline_sprintf(__format,var_args);
}

# find
# Finds and returns the index of the first occurence of the string needle in the string haystack, or -1 if no such occurence was found.
var find=func(needle,haystack)
{
    var inline_find_first_occur=func(needle,haystack){};
    return inline_find_first_occur(needle,haystack);
}

# split
# Splits the input string into a vector of substrings bounded by occurences of the delimeter substring. 
var split=func(delimeter,__string)
{
    var inline_split=func(delimeter,__string){};
    return inline_split(delimeter,__string);
}

# rand
# Returns a random number in the range [0:1) (that is, 0.0 is a possible return value. 1.0 is not).
# If a numeric argument is specified, it is used as a seed instead and the function returns nil.
# Implemented in terms of the C library's rand/srand functions;
# the result should have a full double-precision number's worth of randomness even on systems with a 15 bit rand(). 
var rand=func(seed=nil)
{
    var inline_rand=func(seed){};
    return inline_rand(seed);
}

# id
# Returns a unique string identifying the object.
# Two id strings are equal if and only if the two references point to the same object in memory.
# Numbers don't have id's and will cause a runtime error if passed to id(). 
var id=func(thing)
{
    var inline_getid=func(thing){};
    return inline_getid(thing);
}