#include <stdlib.h>
#include <stdio.h>
#include "token_stream.h"
#include "list.h"
#include "stack.h"

void show_table_symbols(linked_list_t tb_list, list_element_t* list_position)
{
	printf("Tabela de simbolos \n");
	printf("\n");
	printf("\t%-3s\t| %-15s\t| %-2s\t| %-20s\t|\t %-10s\n", "TIPO", "VARIAVEL", "TAM", "VALOR", "LINHA");
	printf("\t------------------------------------------------------------------------------------");
	printf("\n");
	for (int i = 0; i < list_get_size(&tb_list); i++)
	{
		table_symbols_t* object = (table_symbols_t*)list_position->data;

		char* variable = (char*)object->variable;
		char* tipo = (char*)object->type;
		char* value = (char*)object->value;
		char* length = (char*)object->length;
		int line = object->line;

		if (object->enable)
			printf("\t%-3s\t| %-15s\t| %-2s\t| %-20s\t|\t %-10i\n", tipo, variable, length, value, line);
		list_position = list_next(list_position);
	}
}

void insert_table_symbols(stack_t* stack_token, source_t* source, linked_list_t table_symbols, list_element_t* list_position)
{
	stack_t* ids; /* variaveis */
	stack_t* constants; /* atribuicoes de valores */
	stack_init(&ids);
	stack_init(&constants);

	token_t* last_tk = (token_t*)stack_pop(&stack_token);
	int count_id = 0;
	int count_const = 0;

	while (last_tk && last_tk->type == TK_CONST)
	{
		stack_push(&constants, last_tk);
		last_tk = (token_t*)stack_pop(&stack_token);
		count_const++;

		if (last_tk && last_tk->type == TK_EQUAL)
		{
			last_tk = (token_t*)stack_pop(&stack_token);
			if (last_tk && last_tk->type != TK_ID)
				throw_exception(1002, source);

			stack_push(&ids, last_tk);
			count_id++;
			last_tk = (token_t*)stack_pop(&stack_token);
		}
		else
			throw_exception(1002, source);
	}
#pragma region Declaracao de variavel simples Ex: int &var, &var1;

	while (last_tk && last_tk->type == TK_ID)
	{
		stack_push(&ids, last_tk);
		last_tk = (token_t*)stack_pop(&stack_token);
		count_id++;

		while (last_tk->type == TK_CONST)
		{
			stack_push(&constants, last_tk);
			last_tk = (token_t*)stack_pop(&stack_token);
			count_const++;

			if (last_tk && last_tk->type == TK_EQUAL)
			{
				last_tk = (token_t*)stack_pop(&stack_token);
				if (last_tk && last_tk->type != TK_ID)
					throw_exception(1002, source);
				stack_push(&ids, last_tk);
				count_id++;
				last_tk = (token_t*)stack_pop(&stack_token);
			}
			else
				throw_exception(1002, source);
		}
	}
#pragma endregion

	if (last_tk && last_tk->type == TK_TYPE)
	{
		while (count_id > 0)
		{
			token_t* id = stack_pop(&ids);
			token_t* valor = stack_pop(&constants);
			count_id--;
			count_const--;
			char* length = 0; // tamanho de variavel char e dec 

			char* _dec = "dec";
			char* _char = "char";

			if (id && id->type == TK_ID && ts_are_equal(last_tk->id, _char))
				length = any_definition_length(id->id, source, 0);
			if (id && id->type == TK_ID && ts_are_equal(last_tk->id, _dec))
				length = any_definition_length(id->id, source, 1);


			table_symbols_t* tbs = (table_symbols_t*)malloc(sizeof(table_symbols_t));
			tbs->type = last_tk->id;
			tbs->line = last_tk->line;

			if (!length)
				tbs->length = "NULL";
			else
				tbs->length = length;

			if (!valor)
				tbs->value = "NULL";
			else
				tbs->value = valor->id;
			tbs->variable = id->id;

			/* Verificar se item existe na tabela de símbolos */
			if (table_symbols.size == 0)
			{
				if (list_any_tbl_symb(&table_symbols, list_position, tbs->variable, tbs->type))
					throw_exception(1004, source);
				else
				{
					list_insert_next(&table_symbols, NULL, tbs);
					list_position = list_head(&table_symbols);
				}
			}
			else
			{
				if (list_any_tbl_symb(&table_symbols, list_position, tbs->variable, tbs->type))
					throw_exception(1004, source);
				else
				{
					list_insert_next(&table_symbols, list_position, tbs);
					list_position = list_head(&table_symbols);
				}
			}
		}
	}

	if (count_id > 0 || count_const > 0)
	{
		while (count_id != 0)
		{
			token_t* id = stack_pop(&ids);
			token_t* valor = stack_pop(&constants);
			count_id--;
			count_const--;
			int i = 0;

			if (table_symbols.size == 0)
				throw_exception(1003, source);
			if (!list_any_tbl_symb(&table_symbols, list_position, id->id, NULL))
				throw_exception(1003, source);
			else
				list_update_tbl_symb(&table_symbols, list_position, id->id, valor->id);
		}
	}
}

/* Valida funcao gets */
void function_gets() 
{
	//TODO 
}