#include "token_stream.h"
#include <Windows.h>
#include "exception.h"
#include <ctype.h>

/* Verifica se é caracter A...Z ou a...z */
int is_alphanumeric(char value)
{
	return (value >= 65 && value <= 90) || (value >= 97 && value <= 122);
}

/* Verifica se é caracter a...z */
int is_alphanumeric_tolower(char value)
{
	return  (value >= 97 && value <= 122);
}

/* Verifica se é caracter a...z */
int is_alphanumeric_toupper(char value)
{
	return (value >= 65 && value <= 90);
}

/* Verifica se é númerico */
int is_numeric(char value)
{
	return (value >= 48 && value <= 57);
}

/* Verifica se é um número inteiro*/
int is_numeric_int(char* value, source_t* source)
{
	int tam = length_content_token(value);
	int anyPonto = 0;
	for (int i = 0; i < tam; i++)
	{
		if (is_caracter_point(value[i]))
			anyPonto++;

		if (anyPonto > 1)
			return 0;
		if (!(is_numeric(value[i]) || is_caracter_point(value[i])))
			return 0;
	}

	if (anyPonto == 1) 
		throw_alert(1001, source);

	return 1;
}

/* Verifica se é um número decimal */
int is_numeric_decimal(char* value, source_t* source)
{
	int tam = length_content_token(value);
	int anyPonto = 0;
	for (int i = 0; i < tam; i++)
	{
		if (is_caracter_point(value[i]))
			anyPonto++;

		if (anyPonto > 1)
			return 0;
		if (!(is_numeric(value[i]) || is_caracter_point(value[i])))
			return 0;
	}

	if (anyPonto == 0)
		throw_alert(1002, source);

	return 1;
}

/* Verifica se é caracter e-comercial '&' */
int is_caracter_ampersand(char value)
{
	return (value == 38);
}

/* Verifica se é caracter ponto e vírgula ';' */
int is_caracter_semicolon(char value)
{
	return (value == 59);
}

/* Verifica se é caracter vírgula ',' */
int is_caracter_comma(char value)
{
	return (value == 44);
}

/* Verifica se é operador de igualdade '=' */
int is_caracter_equals(char value)
{
	return (value == 61);
}

/* Verifica se é espaço em branco */
int is_space(char value)
{
	return (value == 32);
}

/* Verifica se é final de linha */
int is_new_line(char value)
{
	return (value == 13 || value == 10);
}

/* Verifica se é um caracter '\n' */
int is_caracter_smash_line(char value)
{
	return (value == 10);
}

/* Verifica se é abre chaves '{' */
int is_caracter_key_opened(char value)
{
	return (value == 123);
}

/* Verifica se é caracter fecha chaves '}' */
int is_caracter_key_closed(char value)
{
	return (value == 125);
}

/* Verifica se caracter é letra 'm' */
int is_caracter_m(char value)
{
	return (value == 109);
}

/* Verificar se ponteiros são iguais */
int ts_equals_to(char* pointer1, char* pointer2)
{
	return !(strcmp(pointer1, pointer2));
}

/* Verificar se é caracter ' + '*/
int is_caracter_plus(char value)
{
	return (value == 43);
}

/* Verifica se é caracter menos ' - ' */
int is_caracter_less(char value)
{
	return (value == 45);
}

/* Define se é palavra reservada para funcoes */
int is_token_function(char* value, source_t* source)
{
	char * tipos[6] = { "gets", "puts", "if", "then", "else", "for" };

	for (int i = 0; i < 6; i++)
	{
		if (strcmpi(tipos[i], value) == 0)
		{
			int tam = length_content_token(value);
			for (int j = 0; j < tam; j++)
			{
				if (value[j] >= 65 && value[j] <= 90)
					throw_exception(1011, source);
			}
			return 1;
		}
	}
	return 0;
}

/* Define se o token é um tipo de dado */
int is_token_type_data(char* value, source_t* source)
{
	char * tipos[3] = { "int", "dec", "char" };

	for (int i = 0; i < 3; i++)
	{
		if (strcmpi(tipos[i], value) == 0)
		{
			int tam = length_content_token(value);
			for (int j = 0; j < tam; j++)
			{
				if (value[j] >= 65 && value[j] <= 90)
					throw_exception(1002, source);
			}
			return 1;
		}
	}
	return 0;
}

/* Define se o token é uma variável */
int is_token_variable(char* value)
{
	return is_caracter_ampersand(*value);
}

/* Retorna o tamanho do conteúdo do ponteiro */
int length_content_token(char* value)
{
	int b = 0;
	int length = 0;
	while (value[b] != 0)
	{
		if (value[b] != 0)
			length++;
		else
			return length;
		b++;
	}
	return length;
}

