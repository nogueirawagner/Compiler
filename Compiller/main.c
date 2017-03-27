#include <stdlib.h>
#include <stdio.h>
#include "token_stream.h"
#include "token_stack.h"
#include "stack.h"


int main(int argc, char** argv) {

	source_t * source = ts_open_source("Source.chs"); /* Abre arquivo em binário */
	stack_t stack_token;
	token_t* tt;

	int ret = stack_init(&stack_token);
	if (ret < 0)
	{
		fprintf(stderr, "Falha ao iniciar a stack \n");
	}

	while (1)
	{
		token_type_t type_stat_end;
		token_t * token = ts_get_next_token(source);  /* Pega proximo token */

		/* Insere token na pilha */
		stack_push(&stack_token, token);

		/* Desempilha tokens */
		tt = (token_t*)stack_pop(&stack_token);

	}

error:
	"erro ao processar";
	ts_close_source(source);
	return 0;
}
