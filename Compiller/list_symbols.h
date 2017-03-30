#ifndef _LIST_SYMBOLS
#define _LIST_SYMBOLS

#include "table_symbols.h"

typedef struct symb_node_t {
	table_symbols_t* data;
	struct symb_node_t * next;
	int size;
} symb_node_t;

void init_list(symb_node_t* symbol_list);
void add_list(symb_node_t* symbol_list, table_symbols_t tbs);

/* Iterating over a list */
void print_list(symb_node_t * head);

/* Adding an item to the end of the list */
void push(symb_node_t * head, table_symbols_t* tbs);

/* Adding an item to the beginning of the list (pushing to the list) */
void push_begin(symb_node_t ** head, table_symbols_t* tbs);

/* Removing the first item (popping from the list) */
table_symbols_t* pop(symb_node_t ** head);

/* Removing the last item of the list */
table_symbols_t* remove_last(symb_node_t * head);

/* Removing a specific item */
table_symbols_t* remove_by_index(symb_node_t ** head, int n);


#endif // !_LIST_SYMBOLS
