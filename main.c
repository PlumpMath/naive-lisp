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

void test1() {
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	assoc(global_env, make_symbol("a"), make_int(42));
	assoc(global_env, make_symbol("b"), make_int(100));
	assoc(global_env, make_symbol("+"), make_prim_op(plus, "plus"));

	Cell *e = cons(make_symbol("+"), 
		           cons(make_int(20), 
		           	    cons(make_int(30), 
		           	    	 nil)));
	
	print_cell(eval(e, global_env), 0);

	maybe_run_gc();
}

void test2() {
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	assoc(global_env, make_symbol("a"), make_int(42));
	assoc(global_env, make_symbol("b"), make_int(100));

	Cell *sub_env = make_env(global_env);
	assoc(sub_env, make_symbol("a"), make_int(200));

	printf("a = ");
	print_cell(lookup(sub_env, make_symbol("a")), 0);

	printf("b = ");
	print_cell(lookup(sub_env, make_symbol("b")), 0);
}

void test3() {
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	assoc(global_env, make_symbol("+"), make_prim_op(plus, "plus"));

	Cell *l = make_lambda(cons(make_symbol("x"), nil), cons(make_symbol("+"), cons(make_symbol("x"), cons(make_int(1000), nil))), global_env);
	assoc(global_env, make_symbol("f"), l);

	Cell *e = cons(make_symbol("f"), cons(make_int(55), nil));
	print_cell(eval(e, global_env), 0);

	maybe_run_gc();
}

void test4() {
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	assoc(global_env, make_symbol("+"), make_prim_op(plus, "plus"));

	Cell *e = cons(make_symbol("+"), cons(make_int(55), cons(make_int(20), nil)));
	print_cell(eval(e, global_env), 0);

	maybe_run_gc();
}

void test5() {
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	Cell *list = cons(make_int(1), cons(make_int(2), cons(make_int(3), nil)));
	Cell *result = evlis(list, global_env);

	print_cell(result, 0);
}

int main(int argc, char const *argv[])
{
	// test1();
	// test2();
	// test3();
	// test4();
	// test5();
}






