#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "list.h"
#include "token_exception.h"
#include "tb_symbols.h"

int main(int argc, char** argv) {

	source_t* source = ts_open_source("Source.chs"); /* Abre arquivo em binário */
	stack_t* stack_token; /* Pilha de Tokens */
	int length_stack = 0;

	int ret = stack_init(&stack_token);
	if (ret < 0)
		fprintf(stderr, "Falha ao iniciar a stack \n");

	list_element_t* list_position = NULL;
	linked_list_t table_symbols;
	list_initialize(&table_symbols, NULL);

	token_t* last_tk_temp = (token_t*)malloc(sizeof(token_t));
	char* last_type = (char*)malloc(sizeof(char));

	while (1)
	{
		token_t * token = ts_get_next_token(source, last_tk_temp, last_type);  /* Pega proximo token */

		/* Insere token na pilha */
		if (token != NULL && is_token_valid(token, source))
		{
			if (token->type == TK_TYPE)
				last_type = token->id;

			stack_push(&stack_token, token);
			length_stack++;
			last_tk_temp = token;
		}

		if (is_caracter_semicolon(source->last_read))
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
			int count_semicolon = 0;
			int count_functions = 0;

			if (last_tk && last_tk->type == TK_STM_END)
			{
				count_semicolon++;
				last_tk = (token_t*)stack_pop(&stack_token);
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
#pragma region Declaracao de variavel simples Ex: int &var, &var1;

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
#pragma endregion

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

					char buffer[255];
					FillMemory(&buffer, 255, 0);
					int tam = length_content_token(tbs->value);
					int any_comma = 0;

					/* Verificar se item existe na tabela de símbolos */
					if (table_symbols.size == 0)
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

								if (!list_any_tbl_symb(&table_symbols, list_position, buffer, NULL))
									throw_exception(1011, source->line_cur, source);

								FillMemory(&buffer, 255, 0);
							}
							else
								strncat(buffer, scopy, 1);
							j++;
						}
						if (!any_comma)
						{
							if (!list_any_tbl_symb(&table_symbols, list_position, tbs->value, NULL))
								throw_exception(1011, source->line_cur, source);
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

					/* Verificar se item existe na tabela de símbolos */
					if (table_symbols.size == 0)
					{
						if (list_any_tbl_symb(&table_symbols, list_position, tbs->variable, tbs->type))
							throw_exception(1004, source->line_cur, source);
						else
						{
							list_insert_next(&table_symbols, NULL, tbs);
							list_position = list_head(&table_symbols);
						}
					}
					else
					{
						if (list_any_tbl_symb(&table_symbols, list_position, tbs->variable, tbs->type))
							throw_exception(1004, source->line_cur, source);
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

					int i = 0;

					if (table_symbols.size == 0)
						throw_exception(1003, source->line_cur, source);
					if (!list_any_tbl_symb(&table_symbols, list_position, id->id, NULL))
						throw_exception(1003, source->line_cur, source);
					else
						list_update_tbl_symb(&table_symbols, list_position, id->id, valor->id);
				}
			}
		}
		if (source->last_read == -1)
			show_table_symbols(table_symbols, list_position);
	}

error:
	"erro ao processar";
	ts_close_source(source);
	return 0;

	getchar();
}