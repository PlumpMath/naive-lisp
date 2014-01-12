#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Cell *make_env(Cell *root) {
	// root->first = bindings
	// root->rest = parent env
	return make_pair(nil, root);
}

void print_env(Cell *env) {
	assert(env);

	printf("Printing environment:\n");
	env = env->first;
	again:
	if(env->first) {
		print_cell(env->first, 0);
	}
	if(env->rest) {
		env = env->rest;
		goto again;
	}
	printf("\n");
	if(env->rest) {
		printf("parent env:");
		print_env(env->rest);
	}
}

Cell *lookup(Cell *env, Cell *symbol) {
	assert(env);
	assert(symbol);

	Cell *node = env->first;

	if(node == NULL) {
		printf("environment is NULL\n");
		exit(1);
	}

	//printf("Looking up %s in env:\n", symbol->name);
	//print_env(env); 

	again:
	if(node->first && eq(node->first->first, symbol)) {
		return node->first->rest;
	}
	else if(node->rest) {
		node = node->rest;
		goto again;
	}

	if(env->rest) {
		return lookup(env->rest, symbol);
	} else {
		printf("Can't find symbol %s in environment\n", symbol->name);
		return nil;
	}
}

void assoc(Cell *env, Cell *symbol, Cell *value) {
	assert(env);
	assert(symbol);
	assert(value);

	Cell *binding = make_pair(symbol, value);
	env->first = cons(binding, env->first);
}

Cell *extend(Cell *env, Cell *symbols, Cell *values) {
	assert(env);
	assert(symbols);
	assert(values);

	Cell *new_env = make_env(env);

	// printf("symbols: \n");
	// print_cell(symbols, 0);

	// printf("values: \n");
	// print_cell(values, 0);

	while(!eq(symbols, nil) && !eq(values, nil)) {
		assoc(new_env, symbols->first, values->first);
		symbols = symbols->rest;
		values = values->rest;
	}

	return new_env;
}
