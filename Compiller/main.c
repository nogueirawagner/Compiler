#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "table_symbols.h"
#include "list_symbols.h"
#include "_list.h"



int main(int argc, char** argv) {

	source_t * source = ts_open_source("Source.chs"); /* Abre arquivo em binário */
	stack_t * stack_token; /* Pilha de Tokens */
	int length_stack = 0;

	int ret = stack_init(&stack_token);
	if (ret < 0)
		fprintf(stderr, "Falha ao iniciar a stack \n");

	list_element_t* list_position = NULL;
	linked_list_t table_symbols;
	list_initialize(&table_symbols, NULL);

	symb_node_t* list_symb;
	init_list(&list_symb);

	while (1)
	{

		table_symbols_t* elemento;
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

				if (is_caracter_semicolon(source->last_read))
				{
					stack_t * ids;
					stack_init(&ids);

					token_t* last_tk = (token_t*)stack_pop(&stack_token);
					int count_id = 0;

					while (last_tk->type == TK_ID)
					{
						stack_push(&ids, last_tk);
						last_tk = (token_t*)stack_pop(&stack_token);
						count_id++;
					}

					if (last_tk->type == TK_TYPE)
					{
						while (count_id != 0)
						{
							token_t* id = stack_pop(&ids);

							table_symbols_t* tbs = (table_symbols_t*)malloc(sizeof(table_symbols_t));
							tbs->type = last_tk->id;
							tbs->line = last_tk->line;
							tbs->value = NULL;
							tbs->variable = id->id;
							count_id--;

							list_insert_next(&table_symbols, NULL, tbs);
							push(list_symb, tbs);
						}
					}
				}
		}



			/*	Add na função
				if (is_caracter_semicolon(source->last_read))
					tb_add(&stack_token, &table_symbols, &list_position);
			*/

		if (source->last_read == -1)
		{
			printf("Fim de analise lexica.\n");
		}

	}

error:
	"erro ao processar";
	ts_close_source(source);
	return 0;
}
