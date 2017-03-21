#include <stdio.h>
#include "token_stream.h"
#include "token_stack.h"

int main(int argc, char** argv) {
	
	source_t * source = ts_open_source("Source.chs"); /* Abre arquivo em binário */
	token_stack_t * ts_stack = (token_stack_t*)malloc(sizeof(token_stack_t)); /* Pilha de Token */
	
	if (!ts_stack) {
		printf("Sem memória disponível\n");
		exit(1);
	}
	ts_init_stack_tokens(ts_stack); /* Inícia Pilha */

	token_type_t types[5] = { TK_TYPE, TK_ID, TK_EQUAL, TK_CONST, TK_STM_END };
	int i = 0;
	while (1) 
	{
		token_type_t type_stat_end;
		token_t * token = ts_get_next_token(source);  /* Pega proximo token */

		/* Insere token na pilha */
		ts_push_stack_tokens(ts_stack, token);


		i++;
		//ts_next_token = (token_list_t*)malloc(sizeof(token_list_t));
		//ts_save_tokens_table_symbols(ts_next_token, &tk);

		//if (!ts_is_token_type(tk, types[i]))
		//	goto error;
		//i++;
	}

error:
	"erro ao processar";

	ts_close_source(source);
	return 0;
}
