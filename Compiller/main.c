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
#include "alerts.h"

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
			stack_t* functions; /* funções */
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

					/* Verificar se item existe na tabela de símbolos */
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
					char* length_obj = 0;
					char* vartemp = 0;

					char* _dec = "dec";
					char* _char = "char";
					char* _int = "int";
					table_symbols_t* tbs = (table_symbols_t*)malloc(sizeof(table_symbols_t));

					if (id && id->type == TK_ID && equals_to(last_tk->id, _int))
					{
						if (valor != NULL)
						{
							table_symbols_t* objint = list_get_tbl_symb(&table_symbols, list_position, valor->id, last_tk->id);
							table_symbols_t* objdec = list_get_tbl_symb(&table_symbols, list_position, valor->id, _dec);
							if (objdec != NULL)
							{
								if (objdec->value != "NULL")
								{
									if (!(is_numeric_int(objdec->value, source)))
										throw_exception(1007, source);
								}
								else
									throw_alert(1001, source);
							}
							else if (objint != NULL)
							{
								if (objint->value != "NULL")
								{
									if (!(is_numeric_decimal(objint->value, source)))
										throw_exception(1007, source);
								}
							}
							else if (objdec == NULL && objint == NULL)
							{
								if (!(is_numeric_int(valor->id, source)))
									throw_exception(1007, source);
							}
						}
					}

					if (id && id->type == TK_ID && equals_to(last_tk->id, _dec))
					{
						if (valor != NULL)
						{
							table_symbols_t* objdec = list_get_tbl_symb(&table_symbols, list_position, valor->id, last_tk->id);
							table_symbols_t* objint = list_get_tbl_symb(&table_symbols, list_position, valor->id, _int);
							if (objdec != NULL)
							{
								length_obj = objdec->length;

								if (objdec->value != "NULL")
								{
									if (!(is_numeric_decimal(objdec->value, source)))
										throw_exception(1016, source);
								}
							}
							else if (objint != NULL)
							{
								if (objint->value != "NULL")
								{
									if (!(is_numeric_decimal(objint->value, source)))
										throw_exception(1016, source);
								}
								else
									throw_alert(1002, source);
							}
							else if (objdec == NULL && objint == NULL)
							{
								if (!(is_numeric_decimal(valor->id, source)))
									throw_exception(1016, source);
							}
						}
					}

					if (id && id->type == TK_ID && equals_to(last_tk->id, _char))
					{
						length = any_definition_length(id->id, source, 0);
						if (!length)
							throw_exception(1009, source);
						vartemp = content_variable_id(id->id);

						if (valor != NULL && valor->id != NULL)
						{
							int tam = length_content_token_char(valor->id);
							int toint = atoi(length);
							int tosub = atoi(length);
							int tamPedaco = toint;
							tosub++;

							if (!(length_content_token(valor->id) <= tosub))
							{
								char* pedaco = content_substring(valor->id, 1, tosub);

								char* esquerda = (char*)malloc(255);
								FillMemory(esquerda, 255, 0);
								char* direita = (char*)malloc(255);
								FillMemory(direita, 255, 0);
								char* buffer = (char*)malloc(255);
								FillMemory(buffer, 255, 0);

								char scopy[1] = { '\"' };
								strncat(buffer, scopy, 1);
								strncat(buffer, pedaco, tamPedaco);
								strncat(buffer, scopy, 1);

								valor->id = buffer;
							}


							if (tam > toint)
								throw_alert(1004, source);
						}
					}
					if (id && id->type == TK_ID && equals_to(last_tk->id, _dec))
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
					{
						tbs->length = length;
						if (length_obj != NULL && length != length_obj)
							throw_alert(1003, source);
					}

					if (!valor)
						tbs->value = "NULL";
					else
					{
						char* search = ".";
						if (equals_to(last_tk->id, _dec) && content_indexOf(valor->id, search, 0) >= 0)
						{
							int indexP = content_indexOf(valor->id, search, 0);  //Da posicao zero para o ponto do valor atribuido
							int indexCasaDec = content_indexOf(length, search, 0);
							int antesPonto = indexCasaDec - 1;
							char* direitaCasa = content_substring(length, 0, antesPonto);  //Da posicao zero para o ponto do valor Declarado
							int direitaInt = atoi(direitaCasa) - 1;

							if (indexP - 1 < direitaInt)
								direitaInt = indexP - 1;

							char* prefixo = content_substring(valor->id, 0, direitaInt); // Do ponto pra o final do valor atribuido
							int tamPref = length_content_token(prefixo);

							indexCasaDec++;

							int tam = length_content_token(length);
							char* esqCasa = content_substring(length, indexCasaDec, ++tam); // Da posicao do ponto até o resto do valor declarado
							char* sufixo = content_substring(valor->id, indexP, indexP + atoi(esqCasa));
							strncat(prefixo, sufixo, 255);
							tbs->value = prefixo;

							if ((atoi(direitaCasa) <  indexP) || (atoi(esqCasa) < tamPref))
								throw_alert(1003, source);
						}
						else
							tbs->value = valor->id;
					}


					tbs->enable = 1;

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

					table_symbols_t* obj = list_get_tbl_symb(&table_symbols, list_position, id->id, NULL);
					if (obj != NULL)
					{
						char* _dec = "dec";
						char* _char = "char";
						char* _int = "int";

						char* variable = (char*)obj->variable;
						char* tipo = (char*)obj->type;

						// Tipo int
						if (equals_to(tipo, _int))
						{
							if (valor != NULL)
							{
								table_symbols_t* objint = list_get_tbl_symb(&table_symbols, list_position, valor->id, _int);
								table_symbols_t* objdec = list_get_tbl_symb(&table_symbols, list_position, valor->id, _dec);
								if (objdec != NULL)
									throw_alert(1001, source);
								else if (objdec == NULL && objint == NULL)
								{
									if (!(is_numeric_int(valor->id, source)))
										throw_exception(1016, source);
								}
							}
						}
						// Tipo dec
						if (equals_to(tipo, _dec))
						{
							if (valor != NULL)
							{
								table_symbols_t* objdec = list_get_tbl_symb(&table_symbols, list_position, valor->id, _dec);
								table_symbols_t* objint = list_get_tbl_symb(&table_symbols, list_position, valor->id, _int);
								if (objdec != NULL)
								{
									if (objdec->length != obj->length)
										throw_alert(1003, source);
								}

								if (objint != NULL)
									throw_alert(1002, source);
								else if (objdec == NULL && objint == NULL)
								{
									if (!(is_numeric_decimal(valor->id, source)))
										throw_exception(1015, source);
								}
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
		{
			show_table_symbols(table_symbols, list_position);
			printf("\n\n\n\n ========== Build: 1 succeeded, 0 failed, %i alerts ==========", source->count_alerts);
		}
	}
}