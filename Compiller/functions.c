#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "token_stream.h"
#include "utils.h"
#include "token_exception.h"
#include "stack.h"
#include "list.h"

/* Verifica se arquivo começa com main() */
int fn_main(char value, source_t* source)
{
	int tam = 1;
	char * main = "main(){";
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);

	char scopy[1] = { value };
	strncat(buffer, scopy, 1);

	while (1)
	{
		value = ts_get_next_caracter(source);
		if (is_alphanumeric(value))
		{
			while (1)
			{
				if (is_new_line(value))
				{
					if (is_caracter_smash_line(value))
					{
						source->line_cur++;
						source->init_pos_line = source->last_pos;
					}
				}

				if (!is_new_line(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					tam++;
					if (tam > 7)
						throw_exception(1001, 1, source);
				}

				value = ts_get_next_caracter(source); // Lê próximo caracter
				if (tam == 7 && ts_are_equal(main, buffer))
					return 1;
			}
		}
		else
			throw_exception(1001, 1, source);
	}
	return 0;
}

/* Função gets */
token_t* fn_gets(source_t* source, token_t* last_token)
{
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);
	int line = source->line_cur;
	token_t* token = (token_t*)malloc(sizeof(token_t));

	if (!is_caracter_open_parathesi(source->last_read))
		throw_exception(1011, source->line_cur, source);

	int is_virgula = 0;
	int expected_ampersand = 0;
	while (1)
	{
		char value = ts_get_next_caracter(source);
		if (is_caracter_ampersand(value) || is_space(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				if (!is_space(value))
					strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);

				if (expected_ampersand)
				{
					if (!is_space(value) && !is_caracter_ampersand(value))
						throw_exception(1002, source->line_cur, source);

					if (is_caracter_ampersand(value))
						expected_ampersand = 0;
				}

				if (is_caracter_closed_parathesi(value))
				{
					value = ts_get_next_caracter(source);
					if (!is_caracter_semicolon(value))
						throw_exception(1012, source->line_cur, source);

					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source);
					return token;
				}

				if (is_caracter_comma(value))
					expected_ampersand = 1;
			}
		}
		else
			throw_exception(1002, source->line_cur, source);
	}
}

/* Função puts */
token_t* fn_puts(source_t* source, token_t* last_token)
{
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);
	int line = source->line_cur;
	token_t* token = (token_t*)malloc(sizeof(token_t));


	if (last_token->type == TK_FN_PUTS)
		if (!is_caracter_open_parathesi(source->last_read))
			throw_exception(1011, source->line_cur, source);

	while (1)
	{
		if (is_caracter_plus(source->last_read))
		{
			char value = ts_get_next_caracter(source);

			token->id = "+";
			token->line = line;
			token->type = TK_ADIC;
			return token;
		}

		if (is_caracter_closed_parathesi(source->last_read))
		{
			char value = ts_get_next_caracter(source);
			if (!is_caracter_semicolon(value))
				throw_exception(1012, source->line_cur, source);
			else
			{
				token->id = ";";
				token->line = line;
				token->type = TK_STM_END;
				return token;
			}
		}

		char value = ts_get_next_caracter(source);
		if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // Lê proximo caracter
				int tam = length_content_token(buffer);

				if (is_caracter_ampersand(value))
					throw_exception(1002, source->line_cur, source);

				if (tam == 1 && (is_numeric(value) || is_alphanumeric_toupper(value)))
					throw_exception(1002, source->line_cur, source);

				if (is_space(value) || is_caracter_plus(value) || is_caracter_comma(value) || is_caracter_quotes_plus(value) || is_caracter_closed_parathesi(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		if (is_caracter_quotes_plus(value))
		{
			int count_quotes = 1;
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);
				if (is_caracter_semicolon(value) && count_quotes % 2 != 0)
					throw_exception(1013, source->line_cur, source);
				if (is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_plus(value))
				{
					if (count_quotes % 2 == 0)
					{
						token->id = buffer;
						token->line = line;
						token->type = TK_CONST;
						return token;
					}
					else
						throw_exception(1013, source->line_cur, source);
				}
				if (is_space(value) && count_quotes == 0)
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					value = ts_get_next_caracter(source);
				}
				if (is_caracter_plus(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					value = ts_get_next_caracter(source);
					if (is_caracter_semicolon(value))
						throw_exception(1013, source->line_cur, source);
				}
				if (is_caracter_quotes_plus(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					if (count_quotes % 2 == 0)
					{
						count_quotes++;
						while (1)
						{
							value = ts_get_next_caracter(source);
							char scopy[1] = { value };
							strncat(buffer, scopy, 1);
							if (is_caracter_semicolon(value) && count_quotes % 2 != 0)
								throw_exception(1013, source->line_cur, source);
							if (is_caracter_quotes_plus(value))
							{
								char scopy[1] = { value };
								strncat(buffer, scopy, 1);
								break;
							}
						}
					}
					count_quotes++;
				}
			}
		}
	}
	return NULL;
}

