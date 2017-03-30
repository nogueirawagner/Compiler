#ifndef _TABLE_SYMBOLS
#define _TABLE_SYMBOLS

#include <stdio.h>
#include "_list.h"
#include "stack.h"

/* Tabela de simbols */
typedef struct {
	char* variable;
	char* type;
	char* value;
	int line;
} table_symbols_t;

void tb_add(stack_t* stack_token, linked_list_t* tbl_symbols, list_element_t* position);

void tb_print_list(linked_list_t* tbl_symbols, list_element_t* position);

#endif // !_TABLE_SYMBOLS