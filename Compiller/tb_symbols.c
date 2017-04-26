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

void show_table_symbols(linked_list_t* tb_list, list_element_t* position)
{
	printf("Tabela de simbolos \n");
	printf("\n");
	printf("\t%-3s\t| %-15s\t| %-2s\t| %-20s\t|\t %-10s\n", "TIPO", "VARIAVEL", "TAM", "VALOR", "LINHA");
	printf("\t------------------------------------------------------------------------------------");
	printf("\n");
	for (int i = 0; i < list_get_size(tb_list); i++)
	{
		table_symbols_t* object = (table_symbols_t*)position->data;

		char* variable = (char*)object->variable;
		char* tipo = (char*)object->type;
		char* value = (char*)object->value;
		char* length = (char*)object->length;
		int line = object->line;

		if (object->enable)
			printf("\t%-3s\t| %-15s\t| %-2s\t| %-20s\t|\t %-10i\n", tipo, variable, length, value, line);
		position = list_next(position);
	}
}

void insert_table_symbols(source_t* source, struct stack_t* stack_token, linked_list_t* tbl_symbols, list_element_t* position, token_type_t last_func)
{
	stack_t* ids;		/* variaveis */
	stack_t* constants; /* atribuicoes de valores */
	stack_t* functions; /* funções */
	stack_init(&ids);
	stack_init(&constants);
	stack_init(&functions);

	token_t* last_tk = (token_t*)stack_pop(&stack_token);
	int count_id = 0;
	int count_const = 0;
	int count_functions = 0;

	if (last_tk && last_tk->type == TK_STM_END)
		last_tk = (token_t*)stack_pop(&stack_token);

	if (last_func == TK_FN_PUTS)
	{
		if (last_tk->type == TK_FN_PUTS)
			throw_exception(1011, source->line_cur, source);
		while (1)
		{
			if (last_tk->type == TK_ID)
			{
				stack_push(&ids, last_tk);
				count_id++;
				last_tk = (token_t*)stack_pop(&stack_token);

			}
			if (last_tk->type == TK_CONST)
			{
				stack_push(&constants, last_tk);
				last_tk = (token_t*)stack_pop(&stack_token);

				count_const++;
			}
			if (last_tk->type == TK_ADIC)
			{
				last_tk = (token_t*)stack_pop(&stack_token);

			}


			if (last_tk->type == TK_FN_PUTS)
			{
				int tam = count_id;
				for (int i = 0; i < tam; i++)
				{
					token_t* id = stack_pop(&ids);
					count_id--;

					if (!list_any_tbl_symb(tbl_symbols, position, id->id, NULL))
						throw_exception(1015, source->line_cur, source);
				}
				break;
			}
		}
	}

	while (last_tk && last_tk->type == TK_CONST)
	{
		stack_push(&constants, last_tk);
		last_tk = (token_t*)stack_pop(&stack_token);

		count_const++;

		if (last_tk && last_tk->type == TK_EQUAL)
		{
			last_tk = (token_t*)stack_pop(&stack_token);

			if (last_tk && last_tk->type != TK_ID)
				throw_exception(1002, source->line_cur, source);

			stack_push(&ids, last_tk);
			count_id++;
			last_tk = (token_t*)stack_pop(&stack_token);

		}
		else
			throw_exception(1002, source->line_cur, source);
	}

	while (last_tk && last_tk->type == TK_ID)
	{
		stack_push(&ids, last_tk);
		count_id++;
		last_tk = (token_t*)stack_pop(&stack_token);


		if (last_tk->type == TK_FN_GETS)
		{
			stack_push(&functions, last_tk);
			count_functions++;
		}
		if (last_tk->type == TK_FN_PUTS)
		{
			stack_push(&functions, last_tk);
			count_functions++;
		}
		else
		{
			while (last_tk->type == TK_CONST)
			{
				stack_push(&constants, last_tk);
				last_tk = (token_t*)stack_pop(&stack_token);

				count_const++;

				if (last_tk && last_tk->type == TK_EQUAL)
				{
					last_tk = (token_t*)stack_pop(&stack_token);

					if (last_tk && last_tk->type != TK_ID)
						throw_exception(1002, source->line_cur, source);
					stack_push(&ids, last_tk);
					count_id++;
					last_tk = (token_t*)stack_pop(&stack_token);

				}
				else
					throw_exception(1002, source->line_cur, source);
			}
		}
	}

	if (last_tk && last_tk->type == TK_FN_GETS)
	{
		while (count_id > 0)
		{
			token_t* id = stack_pop(&ids);
			token_t* valor = stack_pop(&constants);
			count_id--;
			count_const--;

			table_symbols_t* tbs = (table_symbols_t*)malloc(sizeof(table_symbols_t));
			tbs->type = last_tk->id;
			tbs->line = last_tk->line;

			tbs->length = "NULL";
			tbs->value = id->id;
			tbs->variable = "NULL";
			tbs->enable = 0;

			char buffer[255];
			FillMemory(&buffer, 255, 0);
			int tam = length_content_token(tbs->value);
			int any_comma = 0;

			/* Verificar se item existe na tabela de símbolos */
			if (tbl_symbols->size == 0)
				throw_exception(1011, source->line_cur, source);
			else
			{
				int j = 1;
				for (int i = 0; i < tam; i++)
				{
					char value = tbs->value[i];
					char scopy[1] = { value };

					if (is_caracter_comma(value) || j == tam)
					{
						any_comma = 1;

						if (j == tam)
							strncat(buffer, scopy, 1);

						if (!list_any_tbl_symb(tbl_symbols, position, buffer, NULL))
							throw_exception(1011, source->line_cur, source);

						FillMemory(&buffer, 255, 0);
					}
					else
						strncat(buffer, scopy, 1);
					j++;
				}
				if (!any_comma)
				{
					if (!list_any_tbl_symb(tbl_symbols, position, tbs->value, NULL))
						throw_exception(1011, source->line_cur, source);
				}
				list_insert_next(tbl_symbols, NULL, tbs);
				position = list_head(tbl_symbols);
			}
		}
	}

	if (last_tk && last_tk->type == TK_TYPE)
	{
		while (count_id > 0)
		{
			token_t* id = stack_pop(&ids);
			token_t* valor = stack_pop(&constants);
			count_id--;
			count_const--;
			char* length = 0;
			char* vartemp = 0;

			char* _dec = "dec";
			char* _char = "char";
			table_symbols_t* tbs = (table_symbols_t*)malloc(sizeof(table_symbols_t));

			if (id && id->type == TK_ID && ts_are_equal(last_tk->id, _char))
			{
				length = any_definition_length(id->id, source, 0);
				if (!length)
					throw_exception(1009, source->line_cur, source);

				vartemp = content_variable_id(id->id);
			}
			if (id && id->type == TK_ID && ts_are_equal(last_tk->id, _dec))
			{
				length = any_definition_length(id->id, source, 1);
				if (!length)
					throw_exception(1010, source->line_cur, source);

				vartemp = content_variable_id(id->id);
			}

			tbs->type = last_tk->id;
			tbs->line = last_tk->line;

			if (!vartemp)
				tbs->variable = id->id;
			else
				tbs->variable = vartemp;

			if (!length)
				tbs->length = "NULL";
			else
				tbs->length = length;

			if (!valor)
				tbs->value = "NULL";
			else
				tbs->value = valor->id;
			tbs->enable = 1;

			/* Verificar se item existe na tabela de símbolos */
			if (tbl_symbols->size == 0)
			{
				if (list_any_tbl_symb(tbl_symbols, position, tbs->variable, tbs->type))
					throw_exception(1004, source->line_cur, source);
				else
				{
					list_insert_next(tbl_symbols, NULL, tbs);
					position = list_head(tbl_symbols);
					show_table_symbols(tbl_symbols, position);
				}
			}
			else
			{
				if (list_any_tbl_symb(tbl_symbols, position, tbs->variable, tbs->type))
					throw_exception(1004, source->line_cur, source);
				else
				{
					list_insert_next(tbl_symbols, position, tbs);
					position = list_head(tbl_symbols);
					show_table_symbols(tbl_symbols, position);
				}
			}
		}

	}

	if (count_id > 0 || count_const > 0 || count_functions > 0)
	{
		while (count_id != 0)
		{
			token_t* id = stack_pop(&ids);
			token_t* valor = stack_pop(&constants);
			token_t* func = stack_pop(&functions);
			count_id--;
			count_const--;
			count_functions--;

			int i = 0;

			if (tbl_symbols->size == 0)
				throw_exception(1003, source->line_cur, source);
			if (!list_any_tbl_symb(tbl_symbols, position, id->id, NULL))
				throw_exception(1003, source->line_cur, source);
			else
				list_update_tbl_symb(tbl_symbols, position, id->id, valor->id);
		}
	}

	last_func = TK_TYPE;
}

/* Valida funcao gets */
void function_gets()
{
	//TODO 
}