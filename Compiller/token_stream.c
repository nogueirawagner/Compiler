#include "token_stream.h";
#include "token_stack.h";
#include "token_exception.h";
#include <stdio.h>;
#include <string.h>;
#include <Windows.h>;

/* Verifica se � caracter A...Z ou a...z */
int is_alphanumeric(char value)
{
	return (value >= 65 && value <= 90) || (value >= 97 && value <= 122);
}

/* Verifica se � n�merico */
int is_numeric(char value)
{
	return (value >= 48 && value <= 57);
}

/* Verifica se � caracter e-comercial '&' */
int is_caracter_ampersand(char value)
{
	return (value == 38);
}

/* Verifica se � caracter ponto e v�rgula ';' */
int is_caracter_semicolon(char value)
{
	return (value == 59);
}

/* Verifica se � caracter v�rgula ',' */
int is_caracter_comma(char value)
{
	return (value == 44);
}

/* Verifica se � operador de igualdade '=' */
int is_caracter_equals(char value)
{
	return (value == 61);
}

/* Verifica se � espa�o em branco */
int is_space(char value)
{
	return (value == 32);
}

/* Verifica se � final de linha */
int is_new_line(char value)
{
	return (value == 13 || value == 10);
}

/* Verifica se � um caracter '\n' */
int is_caracter_smash_line(char value)
{
	return (value == 10);
}

/* Verifica se � abre chaves */
int is_caracter_key_opened(char value)
{
	return (value == 123);
}

/* Verificar se ponteiros s�o iguais */
int ts_are_equal(char * pointer1, char * pointer2)
{
	return !(strcmp(pointer1, pointer2));
}

/* Define o tipo do token */
token_type_t ts_get_type(char * value)
{
	//char *pointer = "int";
	//if (strcmp(pointer, value) == 0) { // match!
	//	return TK_INT;
	//}

	return TK_TYPE;

}

/* Abre o arquivo em bin�rio */
source_t * ts_open_source(char * source)
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

char ts_get_next_caracter(source_t * source)
{
	char value = getc(source->source);
	source->last_pos = ftell(source->source);
	source->last_read = value;
	return value;
}

/* Verifica se */
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

/* Verifica se arquivo come�a com main() */
int ts_begin_main(char value, source_t * source)
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
						source->line_cur++;
				}

				if (!is_new_line(value))
				{
					char scopy[1] = { value };
					strncat(buffer, scopy, 1);
					tam++;
					if (tam > 7)
						te_generate_exception(1001, 1, source);
				}

				value = ts_get_next_caracter(source); // L� pr�ximo caracter
				if (tam == 7 && ts_are_equal(main, buffer))
				{
					return 1;
				}
			}
		}
		else
			te_generate_exception(1001, 1, source);
	}
	return 0;
}

/* Pega pr�ximo token */
token_t * ts_get_next_token(source_t * source)
{
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);

	int line = source->line_cur;
	token_t* token = (token_t*)malloc(sizeof(token_t));
	while (1)
	{
		/* Verificar se � um final de linha */
		if (is_caracter_semicolon(source->last_read))
			return ts_get_token_delimiter(source);

		char value = ts_get_next_caracter(source);

		/* Verificar se � primeira linha e se a primeira letra � 'm' */
		if (source->last_pos == 1 && value != 109)
			te_generate_exception(1001, 1, source);
		else if (line == 1) /* Valida a palavra reservada main */
		{
			ts_begin_main(value, source);
			line = source->line_cur;
			value = source->last_read;
		}

		/* Verificar se � qualquer letra de A...Z e a...z */
		if (is_alphanumeric(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = ts_get_next_caracter(source); // L� pr�ximo caracter
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

				value = ts_get_next_caracter(source); // L� proximo caracter
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

				value = ts_get_next_caracter(source); // L� proximo caracter
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

				value = ts_get_next_caracter(source); // L� proximo caracter
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

