#include "token_stream.h"
#include <Windows.h>
#include "token_exception.h"

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

/* Verifica se � abre chaves '{' */
int is_caracter_key_opened(char value)
{
	return (value == 123);
}

/* Verifica se � caracter fecha chaves '}' */
int is_caracter_key_closed(char value)
{
	return (value == 125);
}

/* Verifica se caracter � letra 'm' */
int is_caracter_m(char value)
{
	return (value == 109);
}

/* Verificar se ponteiros s�o iguais */
int ts_are_equal(char* pointer1, char* pointer2)
{
	return !(strcmp(pointer1, pointer2));
}

/* Define se o token � um tipo de dado */
int is_token_type_data(char* value)
{
	char * tipos[4] = { "int", "dec", "char" };

	for (int i = 0; i < 3; i++)
	{
		if (strcmp(tipos[i], value) == 0)
			return 1;
	}
	return 0;
}

/* Define se o token � uma vari�vel */
int is_token_variable(char* value)
{
	return is_caracter_ampersand(*value);
}

/* Retorna o tamanho do conte�do do ponteiro */
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

/* Verifica se � operadores relacional */
int is_caracter_relational(char value)
{
	if (value == 33			/* ! */
		|| value == 60		/* < */
		|| value == 62		/* > */
		|| is_caracter_equals(value));		/* = */
}

/* Verifica se � operadores l�gicos */
int is_operator_logic(char value, char last_value)
{

	//	Operadores logicos
	//	&& ||

}

/* Verifica se � aspas duplas ' "" '  */
int is_caracter_quotes_plus(char value)
{
	return (value == 34);
}

/* Verifica se � abre parenteses '(' */
int is_caracter_open_parathesi(char value)
{
	return (value == 40);
}

/* Verifica se � fecha parenteses ')' */
int is_caracter_closed_parathesi(char value)
{
	return (value == 41);
}

/* Verifica se � ponto '.' */
int is_caracter_point(char value)
{
	return (value == 46);
}

/* Verifica se nao � tabula��o */
int is_caracter_tab(char value)
{
	return (value == 9);
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
							throw_exception(1010, source->line_cur, source);
					}


					if (!is_caracter_closed_parathesi(caracter))
					{
						if (is_caracter_point(caracter))
							leuPonto++;
						if (!(leuPonto <= 2))
							throw_exception(1010, source->line_cur, source);
						char scopy[1] = { caracter };
						strncat(buffer, scopy, 1);
					}
					if (is_caracter_closed_parathesi(caracter))
					{
						if (leuPonto == 1)
							return buffer;
						else
							throw_exception(1010, source->line_cur, source);
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
							throw_exception(1009, source->line_cur, source);
						char scopy[1] = { caracter };
						strncat(buffer, scopy, 1);
					}
					if (is_caracter_closed_parathesi(caracter))
					{
						int toint = atoi(buffer);
						if (toint > 0 && toint > 255)
							throw_exception(1009, source->line_cur, source);
						return buffer;
					}

				}
			}
		}
	}
	return 0;
}