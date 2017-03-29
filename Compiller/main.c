#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "list.h"
#include "table_symbols.h"

int main(int argc, char** argv) {

	source_t * source = ts_open_source("Source.chs"); /* Abre arquivo em binário */
	stack_t * stack_token; /* Pilha de Tokens */
	int length_stack = 0;

	list_element_t* list_position = NULL;
	linked_list_t table_symbols;
	list_initialize(&table_symbols, NULL);


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
			tb_add(&stack_token, table_symbols);

		if (source->last_read == -1)
		{
			printf("Fim de analise lexica.\n");
			tb_print_list(table_symbols);
		}

	}

error:
	"erro ao processar";
	ts_close_source(source);
	return 0;
}
