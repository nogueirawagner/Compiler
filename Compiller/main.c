#include <stdlib.h>
#include <stdio.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "list.h"

int main(int argc, char** argv) {

	source_t * source = ts_open_source("Source.chs"); /* Abre arquivo em binário */
	stack_t * stack_token; /* Pilha de Tokens */
	int length_stack = 0;
	token_t * token_list = list_new();
	token_table_symbols_t * table_symbols = list_new();

	

	int ret = stack_init(&stack_token);
	if (ret < 0)
		fprintf(stderr, "Falha ao iniciar a stack \n");

	while (1)
	{
		token_type_t type_stat_end;
		token_t * token = ts_get_next_token(source);  /* Pega proximo token */

		/* Insere token na pilha */
		if (token != NULL) 
		{
			stack_push(&stack_token, token);
			length_stack++;
		}
			

		if (is_caracter_semicolon(source->last_read))
		{
			token_t* tt;
			while (length_stack != 0)
			{
				tt = (token_t*)stack_pop(&stack_token);
				length_stack--;
			}
		}
	}

error:
	"erro ao processar";
	ts_close_source(source);
	return 0;
}
