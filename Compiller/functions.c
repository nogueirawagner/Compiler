#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "token_stream.h"
#include "utils.h"
#include "token_exception.h"

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
	}
	return NULL;
}