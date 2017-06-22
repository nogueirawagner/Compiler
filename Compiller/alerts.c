#include "token_stream.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "utils.h"

void throw_alert(int code, source_t * source)
{
	switch (code)
	{
	case 1001:
		alert_int_assing_dec(source);
		break;
	case 1002:
		alert_dec_assing_int(source);
		break;
	case 1003:
		alert_length_dec_diff(source);
		break;
	case 1004:
		alert_length_char_diff(source);
		break;
	default:
		0;
	}
}

/* Variavel do tipo inteira recebendo valor decimal */
int alert_int_assing_dec(source_t* source)
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
				if (is_caracter_semicolon(value))
				{
					source->count_alerts++;
					system("color E");
					printf("AL-1001 - atribuicao de valor decimal para o tipo int '%s' | linha: %i \n", buffer, source->line_cur);
					return 0;
				}
			}
		}
	}
	return 0;
}

/* Variavel do tipo decimal recebendo valor inteiro */
int alert_dec_assing_int(source_t* source)
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
				if (is_caracter_semicolon(value))
				{
					source->count_alerts++;
					system("color E");
					printf("AL-1002 - atribuicao de valor inteiro para o tipo decimal '%s' | linha: %i \n", buffer, source->line_cur);
					return 0;
				}
			}
		}
	}
	return 0;
}

/* Valores diferentes para tipos decimal */
int alert_length_dec_diff(source_t* source)
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
				if (is_caracter_semicolon(value))
				{
					source->count_alerts++;
					system("color E");
					printf("AL-1003 - declaracao de tamanho diferentes para variaveis do tipo decimal '%s' | linha: %i \n", buffer, source->line_cur);
					return 0;
				}
			}
		}
	}
	return 0;
}

/* Valores diferentes para tipos char */
int alert_length_char_diff(source_t* source)
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
				if (is_caracter_semicolon(value))
				{
					source->count_alerts++;
					system("color E");
					printf("AL-1004 - declaracao de tamanho diferentes para variaveis do tipo char '%s' | linha: %i \n", buffer, source->line_cur);
					return 0;
				}
			}
		}
	}
	return 0;
}

int alert_unknown()
{
	printf("Alerta indefinido");
	return 0;
}