/* Função for */
token_t* fn_for(source_t* source, token_t* last_token)
{
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);
	int line = source->line_cur;
	token_t* token = (token_t*)malloc(sizeof(token_t));

	if (last_token->type == TK_FN_FOR)
		if (!is_caracter_open_parathesi(source->last_read))
			throw_exception(1011, source->line_cur, source);

	while (1)
	{
		char value = ts_get_next_caracter(source);

		if (is_alphanumeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);
				if (is_token_function(buffer, source) || is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value))
				{
					token->id = buffer;
					token->line = line;
					if (is_token_function(buffer, source))
						token->type = ts_get_type_fn(buffer);
					else
						token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);
				int tam = length_content_token(buffer);

				if (is_caracter_arimetic(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					while (1)
					{
						char tmp = value;
						value = ts_get_next_caracter(source);
						if (is_caracter_arimetic(value))
						{
							if (is_caracter_plus(value) && is_caracter_plus(tmp) || is_caracter_less(value) && is_caracter_less(tmp))
							{
								char scopy[1] = { value };
								strncat(buffer, scopy, 1);

								value = ts_get_next_caracter(source);

								if (is_caracter_closed_parathesi(value))
								{
									token->id = buffer;
									token->line = line;
									token->type = TK_INCREMENT;
									return token;
								}
							}
							else
								throw_exception(1011, source->line_cur, source);
						}
						else if (is_caracter_closed_parathesi(value))
							throw_exception(1011, source->line_cur, source);
					}
				}

				if (is_caracter_ampersand(value))
					throw_exception(1002, source->line_cur, source);

				if (tam == 1 && (is_numeric(value) || is_alphanumeric_toupper(value)))
					throw_exception(1002, source->line_cur, source);

				if (tam > 1)
					if (!(is_numeric(value) || is_alphanumeric(value) || is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value) || is_caracter_open_parathesi(value) || is_caracter_point(value) || is_caracter_closed_parathesi(value)))
						throw_exception(1002, source->line_cur, source);

				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value) || is_caracter_closed_parathesi(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_equals(value))
		{
			value = ts_get_next_caracter(source);
			if (is_space(value))
			{
				token->id = "=";
				token->line = line;
				token->type = TK_EQUAL;
				return token;
			}
			else if (!is_caracter_equals(value))
				throw_exception(1012, source->line_cur, source);
			else
			{
				token->id = "==";
				token->line = line;
				token->type = TK_RELATIONAL;
				return token;
			}
		}
		else if (is_numeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // Lê proximo caracter
				if (is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value) || is_caracter_relational(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = TK_CONST;
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_relational(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);
				if (is_space(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = TK_RELATIONAL;
					return token;
				}
				if (!is_caracter_relational(value))
					throw_exception(1012, source->line_cur, source);
				else
				{
					if (*buffer == value)
						if (!is_caracter_equals(value))
							throw_exception(1012, source->line_cur, source);

					if (is_caracter_equals(*buffer) && !is_caracter_equals(value))
						strncat(buffer, scopy, 1);
					else
						throw_exception(1012, source->line_cur, source);
				}

				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_arimetic(value))
		{
			char scopy[1] = { value };
			strncat(buffer, scopy, 1);
			while (1)
			{
				char tmp = value;
				value = ts_get_next_caracter(source);
				if (is_caracter_arimetic(value))
				{
					if (is_caracter_plus(value) && is_caracter_plus(tmp) || is_caracter_less(value) && is_caracter_less(tmp))
					{
						while (1)
						{
							char scopy[1] = { value };
							strncat(buffer, scopy, 1);

							value = ts_get_next_caracter(source);

							if (is_caracter_closed_parathesi(value))
							{
								token->id = buffer;
								token->line = line;
								token->type = TK_INCREMENT;
								return token;
							}
						}

					}
					else
						throw_exception(1011, source->line_cur, source);
				}
				else
					throw_exception(1011, source->line_cur, source);
			}
		}
		else if (is_caracter_semicolon(value))
		{
			token->id = ";";
			token->line = line;
			token->type = TK_STM_END;
			return token;
		}
		else if (is_caracter_closed_parathesi(value))
			return NULL;
	}
}

