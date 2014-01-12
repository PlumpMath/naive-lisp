#ifndef TESTS
#define TESTS

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
	
	Cell *result = eval(e, global_env);
	print_cell(result, 0);

	assert(result->value == 50);

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
	Cell *result = eval(e, global_env);
	print_cell(result, 0);

	assert(result->value == 1055);

	maybe_run_gc();
}

void test4() {
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	assoc(global_env, make_symbol("+"), make_prim_op(plus, "plus"));

	Cell *e = cons(make_symbol("+"), cons(make_int(55), cons(make_int(20), nil)));
	Cell *result = eval(e, global_env);
	print_cell(result, 0);

	assert(result->value == 75);

	maybe_run_gc();
}

void test5() {
	init_gc();
	
	Cell *global_env = make_env(nil);
	add_to_root(global_env);

	Cell *list = cons(make_int(1), cons(make_int(2), cons(make_int(3), nil)));
	Cell *result = evlis(list, global_env);

	assert(result->first->value == 1);	
	assert(result->rest->first->value == 2);
	assert(result->rest->rest->first->value == 3);

	print_cell(result, 0);
}

void test_gc() {
	Cell *root = init_gc();
	make_int(10);
	root->first = make_int(20);
	root->rest = make_pair(make_int(1), make_int(2));
	print_cell(root, 0);
	printf("\n");
	maybe_run_gc();
}

void run_all_tests() {
	test_gc();
	test1();
	test2();
	test3();
	test4();
	test5();
}

#endif