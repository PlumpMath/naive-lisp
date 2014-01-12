#include "prim_ops.h"
#include "assert.h"

Cell *plus(Cell *args) {
	assert(args);

	//printf("Calling plus\n");
	int sum = 0;
	while(1) {
		//print_cell(args->first, 0);
		sum += args->first->value;
		if(!eq(args->rest, nil)) {
			args = args->rest;
		} else {
			break;
		}
	}
	return make_int(sum);
}