/* Processa função for */
void fn_run_for(source_t* source, struct stack_t* stack_token, linked_list_t table_symbols, list_element_t* list_position)
{
	token_t* last_tk;
	int read = 0;
	stack_t* ids;
	int count_id = 0;
	stack_init(&ids);

	while (1)
	{
		last_tk = (token_t*)stack_pop(&stack_token);
		if (last_tk->type != TK_FN_FOR)
		{
			stack_push(&ids, last_tk);
			count_id++;
		}
		else
			break;
	}

	if (last_tk->type == TK_FN_FOR)
	{
		while (1)
		{
			token_t* id = stack_pop(&ids);
			count_id--;
			char * buffer = (char*)malloc(255);
			FillMemory(buffer, 255, 0);

			if (count_id > 0 && id->type == TK_TYPE)
			{
				if (strcmpi("int", id->id) != 0)
					throw_exception(1011, source->line_cur, source);

				while (1)
				{
					token_t* id = stack_pop(&ids);
					count_id--;
					if (id->type != TK_ID)
						throw_exception(1011, source->line_cur, source);
					read = 1;
					break;
				}
			}
			else if (count_id > 0 && id->type == TK_ID)
			{
				char* type = "int";
				if (!list_any_tbl_symb(&table_symbols, list_position, id->id, type))
					throw_exception(1015, source->line_cur, source);
			}
			if (count_id == 0 || read == 1)
				break;
		}
	}
}

