# nasal lib thread.nas
# 2020/2/6
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var thread=
{
    newthread:func(){},
    newlock:func(){},
    lock:func(){},
    unlock:func(){},
    newsem:func(){},
    semdown:func(){},
    semup:func(){},
};