#include "eval.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>

Cell *eval(Cell *exp, Cell *env);

Cell *reverse(Cell *list) {
	Cell *reversed = nil;
	while(!eq(list, nil)) {
		reversed = cons(list->first, reversed);
		list = list->rest;
	}
	return reversed;
}

Cell *evlis(Cell *list, Cell *env) {
	assert(list);
	assert(env);

	Cell *new_list = nil;
	while(!eq(list, nil)) {
		new_list = cons(eval(list->first, env), new_list);
		list = list->rest;
	}

	return reverse(new_list);
}

Cell *eval(Cell *exp, Cell *env) {
	assert(exp);
	assert(env);

	//printf("EVAL:\n");
	//print_cell(exp, 0);

	if(exp->type == 'i' || exp->type == 'o' || exp->type == 'l') {
		return exp;
	}
	else if(exp->type == 's') {
		Cell *val = lookup(env, exp);
		//printf("It was: \n");
		//print_cell(val, 0);
		return val;
	}
	else if(exp->type == 'p') {
		//printf("Proc...\n");
		Cell *first = eval(exp->first, env);
		//printf("Args\n");
		Cell *args = evlis(exp->rest, env);
		if(first->type == 'o') {
			//printf("It's a prim_op\n");
			proc *f = first->prim_op;
			Cell *result = f(args);
			return result;
		}
		else if (first->type == 'l') {
			//printf("It's a lambda\n");
			Lambda *l = first->lambda;
			Cell *lambda_env = extend(env, l->params, args);
			Cell *result = eval(l->body, lambda_env);
			return result;
		}
		else {
			printf("Not a function:\n");
			print_cell(first, 0);
		}
	}
	return nil;
}