#ifndef EVAL
#define EVAL

#include "assert.h"
#include "gc.h"
#include "env.h"

Cell *eval(Cell *exp, Cell *env);
Cell *reverse(Cell *list);
Cell *evlis(Cell *list, Cell *env);
Cell *eval(Cell *exp, Cell *env);

#endif