#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "gc.h"

Cell *nil;

void test_gc() {
	Cell *root = init_gc();
	make_int(10);
	root->first = make_int(20);
	root->rest = make_pair(make_int(1), make_int(2));
	print_cell(root, 0);
	printf("\n");
	maybe_run_gc();
}

Cell *cons(Cell *value, Cell *list) {
	return make_pair(value, list);
}

// ENVIRONMENT

Cell *make_env(Cell *root) {
	// root->first = bindings
	// root->rest = parent env
	return make_pair(nil, root);
}

Cell *extend_env(Cell *parent, Cell *symbols, Cell *values) {
	return parent;
}

Cell *lookup(Cell *env, Cell *symbol) {
	Cell *node = env->first;
	again:
	if(node->first && eq(node->first->first, symbol)) {
		return node->first->rest;
	}
	if(node->rest) {
		node = node->rest;
		goto again;
	}
	printf("Can't find symbol %s in environment\n", symbol->name);
	return nil;
}

void print_env(Cell *env) {
	printf("Printing environment:\n");
	again:
	if(env->first) {
		print_cell(env->first, 0);
	}
	if(env->rest) {
		env = env->rest;
		goto again;
	}
	printf("\n");
}

void assoc(Cell *old_env, Cell *symbol, Cell *value) {
	Cell *binding = make_pair(symbol, value);
	old_env->first = cons(binding, old_env->first);
}

Cell *eval(Cell *exp, Cell *env);

Cell *evlis(Cell *list, Cell *env) {
	//Cell *new_list = nil;
	//new_list = cons(eval(list->first, env)
	return list;
}

Cell *eval(Cell *exp, Cell *env) {
	if(exp->type == 'i') {
		return exp;
	}
	else if(exp->type == 's') {
		Cell *val = lookup(env, exp);
		return val;
	}
	else if(exp->type == 'p') {
		proc *f = eval(exp->first, env)->prim_op;
		Cell *args = evlis(exp->rest, env);
		//printf("Args:\n");
		//print_cell(args, 0);
		Cell *result = f(args);
		return result;
	}
	return nil;
}

Cell *plus(Cell *args) {
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

int main(int argc, char const *argv[])
{
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	assoc(global_env, make_symbol("a"), make_int(42));
	assoc(global_env, make_symbol("b"), make_int(100));
	assoc(global_env, make_symbol("+"), make_prim_op(plus, "plus"));

	// printf("Root: ");
	// print_cell(get_root());
	// printf("\n");

	//print_env(global_env);

	// printf("a\n");
	// print_cell(eval(make_symbol("a"), global_env));
	// printf("\nb\n");
	// print_cell(eval(make_symbol("b"), global_env));
	// printf("\n");

	Cell *e = cons(make_symbol("+"), 
		           cons(make_int(20), 
		           	    cons(make_int(30), 
		           	    	 nil)));
	print_cell(eval(e, global_env), 0);
	//print_cell(nil, 0);

	maybe_run_gc();
}
