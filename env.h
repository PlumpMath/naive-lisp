#ifndef ENV
#define ENV

#include "gc.h"

Cell *make_env(Cell *root);
void print_env(Cell *env);
Cell *lookup(Cell *env, Cell *symbol);
void assoc(Cell *env, Cell *symbol, Cell *value);
Cell *extend(Cell *env, Cell *symbols, Cell *values);

#endif