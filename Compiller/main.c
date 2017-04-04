#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "token_stream.h"
#include "stack.h"
#include "utils.h"
#include "list.h"
#include "token_exception.h"

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

	token_t* last_tk_temp = (token_t*)malloc(sizeof(token_t));

	while (1)
	{
		token_t * token = ts_get_next_token(source, last_tk_temp);  /* Pega proximo token */

		/* Insere token na pilha */
		if (token != NULL)
		{
			stack_push(&stack_token, token);
			length_stack++;
			last_tk_temp = token;
		}

		if (is_caracter_semicolon(source->last_read))
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
						te_generate_exception(1002, source->line_cur, source);

					stack_push(&ids, last_tk);
					count_id++;
					last_tk = (token_t*)stack_pop(&stack_token);
				}
				else
					te_generate_exception(1002, source->line_cur, source);
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
							te_generate_exception(1002, source->line_cur, source);
						stack_push(&ids, last_tk);
						count_id++;
						last_tk = (token_t*)stack_pop(&stack_token);
					}
					else
						te_generate_exception(1002, source->line_cur, source);
				}
			}
#pragma endregion

			if (last_tk && last_tk->type == TK_TYPE)
			{
				while (count_id != 0)
				{
					token_t* id = stack_pop(&ids);
					token_t* valor = stack_pop(&constants);

					table_symbols_t* tbs = (table_symbols_t*)malloc(sizeof(table_symbols_t));
					tbs->type = last_tk->id;
					tbs->line = last_tk->line;

					if (!valor)
						tbs->value = "NULL";
					else
						tbs->value = valor->id;

					tbs->variable = id->id;
					count_id--;
					count_const--;


					/* Verificar se item existe na tabela de símbolos */
					if (table_symbols.size == 0)
					{
						list_insert_next(&table_symbols, NULL, tbs);
						list_position = list_head(&table_symbols);
					}
					else
					{
						list_insert_next(&table_symbols, list_position, tbs);
						list_position = list_head(&table_symbols);
					}
				}
			}

			if (count_id > 0 || count_const > 0) 
			{
				while (count_id != 0)
				{
					token_t* id = stack_pop(&ids);
					token_t* valor = stack_pop(&constants);
					int i = 0;

					if (table_symbols.size == 0)
						te_generate_exception(1003, source->line_cur, source);
					if (!list_any_tbl_symb(&table_symbols, list_position, id->id))
						te_generate_exception(1003, source->line_cur, source);
					else 
					{
						// alterar
						int i = 20;
					}
					/* procurar itens na tabela de simbolos */
					/* deve encontrar o item id que é a variavel  */

				}
			}
		}

#pragma region Exibir tabela de simbolos
		if (source->last_read == -1)
		{
			printf("Tabela de simbolos \n");
			printf("\n");
			printf("\t%-3s\t|\t %-20s\t| %-20s\t| %-10s\n", "TIPO", "VARIAVEL", "VALOR", "LINHA");
			printf("\t------------------------------------------------------------------------");
			printf("\n");
			for (int i = 0; i < list_get_size(&table_symbols); i++)
			{
				table_symbols_t* object = (table_symbols_t*)list_position->data;

				char* variable = (char*)object->variable;
				char* tipo = (char*)object->type;
				char* value = (char*)object->value;
				int line = object->line;

				printf("\t%-3s\t|\t %-20s\t| %-20s\t| %-10i\n", tipo, variable, value, line);
				list_position = list_next(list_position);
			}
		}
#pragma endregion
	}

error:
	"erro ao processar";
	ts_close_source(source);
	return 0;

	getchar();
}


/*

1º Verificar se ja existe na tabela de simbolos antes de inserir
2º Validar automatos na hora de inserir na tabela de simbolos
	2.1- Variavel -> Tipo; Ex: int &doc;
	2.2- Valor -> Igual -> Variavel -> Tipo; Ex: int &doc = 10;
	2.3- Variavel -> Igual -> Variavel -> Tipo; Ex: int &doc = &a;
	2.4- Variaveis do tipo char... (fudeu);
3º Definir tipo correto para valores atribuidos hoje está vindo como ID deve ser CONST;

*/
