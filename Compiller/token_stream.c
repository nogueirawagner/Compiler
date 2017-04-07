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
	if (is_token_type_data(value))
		return TK_TYPE;
	if (is_token_variable(value))
		return TK_ID;
}

/* Abre o arquivo em bin�rio */
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

/* Verifica se arquivo come�a com main() */
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

				value = ts_get_next_caracter(source); // L� pr�ximo caracter
				if (tam == 7 && ts_are_equal(main, buffer))
				{
					return 1;
				}
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
}


/* Pega pr�ximo token */
token_t* ts_get_next_token(source_t* source, token_t* last_token)
{
	token_type_t scope = ts_define_scope(last_token);

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
		else if (line == 1 && source->last_pos == 1) /* Valida a palavra reservada main */
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
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // L� pr�ximo caracter
				if (is_caracter_quotes_plus(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);

					token->id = buffer;
					token->line = line;
					token->type = TK_CONST;
					return token;
				}
			}
		}
		/* Verificar se � qualquer letra de A...Z e a...z */
		if (is_alphanumeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // L� pr�ximo caracter
				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source); // resolver
					return token;
				}
			}
		}
		else if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // L� proximo caracter
				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value) || is_caracter_relational(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id, last_token, source); // resolver 
					return token;
				}
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
		if (!is_token_type_data(token->id))
			throw_exception(1007, source->line_cur, source); //Tipo n�o definido
	}
	else if (token->type == TK_ID) // Se � variavel
	{
		if (!is_caracter_ampersand(token->id[0]))
			throw_exception(1005, source->line_cur, source); //Erro ao declarar variavel deve iniciar com &
		else if (is_token_type_data(token->id))
			throw_exception(1006, source->line_cur, source); //Variavel com nome de palavra reservada
	}
	return 1;
}