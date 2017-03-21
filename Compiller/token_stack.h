#ifndef _TOKEN_STACK
#define _TOKEN_STACK

#include <stdio.h>

/******************************************************
Função para iniciar a pilha de simbolos
*/
void ts_init_stack_tokens(token_stack_t * stack);
/******************************************************/

/******************************************************
Função para inserir dados na pilha de Token
*/
void ts_push_stack_tokens(token_stack_t * stack, token_t * token);
/******************************************************/



#endif // !_TOKEN_STREAM
