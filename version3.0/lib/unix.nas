# nasal lib unix.nas
# 2020/2/6
# this file is used to avoid name confliction
# and is used to avoid name undefined
# before running this file will be translated to abstract syntax tree
# and this ast will be linked before main ast as main-ast's beginning

var unix=
{
    pipe:func(){},
    fork:func(){},
    dup2:func(){},
    exec:func(){},
    waitpid:func(){},
    opendir:func(){},
    readdir:func(){},
    closedir:func(){},
    time:func(){},
    chdir:func(){},
    environ:func(){},
    sleep:func(){},
};