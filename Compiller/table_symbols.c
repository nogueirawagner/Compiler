#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "token_stream.h"
#include "table_symbols.h"

/* Insere elementos na tabela de simbols */
void tb_add(stack_t* stack_token, linked_list_t* tbl_symbols, list_element_t* position)
{
	stack_t * ids;
	stack_init(&ids);

	token_t* last_tk = (token_t*)stack_pop(stack_token);
	int count_id = 0;

	while (last_tk->type == TK_ID)
	{
		stack_push(&ids, last_tk);
		last_tk = (token_t*)stack_pop(stack_token);
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

			if (tbl_symbols->size == 0)
			{
				list_insert_next(tbl_symbols, NULL, tbs);
				position = list_head(tbl_symbols);
			}
			else
			{
				list_insert_next(tbl_symbols, position, tbs);
				position = list_next(position);
			}
		}
	}
}

void tb_print_list(linked_list_t* tbl_symbols, list_element_t* position)
{
	for (int i = 0; i < list_get_size(tbl_symbols); i++)
	{
		printf((linked_list_t*)position->data);
		position = list_next(position);
	}
}