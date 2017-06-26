#include "token_stream.h"
#include "exception.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "stack.h"
#include "functions.h"

/* Define o tipo do token */
token_type_t ts_get_type(char* value, token_t* last_tk, source_t* source)
{
	
	if (last_tk->type == TK_EQUAL)
		return TK_CONST;
	if (is_token_type_data(value, source))
		return TK_TYPE;
	if (is_token_variable(value))
		return TK_ID;
	if (is_caracter_relational(value))
		return TK_RELATIONAL;
}

/* Define fun�ao */
token_type_t ts_get_type_fn(char* value)
{
	char* _gets = "gets";
	char* _if = "if";
	char* _for = "for";
	char* _puts = "puts";

	if (equals_to(_gets, value))
		return TK_FN_GETS;
	if (equals_to(_puts, value))
		return TK_FN_PUTS;
	if (equals_to(_if, value))
		return TK_FN_IF;
	if (equals_to(_for, value))
		return TK_FN_FOR;
	
}

/* Verifica se palavra n�o se trata de um n� interno de fun��o */
int is_child_function(char* value)
{
	char* _else = "else";
	char* _then = "then";
	if (equals_to(_else, value))
		return 1;
	if (equals_to(_then, value))
		return 1;
	return 0;
}


/* Abre o arquivo em bin�rio */
source_t * ts_open_source(char* source)
{
	source_t* psource = (source_t*)malloc(sizeof(source_t));
	psource->source = fopen(source, "rb");
	psource->last_pos = 0;
	psource->line_cur = 1;
	psource->count_alerts = 0;
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

/* Verifica se � um ponto e v�rgula */
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

token_type_t ts_define_scope(token_t* last_tk)
{
	if (last_tk->type == TK_TYPE)
		return TK_ID;
	if (last_tk->type == TK_EQUAL)
		return TK_CONST;
	return TK_TYPE;
}

/* Pega pr�ximo token */
token_t* ts_get_next_token(source_t* source, token_t* last_token, token_type_t last_func)
{
	token_type_t scope;
	if (source->last_pos == 0)
		scope = TK_MAIN;
	if (source->last_pos > 7)
		scope = ts_define_scope(last_token);

	if (last_token->type == TK_FN_GETS || last_func == TK_FN_GETS)
		return fn_gets(source, last_token);

	if (last_token->type == TK_FN_PUTS || last_func == TK_FN_PUTS)
		return fn_puts(source, last_token);

	if (last_token->type == TK_FN_FOR || last_func == TK_FN_FOR)
		return fn_for(source, last_token);

	if (last_token->type == TK_FN_IF || last_func == TK_FN_IF)
		return fn_if(source, last_token);

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
			throw_exception(1002, source);
		else if (line == 1 && source->last_pos == 1)
		{
			fn_main(value, source);
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
					throw_exception(1013, source);
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
						throw_exception(1013, source);
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
						throw_exception(1013, source);
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
								throw_exception(1013, source);
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
					if (is_token_function(buffer, source) && !is_child_function(buffer))
						token->type = ts_get_type_fn(buffer);
					else 
					{
						if(!is_child_function(buffer))
							token->type = ts_get_type(token->id, last_token, source);
						else
							throw_exception(1002, source);
					}
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source);
			}
		}
		else if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // L� proximo caracter
				int tam = length_content_token(buffer);

				if(is_caracter_invalid_var(value))
					throw_exception(1002, source);

				if (is_caracter_ampersand(value))
					throw_exception(1002, source);

				if (tam == 1 && (is_numeric(value) || is_alphanumeric_toupper(value)))
					throw_exception(1002, source);

				if (tam > 1)
					if (!(is_numeric(value) || is_alphanumeric(value) || is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value) || is_caracter_open_parathesi(value) || is_caracter_point(value) || is_caracter_closed_parathesi(value)))
						throw_exception(1002, source);

				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source);
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

				value = ts_get_next_caracter(source); // L� proximo caracter
				if (is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value) || is_caracter_relational(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source);
					return token;
				}
				if (is_new_line(value))
					throw_exception(1012, source);
			}
		}
		else if (is_new_line(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // L� proximo caracter
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

				value = ts_get_next_caracter(source); // L� proximo caracter
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

/* Verifica se � um token v�lido */
int is_token_valid(token_t* token, source_t* source)
{
	if (token->type == TK_TYPE)
	{
		if (!is_token_type_data(token->id, source))
			throw_exception(1008, source); //Tipo n�o definido
	}
	else if (token->type == TK_ID) // Se � variavel
	{
		if (!is_caracter_ampersand(token->id[0]))
			throw_exception(1005, source); //Erro ao declarar variavel deve iniciar com &
		else if (is_token_type_data(token->id, source))
			throw_exception(1006, source); //Variavel com nome de palavra reservada
	}
	return 1;
}