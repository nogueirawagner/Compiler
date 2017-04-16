#ifndef _FUNCTIONS
#define _FUNCTIONS

#include "token_stream.h"
#include "stack.h"
#include "list.h"

/* Verifica se arquivo come�a com main() */
int fn_main(char value, source_t* source);

/* Fun��o gets */
token_t* fn_gets(source_t* source, token_t* last_token);

/* Fun��o puts */
token_t* fn_puts(source_t* source, token_t* last_token);

/* Fun��o for */
token_t* fn_for(source_t* source, token_t* last_token);

/* Processa fun��o for */
void fn_run_for(source_t* source, struct stack_t* stack_token, linked_list_t table_symbols, list_element_t* list_position);

/* Fun��o if */
token_t* fn_if(source_t* source, token_t* last_token);

/* Processa fun��o if */
void fn_run_if(source_t* source, struct stack_t* stack_token, linked_list_t table_symbols, list_element_t* list_position);

#endif // !_FUNCTIONS
