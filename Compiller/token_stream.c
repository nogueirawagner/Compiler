#include "token_stream.h";
#include "token_stack.h";
#include <stdio.h>;
#include <string.h>;
#include <Windows.h>;

// verifica se é caracter A...Z ou a...z
int is_alphanumeric(char value)
{
	return (value >= 65 && value <= 90) || (value >= 97 && value <= 122);
}

int is_numeric(char value)
{
	return (value >= 48 && value <= 57);
}

// verifica se é caracter e-comercial '&'
int is_caracter_ampersand(char value)
{
	return (value == 38);
}

// verifica se é caracter ponto e vírgula ';'
int is_caracter_semicolon(char value)
{
	return (value == 59);
}

// verifica se é caracter vírgula ','
int is_caracter_comma(char value)
{
	return (value == 44);
}

// verifica se é operador de igualdade '='
int is_caracter_equals(char value)
{
	return (value == 61);
}

// verifica se é espaço em branco
int is_space(char value)
{
	return (value == 32);
}

// verifica se é final de linha
int is_new_line(char value) {
	return (value == 13 || value == 10);
}

token_type_t ts_get_type(char * value)
{
	//char *pointer = "int";
	//if (strcmp(pointer, value) == 0) { // match!
	//	return TK_INT;
	//}

	return TK_TYPE;

}

source_t * ts_open_source(char * source)
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

char ts_get_next_caracter(source_t * source)
{
	char value = getc(source->source);
	source->last_pos = ftell(source->source);
	source->last_read = value;
	return value;
}

token_t * ts_get_token_delimiter(source_t * source)
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

int ts_is_token_type(token_t *token, token_type_t type)
{
	return (token->type == type);
}

token_t * ts_get_next_token(source_t* source)
{
	char* buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);

	int line = 1;
	source->line_cur = line;
	token_t* token = (token_t*)malloc(sizeof(token_t));
	while (1)
	{
		if (is_caracter_semicolon(source->last_read))
			return ts_get_token_delimiter(source);

		char value = ts_get_next_caracter(source);

		if (is_alphanumeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // Lê próximo caracter
				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id); // resolver
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

				value = ts_get_next_caracter(source); // Lê proximo caracter
				if (is_space(value) || is_caracter_semicolon(value) || is_caracter_comma(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id); // resolver 
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

				value = ts_get_next_caracter(source); // Lê proximo caracter
				if (is_space(value) || is_caracter_comma(value) || is_caracter_semicolon(value))
				{
					token->id = buffer;
					token->line = line;
					token->type = ts_get_type(token->id); // resolver 

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

				value = ts_get_next_caracter(source); // Lê proximo caracter
				if (is_new_line(value))
				{
					line++;
					source->line_cur = line;
					return;
				}
			}
		}
		source->last_pos += 1;
	};

	return NULL;
}

