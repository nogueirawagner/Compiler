#include "token_stream.h"
#include "token_exception.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "stack.h"

/* Define o tipo do token */
token_type_t ts_get_type(char* value, token_t* last_tk, source_t* source)
{
	if (last_tk->type == TK_EQUAL)
		return TK_CONST;
	if (is_token_type_data(value, source))
		return TK_TYPE;
	if (is_token_variable(value))
		return TK_ID;
}

/* Define funçao */
token_type_t ts_get_type_fn(char* value)
{
	char* _gets = "gets";
	char* _if = "if";
	char* _for = "for";
	char* _then = "then";
	char* _else = "else";
	char* _puts = "puts";

	if (ts_are_equal(_gets, value))
		return TK_FN_GETS;
	if (ts_are_equal(_puts, value))
		return TK_FN_PUTS;
	if (ts_are_equal(_if, value))
		return TK_FN_IF;
	if (ts_are_equal(_for, value))
		return TK_FN_FOR;
}

/* Abre o arquivo em binário */
source_t * ts_open_source(char* source)
{
	source_t* psource = (source_t*)malloc(sizeof(source_t));
	psource->source = fopen(source, "rb");
	psource->last_pos = 0;
	psource->line_cur = 1;
	return psource;
}

void ts_close_source(char* source)
{
	fclose(source);
}

char ts_get_next_caracter(source_t* source)
{
	char value = getc(source->source);
	source->last_pos = ftell(source->source);
	source->last_read = value;
	return value;
}

/* Verifica se é um ponto e vírgula */
token_t * ts_get_token_delimiter(source_t* source)
{
	token_t token;

	char bufferc[255];
	FillMemory(&bufferc, 255, 0);

	char scopy[1] = { source->last_read };
	strncat(bufferc, scopy, 1);

	token.id = bufferc;
	token.line = source->line_cur;
	token.type = TK_STM_END;

	return &token;
}

/* Verifica se arquivo começa com main() */
int ts_begin_main(char value, source_t* source)
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

token_type_t ts_define_scope(token_t* last_tk)
{
	if (last_tk->type == TK_TYPE)
		return TK_ID;
	if (last_tk->type == TK_EQUAL)
		return TK_CONST;
	return TK_TYPE;
}

token_t* ts_get_token_fn_gets(source_t* source, token_t* last_token)
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

token_t* ts_get_token_fn_puts(source_t* source, token_t* last_token)
{
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);
	int line = source->line_cur;
	token_t* token = (token_t*)malloc(sizeof(token_t));

	if (!is_caracter_open_parathesi(source->last_read))
		throw_exception(1011, source->line_cur, source);
	puts(buffer);
	while (1)
	{
		char value = ts_get_next_caracter(source);
	}
	return NULL;
}



/* Pega próximo token */
token_t* ts_get_next_token(source_t* source, token_t* last_token, char* last_type)
{
	token_type_t scope;
	if (source->last_pos == 0)
		scope = TK_MAIN;
	if (source->last_pos > 7)
		scope = ts_define_scope(last_token);

	if (last_token->type == TK_FN_GETS)
		return ts_get_token_fn_gets(source, last_token);

	/*if (last_token->type == TK_FN_PUTS)
		ts_get_token_fn_puts(source, last_token);*/

	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);

	int line = source->line_cur;
	token_t* token = (token_t*)malloc(sizeof(token_t));
	while (1)
	{
		char value = ts_get_next_caracter(source);

		if (source->last_pos == 1 && !is_caracter_m(value))
			throw_exception(1001, 1, source);
		if (scope == TK_ID && !is_caracter_ampersand(value))
			throw_exception(1002, source->line_cur, source);
		else if (line == 1 && source->last_pos == 1)
		{
			ts_begin_main(value, source);
			line = source->line_cur;

			if (is_new_line(source->last_read) && is_caracter_smash_line(ts_get_next_caracter(source)))
			{
				source->line_cur++;
				source->init_pos_line = source->last_pos;
			}
			return NULL;
		}

		if (is_caracter_semicolon(value))
			return NULL;

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
				if (is_caracter_semicolon(value) || is_caracter_comma(value))
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

				value = ts_get_next_caracter(source); // Lê proximo caracter
				int tam = length_content_token(buffer);

				if (is_caracter_ampersand(value))
					throw_exception(1002, source->line_cur, source);

				if (tam == 1 && (is_numeric(value) || is_alphanumeric_toupper(value)))
					throw_exception(1002, source->line_cur, source);

				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value))
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
			token->id = "=";
			token->line = line;
			token->type = TK_EQUAL;
			return token;
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
					token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source->line_cur, source);
			}
		}
		else if (is_new_line(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // Lê proximo caracter
				if (is_new_line(value))
				{
					line++;
					source->line_cur = line;
					source->init_pos_line = source->last_pos;
					return NULL;
				}
			}
		}
		else if (is_caracter_key_closed(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // Lê proximo caracter
				if (value == -1)
				{
					source->last_read = value;
					return NULL;
				}
			}
		}
		source->last_pos += 1;
	};
	return NULL;
}

/* Verifica se é um token válido */
int is_token_valid(token_t* token, source_t* source)
{
	if (token->type == TK_TYPE)
	{
		if (!is_token_type_data(token->id, source))
			throw_exception(1007, source->line_cur, source); //Tipo não definido
	}
	else if (token->type == TK_ID) // Se é variavel
	{
		if (!is_caracter_ampersand(token->id[0]))
			throw_exception(1005, source->line_cur, source); //Erro ao declarar variavel deve iniciar com &
		else if (is_token_type_data(token->id, source))
			throw_exception(1006, source->line_cur, source); //Variavel com nome de palavra reservada
	}
	return 1;
}