/* Função if */
token_t* fn_if(source_t* source, token_t* last_token)
{
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);
	int line = source->line_cur;
	token_t* token = (token_t*)malloc(sizeof(token_t));

	if (last_token->type == TK_FN_IF)
		if (!is_caracter_open_parathesi(source->last_read))
			throw_exception(1011, source->line_cur, source);

	while (1)
	{
		char value = ts_get_next_caracter(source);

		if (is_alphanumeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);
				if (is_token_function(buffer, source) || is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value))
				{
					token->id = buffer;
					token->line = line;
					if (is_token_function(buffer, source))
						token->type = ts_get_type_fn(buffer);
					else
						token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);
				int tam = length_content_token(buffer);

				if (is_caracter_arimetic(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					while (1)
					{
						char tmp = value;
						value = ts_get_next_caracter(source);
						if (is_caracter_arimetic(value))
						{
							if (is_caracter_plus(value) && is_caracter_plus(tmp) || is_caracter_less(value) && is_caracter_less(tmp))
							{
								char scopy[1] = { value };
								strncat(buffer, scopy, 1);

								value = ts_get_next_caracter(source);

								if (is_caracter_closed_parathesi(value))
								{
									token->id = buffer;
									token->line = line;
									token->type = TK_INCREMENT;
									return token;
								}
							}
							else
								throw_exception(1011, source->line_cur, source);
						}
						else if (is_caracter_closed_parathesi(value))
							throw_exception(1011, source->line_cur, source);
					}
				}

				if (is_caracter_logic(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					while (1)
					{
						if (!is_caracter_ampersand(value))
							throw_exception(1011, source->line_cur, source);

						value = ts_get_next_caracter(source);
						if(!is_space(value))
							throw_exception(1011, source->line_cur, source);
						else
						{
							token->id = "&&";
							token->line = line;
							token->type = TK_OP_AND;
							return token;
						}
					}
				}

				if (is_caracter_ampersand(value))
					throw_exception(1002, source->line_cur, source);

				if (tam == 1 && (is_numeric(value) || is_alphanumeric_toupper(value)))
					throw_exception(1002, source->line_cur, source);

				if (tam > 1)
					if (!(is_numeric(value) || is_alphanumeric(value) || is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value) || is_caracter_open_parathesi(value) || is_caracter_point(value) || is_caracter_closed_parathesi(value)))
						throw_exception(1002, source->line_cur, source);

				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value) || is_caracter_closed_parathesi(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_equals(value))
		{
			value = ts_get_next_caracter(source);
			if (is_space(value))
			{
				token->id = "=";
				token->line = line;
				token->type = TK_EQUAL;
				return token;
			}
			else if (!is_caracter_equals(value))
				throw_exception(1012, source->line_cur, source);
			else
			{
				token->id = "==";
				token->line = line;
				token->type = TK_RELATIONAL;
				return token;
			}
		}
		else if (is_numeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // Lê proximo caracter
				if (is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value) || is_caracter_relational(value) || is_caracter_closed_parathesi(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = TK_CONST;
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_relational(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source);
				if (is_space(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = TK_RELATIONAL;
					return token;
				}
				if (!is_caracter_relational(value))
					throw_exception(1012, source->line_cur, source);
				else
				{
					if (*buffer == value)
						if (!is_caracter_equals(value))
							throw_exception(1012, source->line_cur, source);

					if (is_caracter_equals(*buffer) && !is_caracter_equals(value))
						strncat(buffer, scopy, 1);
					else
						throw_exception(1012, source->line_cur, source);
				}

				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_caracter_arimetic(value))
		{
			char scopy[1] = { value };
			strncat(buffer, scopy, 1);
			while (1)
			{
				char tmp = value;
				value = ts_get_next_caracter(source);
				if (is_caracter_arimetic(value))
				{
					if (is_caracter_plus(value) && is_caracter_plus(tmp) || is_caracter_less(value) && is_caracter_less(tmp))
					{
						while (1)
						{
							char scopy[1] = { value };
							strncat(buffer, scopy, 1);

							value = ts_get_next_caracter(source);

							if (is_caracter_closed_parathesi(value))
							{
								token->id = buffer;
								token->line = line;
								token->type = TK_INCREMENT;
								return token;
							}
						}

					}
					else
						throw_exception(1011, source->line_cur, source);
				}
				else
					throw_exception(1011, source->line_cur, source);
			}
		}
		else if (is_caracter_semicolon(value))
		{
			token->id = ";";
			token->line = line;
			token->type = TK_STM_END;
			return token;
		}
		else if (is_caracter_logic(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);
				while (1)
				{
					value = ts_get_next_caracter(source);
					if (!is_caracter_bar(value))
						throw_exception(1011, source->line_cur, source);
					
					strncat(buffer, scopy, 1);
					value = ts_get_next_caracter(source);

					if (!is_space(value))
						throw_exception(1011, source->line_cur, source);
					else
					{
						token->id = "||";
						token->line = line;
						token->type = TK_OP_OR;
						return token;
					}
				}
			}
		}
		else if (is_caracter_closed_parathesi(value))
			return NULL;
	}
}

/* Processa função if */
void fn_run_if(source_t* source, struct stack_t* stack_token, linked_list_t table_symbols, list_element_t* list_position)
{
	token_t* last_tk;
	int read = 0;
	stack_t* ids;
	int count_id = 0;
	stack_init(&ids);

	while (1)
	{
		last_tk = (token_t*)stack_pop(&stack_token);
		if (last_tk->type != TK_FN_IF)
		{
			stack_push(&ids, last_tk);
			count_id++;
		}
		else
			break;
	}

	if (last_tk->type == TK_FN_IF)
	{
		while (1)
		{
			token_t* id = stack_pop(&ids);
			count_id--;
			char * buffer = (char*)malloc(255);
			FillMemory(buffer, 255, 0);

			if (id->type == TK_TYPE)
				throw_exception(1011, source->line_cur, source);

			else if (count_id > 0 && id->type == TK_ID)
			{
				if (!list_any_tbl_symb(&table_symbols, list_position, id->id, NULL))
					throw_exception(1015, source->line_cur, source);
			}
			if (count_id == 0 || read == 1)
				break;
		}
	}

}