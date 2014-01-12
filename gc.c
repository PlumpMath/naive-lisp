#include "gc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

static Cell *lastAllocatedCell;
static int cell_count;
static int max_cell_count;
static Cell *root;

void print_cell(Cell *cell, int depth) {
	assert(cell);

	if(cell == nil) {
		printf("nil");
	}
	else if(cell->type == 'p') {
		printf("(");
		print_cell(cell->first, depth + 1);
		printf(" . ");
		print_cell(cell->rest, depth + 1);
		printf(")");
	}
	else if(cell->type == 'i') {
		printf("%i", cell->value);
	}
	else if(cell->type == 's') {
		printf("%s", cell->name);
	}
	else if(cell->type == 'o') {
		printf("prim_op<%s>", cell->name);
	}
	else if(cell->type == 'l') {
		printf("[λ");
		print_cell(cell->lambda->params, depth);
		print_cell(cell->lambda->body, depth);
		printf("]");
	}

	if(depth == 0) {
		printf("\n");
	}
}

void mark(Cell *cell) {
	if(!cell->collect) return;

	cell->collect = false;
	if(cell->type == 'p'){
		if(cell->first) mark(cell->first);
		if(cell->rest) mark(cell->rest);	
	}
}

void sweep() {
	Cell** node = &lastAllocatedCell;

	while(*node) {
		//printf("Time to sweep %p with next node %p\n", *node, (*node)->next);
		if((*node)->collect) {
			//printf("Free %p ", *node);
			//print_cell(*node, 0);
			//printf("\n");
			Cell *nextnode = *node;
			*node = nextnode->next;
			free(nextnode);
			cell_count--;
		} else { 
			(*node)->collect = true;
			node = &(*node)->next;
		}
	}
}

void gc() {
	printf("Collecting garbage!\n");
	mark(root);
	sweep();
}

void maybe_run_gc() {
	printf("GC?\n");
	if(cell_count > max_cell_count) {
		printf("Cell count too high (%d)\n", cell_count);
		gc();
		printf("New cell count is %d\n", cell_count);
		max_cell_count = max_cell_count * 2;
		printf("Increased max cell count to %i \n", max_cell_count);
	}
}

Cell* make_Cell( char pType ) {
	Cell* cell = (Cell*)malloc(sizeof(Cell));

	assert(cell);

	cell->collect = true;
	cell->type = pType;
	cell->next = lastAllocatedCell;

	lastAllocatedCell = cell;
	cell_count++;

	if(pType == 'p') {
	printf("%s", KGRN);
	} else {
	printf("%s", KCYN);
	}
	//printf("Alloc %c: %p\n", pType, cell);
	printf("%s", KNRM);

	return cell;
}

Cell* make_pair( Cell *pfirst, Cell *prest){
	Cell* o = make_Cell('p');
	o->first = pfirst;
	o->rest = prest;

	return o;
}

Cell* make_int(int pValue) {
	Cell* o = make_Cell('i');
	o->value = pValue;

	return o;
}

Cell* make_symbol(char *name) {
	Cell* o = make_Cell('s');
	o->name = name;

	return o;
}

Cell* make_prim_op(proc f, char *name) {
	Cell* o = make_Cell('o');
	o->prim_op = f;
	o->name = name;

	return o;	
}

Cell* make_lambda(Cell *params, Cell *body, Cell *env) {
	Cell* o = make_Cell('l');

	Lambda *l = (Lambda*)malloc(sizeof(Lambda));
	l->params = params;
	l->body = body;
	l->env = env;

	o->lambda = l;

	return o;
}

Cell* init_gc() {
	max_cell_count = 1;
	cell_count = 0;
	lastAllocatedCell = NULL;
	root = make_pair(NULL, NULL);
	nil = make_pair(NULL, NULL);
	add_to_root(nil);
	return root;
}

void add_to_root(Cell *cell) {
	root = make_pair(cell, root);
}

Cell *get_root() {
	return root;
}

bool eq(Cell *a, Cell *b) {
	if(a == b) {
		return true;
	}
	else if(a == NULL || b == NULL) {
		return false;
	}
	else if(a->type != b->type) {
		return false;
	}
	else if(a->type == 'i') {
		return a->value == b->value;
	}
	else if(a->type == 's') {
		return a->name == b->name;
	}
	else if(a->type == 'p') {
		return eq(a->first, b->first) && eq(a->rest, b->rest);
	}

	return false;
}