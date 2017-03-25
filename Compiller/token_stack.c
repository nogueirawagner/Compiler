#include "token_stream.h";
#include "token_stack.h";
#include <stdio.h>;
#include <stdlib.h>;

/* Aloca a pilha de tokens na memoria */
token_stack_t * ts_alloc_stack(token_t * token)
{
	token_stack_t * new_element = (token_stack_t*)malloc(sizeof(token_stack_t));
	if (!new_element) {
		printf("TS001 - Sem memória disponível");
		exit(1);
	}
	else 
	{
		new_element->token = token;
		return new_element;
	}
}

/* Inicializa pilha de tokens */
void ts_init_stack_tokens(token_stack_t * stack)
{
	stack->next_token = NULL;
}

/* Insere dados na pilha de tokens */
token_stack_t * ts_push_stack_tokens(token_stack_t * stack, token_t * token)
{
	if (!stack) {		
		return ts_alloc_stack(token);
	}
	else 
	{
		token_stack_t * new_element = ts_alloc_stack(token);
		new_element->next_token = stack;
		return new_element;
	}
}

/* Verifica se a pilha está vazia */
int ts_stack_empty(token_stack_t * stack)
{
	if (stack->next_token == NULL)
		return 1;
	else
		return 0;
}
