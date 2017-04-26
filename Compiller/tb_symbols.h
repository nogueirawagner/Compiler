#ifndef _TB_SYMBOLS
#define _TB_SYMBOLS

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "list.h"
#include "token_exception.h"
#include "tb_symbols.h"
#include "functions.h"

/* Exibir tabela de simbolos */
void show_table_symbols(linked_list_t* tb_list, list_element_t* list_position);

/* Inserir na tabela de simbolos */
void insert_table_symbols(source_t* source, struct stack_t* stack_token, linked_list_t* table_symbols, list_element_t* list_position, token_type_t last_func);

/* Valida funcao gets */
void function_gets();

#endif // !_TB_SYMBOLS
