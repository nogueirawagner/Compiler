#include "token_stream.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "utils.h"

void throw_exception(int code, int line, source_t * source)
{
	switch (code)
	{
	case 1001:
		te_error_main(source);
	case 1002:
		te_error_declare_var(source);
	case 1003:
		te_error_var_not_declared(source);
	case 1004:
		te_error_var_already_declared(source);
	case 1005:
		te_error_var_declared(source);
	case 1006:
		te_error_var_already_declared(source);
	case 1007:
		te_error_var_already_declared(source);
	case 1008:
		te_error_type_undefined(source); //Tipo n encontrado
	default:
		te_error_unknown(line);
	}
}

/* Erro desconhecido */
int te_error_unknown(int line) {
	printf("Erro desconhecido ");
	getchar();
	return 0;
}

/* Main não declarado */
int te_error_main(source_t * source)
{
	char linha[1000];
	fgets(linha, 1000, source->source);

	printf("TE-1001 - Falta do comando main na linha 1 \n");
	printf("Esperado: main() | Declarado: %s ", linha);
	getchar();
	exit(1);
	return 0;
}

/* Erro na declaração de variável */
int te_error_declare_var(source_t* source)
{
	fseek(source->source, source->init_pos_line, SEEK_SET);
	char linha[1000];
	fgets(linha, 1000, source->source);

	printf("TE-1002 - Erro na declaracao de variavel | linha: %i \n", source->line_cur);
	printf("%s", linha);
	getchar();
	exit(1);
	return 0;
}

/* Variável não declarada  */
int te_error_var_not_declared(source_t* source)
{
	fseek(source->source, source->init_pos_line, SEEK_SET);
	char buffer[255];
	FillMemory(buffer, 255, 0);

	while (1)
	{
		char value = ts_get_next_caracter(source);
		if (is_caracter_ampersand(value)) 
		{
			while (1) 
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);
				value = ts_get_next_caracter(source);
				if (is_caracter_equals(value))
				{
					printf("TE-1003 - Identificador %s nao definido | linha: %i \n", buffer, source->line_cur);
					getchar();
					exit(1);
					return 0;
				}
			}
		}
	}
}

int te_error_var_already_declared(source_t* source)
{ 
	fseek(source->source, source->init_pos_line, SEEK_SET);
	char buffer[255];
	FillMemory(buffer, 255, 0);

	while (1)
	{
		char value = ts_get_next_caracter(source);
		if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);
				value = ts_get_next_caracter(source);
				if (is_caracter_equals(value))
				{
					printf("TE-1004 - Identificador %s ja foi declarado | linha: %i \n", buffer, source->line_cur);
					getchar();
					exit(1);
					return 0;
				}
			}
		}
	}
	return 0;
}


int te_error_var_declared(source_t* source)
{
	fseek(source->source, source->init_pos_line, SEEK_SET);
	char buffer[255];
	FillMemory(buffer, 255, 0);

	while (1)
	{
		char value = ts_get_next_caracter(source);
		if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);
				value = ts_get_next_caracter(source);
				if (is_caracter_equals(value))
				{
					printf("TE-1005 - Erro ao declarar variavel %s deve iniciar com o caracter '&' | linha: %i \n", buffer, source->line_cur);
					getchar();
					exit(1);
					return 0;
				}
			}
		}
	}
	return 0;
}

int te_error_type_undefined(source_t* source)
{
	fseek(source->source, source->init_pos_line, SEEK_SET);
	char buffer[255];
	FillMemory(buffer, 255, 0);

	while (1)
	{
		char value = ts_get_next_caracter(source);
		if (is_caracter_ampersand(value))
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);
				value = ts_get_next_caracter(source);
				if (is_caracter_equals(value))
				{
					printf("TE-1008 - Tipo de dado nao definido | linha: %i \n", source->line_cur);
					getchar();
					exit(1);
					return 0;
				}
			}
		}
	}
	return 0;
}

