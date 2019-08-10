#ifndef __NASAL_H__
#define __NASAL_H__

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <ctime>
#include <cmath>

namespace nasal
{

#include "nasal_var.h"
#include "nasal_list.h"
#include "nasal_hash.h"
#include "nasal_functional.h"

#include "nasal_var.cpp"
#include "nasal_list.cpp"
#include "nasal_hash.cpp"
#include "nasal_print.h"
#include "nasal_var_stack.h"
#include "nasal_func_stack.h"
#include "nasal_functional.cpp"

#include "nasal_lexer.h"
#include "nasal_parse.h"
}

#endif