/* Retorna texto a partir de tamanho especifico de string */
char* content_substring(char* value, int pos_ini, int pos_end)
{
	int tam = length_content_token(value);
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);

	if (pos_ini > tam)
		return "";
	if (pos_end > tam)
		pos_end = tam;

	for (int i = pos_ini; i < pos_end; i++)
	{
		char caracter = value[i];
		if (is_caracter_open_parathesi(caracter))
			break;
		else
		{
			char scopy[1] = { caracter };
			strncat(buffer, scopy, 1);
		}
	}
	return buffer;
}

/* Retorna o tamanho do conteúdo do ponteiro do tipo char */
int length_content_token_char(char* value)
{
	int b = 0;
	int length = 0;
	while (value[b] != 0)
	{
		if (value[b] != 0)
		{
			if (value[b] != 34 && !is_caracter_plus(value[b]))
				length++;
		}
		else
			return length;
		b++;
	}
	return length;
}

/* Verifica se é caracter inválido para variaveis */
int is_caracter_invalid_var(char value)
{
	return ((value >= 33 && value <= 37) || value == 39 || (value >= 42 && value <= 43) || value == 45 || value == 47 || value == 58 || (value >= 60 && value <= 64) || (value >= 91 && value <= 96) || (value >= 123 && value <= 127));
}

/* Verifica se é operadores relacional */
int is_caracter_relational(char value)
{
	return (value == 33						/* ! */
		|| value == 60						/* < */
		|| value == 62						/* > */
		|| is_caracter_equals(value));		/* = */
}

/* Verifica se é operador aritmetico */
int is_caracter_arimetic(char value)
{
	return (value == 43		/* + */
		|| value == 45		/* - */
		|| value == 42		/* * */
		|| value == 47);	/* / */
}

/* Verifica se é operador lógico */
int is_caracter_logic(char value)
{
	return (value == 38 || value == 124);
}

/* Verifica se é aspas duplas ' "" '  */
int is_caracter_quotes_plus(char value)
{
	return (value == 34);
}

/* Verifica se é abre parenteses '(' */
int is_caracter_open_parathesi(char value)
{
	return (value == 40);
}

/* Verifica se é fecha parenteses ')' */
int is_caracter_closed_parathesi(char value)
{
	return (value == 41);
}

/* Verifica se é ponto '.' */
int is_caracter_point(char value)
{
	return (value == 46);
}

/* Verifica se nao é tabulação */
int is_caracter_tab(char value)
{
	return (value == 9);
}

/* Verifica se é um token de função */
int is_token_type_function(token_type_t type)
{
	return (type == TK_FN_GETS || type == TK_FN_FOR || type == TK_FN_IF || type == TK_FN_PUTS);
}

/* Verifica se é caracter barra ' | ' */
int is_caracter_bar(char value)
{
	return (value == 124);
}

/* Verifica tamanho da variavel Ex: dec &x(1.5) ou char &y(15) */
char* any_definition_length(char* value, source_t* source, int isDec)
{
	int tam = length_content_token(value);
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);

	if (isDec)
	{
		int leuPonto = 0;

		for (int i = 0; i < tam; i++)
		{
			char caracter = value[i];
			if (is_caracter_open_parathesi(caracter))
			{
				while (1)
				{
					i++;
					caracter = value[i];

					if (!is_numeric(caracter))
					{
						if (!is_caracter_closed_parathesi(caracter) && !is_caracter_point(caracter))
							throw_exception(1010, source);
					}

					if (!is_caracter_closed_parathesi(caracter))
					{
						if (is_caracter_point(caracter))
							leuPonto++;
						if (!(leuPonto <= 2))
							throw_exception(1010, source);
						char scopy[1] = { caracter };
						strncat(buffer, scopy, 1);
					}
					if (is_caracter_closed_parathesi(caracter))
					{
						if (leuPonto == 1)
							return buffer;
						else
							throw_exception(1010, source);
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < tam; i++)
		{
			char caracter = value[i];
			if (is_caracter_open_parathesi(caracter))
			{
				while (1)
				{
					i++;
					caracter = value[i];
					if (!is_caracter_closed_parathesi(caracter))
					{
						if (!is_numeric(caracter))
							throw_exception(1009, source);
						char scopy[1] = { caracter };
						strncat(buffer, scopy, 1);
					}
					if (is_caracter_closed_parathesi(caracter))
					{
						int toint = atoi(buffer);
						if (toint < 1)
							throw_exception(1009, source);
						if (toint > 255)
							throw_exception(1009, source);
						return buffer;
					}
				}
			}
		}
	}
	return 0;
}

/* Retorna somente variavel sem tamanho */
char* content_variable_id(char* value)
{
	int tam = length_content_token(value);
	char * buffer = (char*)malloc(255);
	FillMemory(buffer, 255, 0);

	for (int i = 0; i < tam; i++)
	{
		char caracter = value[i];
		if (is_caracter_open_parathesi(caracter))
			break;
		else
		{
			char scopy[1] = { caracter };
			strncat(buffer, scopy, 1);
		}
	}
	return buffer;
}