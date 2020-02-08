# nasal lib bits.nas
# 2020/2/8
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var bits=
{
    # Interpreting the string str as bits,
    # returns the bitfield of the specified length starting at startbit.
    # Interprets the result as an unsigned integer.
    # The bit order is bytewise big endian: The 0th bit is the high bit of the first byte.
    # The last bit is the low bit of the last byte in the string. 
    fld:func(__string,startbit,length)
    {
        var call_built_in_bitcalc=func(__str,__start,__len){};
        return call_built_in_bitcalc(__string,startbit,length);
    },
    # As bits.fld(), but interprets the result as a 2's complement signed value. 
    sfld:func(__string,startbit,length)
    {
        var call_built_in_sbitcalc=func(__str,__start,__len){};
        return call_built_in_sbitcalc(__string,startbit,length);
    },
    # Sets the specified value into the bit string at the specified position.
    # The string must be mutable: either the result of a runtime concatenation (the ~ operator) or a call to bits.buf()(see below).
    # Attempts to modify immutable strings (e.g. compile time constants) will produce a runtime error. 
    setfld:func(__string,startbit,length,value)
    {
        var call_built_in_setbit=func(__str,__start,__len,__val){};
        return call_built_in_setbit(__string,startbit,length,value);
    },
    # Returns a zero-filled mutable string of the specified length. 
    buf:func(length)
    {
        var call_built_in_null_string_gen=func(__len){};
        return call_built_in_null_string_gen(length);
    },
};