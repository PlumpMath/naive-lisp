#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "gc.h"
#include "env.h"
#include "eval.h"
#include "prim_ops.h"
#include "tests.h"
#include "reader.h"

FILE *in_file;

bool is_whitespace(char c) {
	return (c == ' ') || (c == '\n') || (c == EOF);
}

char peek() {
	char c = getc(in_file);
	ungetc(c, in_file);
	return c;
}

char *get_token() {
	char *token = malloc(sizeof(char) * 100);
	int i = 0;
	char c = ' ';
	while(1) {
		c = getc(in_file);
		if(is_whitespace(c)) {
			break;
		}
		token[i++] = c;
		if(c == '(') {
			break;
		}
		else if(c == ')') {
			break;
		}
		if((peek() == '(') || (peek() == ')')) {
			break;
		}
	}
	token[i] = '\0';
	return token;
}

Cell *read_list();

Cell *token_to_cell(char *token) {
	if(!strcmp(token, "(")) {
		return read_list();
	}

	if(token[strspn(token, "0123456789")] == '\0') {
		return make_int(atoi(token));
	}
	
	return make_symbol(token);
}

Cell *read() {
	char *token = get_token();
	return token_to_cell(token);
}

Cell *read_list() {
	char *token = get_token();
	if(!strcmp(token, ")")) {
		return nil;
	}
	else {
		Cell *item = token_to_cell(token);
		Cell *list = cons(item, read_list());
		return list;
	}
}

void set_input(FILE *fp) {
	in_file = fp;
}

void test6() {
	init_gc();
	set_input(stdin);
	
	// char *token;
	// while((token = get_token())) {
	// 	printf("Token: %s\n", token);
	// }
	
	Cell *form = read();
	printf("Form: ");
	if(form == NULL) {
		printf("NULL\n");
	} 
	else {
		print_cell(form, 0);
		printf("Type: %c\n", form->type);
	}
}

int main(int argc, char const *argv[])
{
	//run_all_tests();
	test6();
}
