# nasal lib regex.nas
# 2020/2/6
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var regex=
{
    comp:func(){},
    exec:func(){},
};