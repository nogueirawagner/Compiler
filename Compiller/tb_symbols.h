#ifndef _TB_SYMBOLS
#define _TB_SYMBOLS

#include <stdlib.h>
#include <stdio.h>
#include "token_stream.h"
#include "list.h"
#include "stack.h"

/* Exibir tabela de simbolos */
void show_table_symbols(linked_list_t tb_list, list_element_t* list_position);

/* Inserir na tabela de simbolos */
void insert_table_symbols(stack_t* stack_token, source_t* source, linked_list_t table_symbols, list_element_t* list_position);


#endif // !_TB_SYMBOLS
