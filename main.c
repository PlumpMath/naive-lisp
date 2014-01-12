#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "gc.h"
#include "env.h"
#include "eval.h"
#include "prim_ops.h"
#include "tests.h"

int main(int argc, char const *argv[])
{
	run_all_tests();
}
