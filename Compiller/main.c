#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "list.h"
#include "exception.h"
#include "tb_symbols.h"
#include "functions.h"

int main(int argc, char** argv) {

	source_t* source = ts_open_source("Source.chs");
	stack_t* stack_token;
	int length_stack = 0;

	int ret = stack_init(&stack_token);
	if (ret < 0)
		fprintf(stderr, "Falha ao iniciar a stack \n");

	list_element_t* list_position = NULL;
	linked_list_t table_symbols;
	list_initialize(&table_symbols, NULL);

	token_t* last_tk_temp = (token_t*)malloc(sizeof(token_t));
	char* last_type = (char*)malloc(sizeof(char));
	token_type_t last_func = (token_type_t)malloc(sizeof(token_type_t));

	while (1)
	{
		token_t * token = ts_get_next_token(source, last_tk_temp, last_func);  /* Pega proximo token */

		/* Insere token na pilha */
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
			fn_run_for(source, stack_token, table_symbols, list_position);
			last_func = TK_END_FN;
		}

		if (is_caracter_closed_parathesi(source->last_read) && last_func == TK_FN_IF)
		{
			fn_run_if(source, stack_token, table_symbols, list_position);
			last_func = TK_END_FN;
		}

		if (is_caracter_semicolon(source->last_read) && last_func != TK_FN_FOR)
		{
			stack_t* ids;		/* variaveis */
			stack_t* constants; /* atribuicoes de valores */
			stack_t* functions; /* fun��es */
			stack_init(&ids);
			stack_init(&constants);
			stack_init(&functions);

			token_t* last_tk = (token_t*)stack_pop(&stack_token);
			length_stack--;
			int count_id = 0;
			int count_const = 0;
			int count_functions = 0;

			if (last_tk && last_tk->type == TK_STM_END)
			{
				last_tk = (token_t*)stack_pop(&stack_token);
				length_stack--;
			}


			if (last_func == TK_FN_PUTS)
			{
				if (last_tk->type == TK_FN_PUTS)
					throw_exception(1011, source);
				while (1)
				{
					if (last_tk->type == TK_ID)
					{
						stack_push(&ids, last_tk);
						count_id++;
						last_tk = (token_t*)stack_pop(&stack_token);
						length_stack--;
					}
					if (last_tk->type == TK_CONST)
					{
						stack_push(&constants, last_tk);
						last_tk = (token_t*)stack_pop(&stack_token);
						length_stack--;
						count_const++;
					}
					if (last_tk->type == TK_ADIC)
					{
						last_tk = (token_t*)stack_pop(&stack_token);
						length_stack--;
					}


					if (last_tk->type == TK_FN_PUTS)
					{
						int tam = count_id;
						for (int i = 0; i < tam; i++)
						{
							token_t* id = stack_pop(&ids);
							count_id--;

							if (!list_any_tbl_symb(&table_symbols, list_position, id->id, NULL))
								throw_exception(1015, source);
						}
						break;
					}
				}
			}

			while (last_tk && last_tk->type == TK_CONST)
			{
				stack_push(&constants, last_tk);
				last_tk = (token_t*)stack_pop(&stack_token);
				length_stack--;
				count_const++;

				if (last_tk && last_tk->type == TK_EQUAL)
				{
					last_tk = (token_t*)stack_pop(&stack_token);
					length_stack--;
					if (last_tk && last_tk->type != TK_ID)
						throw_exception(1002, source);

					stack_push(&ids, last_tk);
					count_id++;
					last_tk = (token_t*)stack_pop(&stack_token);
					length_stack--;
				}
				else
					throw_exception(1002, source);
			}

			while (last_tk && last_tk->type == TK_ID)
			{
				stack_push(&ids, last_tk);
				count_id++;
				last_tk = (token_t*)stack_pop(&stack_token);
				length_stack--;

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
						length_stack--;
						count_const++;

						if (last_tk && last_tk->type == TK_EQUAL)
						{
							last_tk = (token_t*)stack_pop(&stack_token);
							length_stack--;
							if (last_tk && last_tk->type != TK_ID)
								throw_exception(1002, source);
							stack_push(&ids, last_tk);
							count_id++;
							last_tk = (token_t*)stack_pop(&stack_token);
							length_stack--;
						}
						else
							throw_exception(1002, source);
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

					/* Verificar se item existe na tabela de s�mbolos */
					if (table_symbols.size == 0)
						throw_exception(1011, source);
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

								if (!list_any_tbl_symb(&table_symbols, list_position, buffer, NULL))
									throw_exception(1011, source);

								FillMemory(&buffer, 255, 0);
							}
							else
								strncat(buffer, scopy, 1);
							j++;
						}
						if (!any_comma)
						{
							if (!list_any_tbl_symb(&table_symbols, list_position, tbs->value, NULL))
								throw_exception(1011, source);
						}
						list_insert_next(&table_symbols, NULL, tbs);
						list_position = list_head(&table_symbols);
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
					char* _int = "int";
					table_symbols_t* tbs = (table_symbols_t*)malloc(sizeof(table_symbols_t));

					if (id && id->type == TK_ID && ts_are_equal(last_tk->id, _int))
					{
						if (valor != NULL) 
						{
							if (!(is_numeric_int(valor->id)))
								throw_exception(1007, source);
						}
					}

					if (id && id->type == TK_ID && ts_are_equal(last_tk->id, _char))
					{
						length = any_definition_length(id->id, source, 0);
						if (!length)
							throw_exception(1009, source);
						vartemp = content_variable_id(id->id);

						if (valor != NULL && valor->id != NULL)
						{
							int tam = length_content_token_char(valor->id);
							int toint = atoi(length);
							if (tam > toint)
								throw_exception(1009, source);
						}
					}
					if (id && id->type == TK_ID && ts_are_equal(last_tk->id, _dec))
					{
						length = any_definition_length(id->id, source, 1);
						if (!length)
							throw_exception(1010, source);

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

					/* Verificar se item existe na tabela de s�mbolos */
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

					table_symbols_t* obj =  list_get_tbl_symb(&table_symbols, list_position, id->id, NULL);
					if (obj != NULL)
					{
						char* _dec = "dec";
						char* _char = "char";
						char* _int = "int";

						char* variable = (char*)obj->variable;
						char* tipo = (char*)obj->type;

						// Tipo int
						if (ts_are_equal(tipo, _int))
						{
							if (valor != NULL)
							{
								if (!(is_numeric(*valor->id)))
									throw_exception(1007, source);
							}
						}
					}
					else 
						throw_exception(1003, source);


					if (table_symbols.size == 0)
						throw_exception(1003, source);
					if (!list_any_tbl_symb(&table_symbols, list_position, id->id, NULL))
						throw_exception(1003, source);
					else
						list_update_tbl_symb(&table_symbols, list_position, id->id, valor->id);
				}
			}

			last_func = TK_TYPE;
		}

		if (source->last_read == -1)
			show_table_symbols(table_symbols, list_position);
	}
}