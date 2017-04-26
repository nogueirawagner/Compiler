#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "list.h"
#include "token_exception.h"
#include "tb_symbols.h"
#include "functions.h"

int main(int argc, char** argv) {

	source_t* source = ts_open_source("Source.chs");
	stack_t* stack_token;
	int length_stack = 0;

	int ret = stack_init(&stack_token);
	if (ret < 0)
		fprintf(stderr, "Falha ao iniciar a stack \n");

	list_element_t* list_position = (list_element_t*)malloc(sizeof(list_element_t));
	linked_list_t* table_symbols = (linked_list_t*)malloc(sizeof(linked_list_t));
	list_initialize(table_symbols, NULL);

	token_t* last_tk_temp = (token_t*)malloc(sizeof(token_t));
	char* last_type = (char*)malloc(sizeof(char));
	token_type_t last_func = (token_type_t)malloc(sizeof(token_type_t));

	while (1)
	{
		token_t * token = ts_get_next_token(source, last_tk_temp, last_func);

		if (token != NULL && is_token_valid(token, source))
		{
			if (token->type == TK_TYPE)
				last_type = token->id;
			else if (is_token_type_function(token->type))
				last_func = token->type;

			stack_push(&stack_token, token);
			length_stack++;
			last_tk_temp = token;
		}

		if (is_caracter_closed_parathesi(source->last_read) && last_func == TK_FN_FOR)
		{
			//fn_run_for(source, stack_token, table_symbols, list_position);
			last_func = TK_END_FN;
		}

		if (is_caracter_closed_parathesi(source->last_read) && last_func == TK_FN_IF)
		{
			//fn_run_if(source, stack_token, table_symbols, list_position);
			last_func = TK_END_FN;
		}

		if (is_caracter_semicolon(source->last_read) && last_func != TK_FN_FOR)
			insert_table_symbols(source, stack_token, table_symbols, list_position, length_stack, last_func);

		if (source->last_read == -1)
			show_table_symbols(table_symbols, list_position);
	}
}