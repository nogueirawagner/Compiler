#include "token_stream.h";
#include <stdio.h>;
#include <string.h>
#include <Windows.h>

// verifica se � caracter A...Z ou a...z
int is_alphanumeric(char value)
{
	return (value >= 65 && value <= 90) || (value >= 97 && value <= 122) || (value == 95);
}

int is_numeric(char value)
{
	return (value >= 48 && value <= 57);
}

// verifica se � caracter e-comercial '&'
int is_caracter_ampersand(char value)
{
	return (value == 38);
}

// verifica se � caracter ponto e v�rgula ';'
int is_caracter_semicolon(char value)
{
	return (value == 59);
}

// verifica se � caracter v�rgula ','
int is_caracter_comma(char value)
{
	return (value == 44);
}

// verifica se � operador de igualdade '='
int is_caracter_equals(char value)
{
	return (value == 61);
}

// verifica se � espa�o em branco
int is_space(char value)
{
	return (value == 32);
}

// verifica se � final de linha
int is_new_line(char value) {
	return (value == 13 || value == 10);
}

token_type_t ts_get_type(char * value)
{
	return TK_TYPE;
}

token_list_t * ts_save_tokens_table_symbols(token_list_t * token_next, token_t * token)
{
	token_next->token = (token_t*)malloc(sizeof(token_t));
	token_next->node = (token_list_t*)malloc(sizeof(token_list_t));

	token_next->token = &token;
	token_next = token_next->node;

	return token_next;
}

source_t* ts_open_source(char* source)
{
	source_t* psource = (source_t*)malloc(sizeof(source_t));
	psource->source = fopen(source, "rb");
	psource->last_pos = 0;

	return psource;
}

void ts_close_source(char* source)
{
	fclose(source);
}

token_t* ts_get_next_token(source_t* source)
{

	token_list_t * ts_inicio;
	token_list_t * ts_next_token;

	char buffer[255];
	FillMemory(&buffer, 255, 0);

	int line = 1;
	token_t token;
	while (1)
	{
		char value = getc(source->source);
		source->last_pos = ftell(source->source);

		if (is_alphanumeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = getc(source->source);
				source->last_pos = ftell(source->source);
				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value))
				{
					token.id = buffer;
					token.line = line;
					token.type = ts_get_type(token.id); // resolver

					return &token;
				}
			}
		}
		else if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = getc(source->source);
				source->last_pos = ftell(source->source);
				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value))
				{
					token.id = buffer;
					token.line = line;
					token.type = ts_get_type(token.id); // resolver 
					
					return &token;
				}
			}
		}
		else if (is_caracter_equals(value))
		{
			token.id = "=";
			token.line = line;
			token.type = TK_EQUAL;
			return &token;
		}
		else if (is_numeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = getc(source->source);
				source->last_pos = ftell(source->source);
				if (is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value))
				{
					token.id = buffer;
					token.line = line;
					token.type = ts_get_type(token.id); // resolver 

					return &token;
				}
			}
		}
		else if (is_new_line(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = getc(source->source);
				source->last_pos = ftell(source->source);
				if (is_new_line(value))
				{
					line++;
					return;
				}
			}
		}
		source->last_pos += 1;
	};

	return NULL;
}

int ts_is_token_type(token_t *token, token_type_t type)
{
	return (token->type == type);
}