#ifndef _FUNCTIONS
#define _FUNCTIONS

#include "token_stream.h"
#include "stack.h"
#include "list.h"

/* Verifica se arquivo começa com main() */
int fn_main(char value, source_t* source);

/* Função gets */
token_t* fn_gets(source_t* source, token_t* last_token);

/* Função puts */
token_t* fn_puts(source_t* source, token_t* last_token);

/* Função for */
token_t* fn_for(source_t* source, token_t* last_token);

/* Processa função for */
void fn_run_for(source_t* source, struct stack_t* stack_token, linked_list_t table_symbols, list_element_t* list_position);

/* Função if */
token_t* fn_if(source_t* source, token_t* last_token);

/* Processa função if */
void fn_run_if(source_t* source, struct stack_t* stack_token, linked_list_t table_symbols, list_element_t* list_position);

#endif // !_FUNCTIONS
