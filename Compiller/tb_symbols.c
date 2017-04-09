#include <stdlib.h>
#include <stdio.h>
#include "token_stream.h"
#include "list.h"

void show_table_symbols()
{
	printf("Tabela de simbolos \n");
	printf("\n");
	printf("\t%-3s\t| %-15s\t| %-2s\t| %-20s\t|\t %-10s\n", "TIPO", "VARIAVEL", "TAM", "VALOR", "LINHA");
	printf("\t------------------------------------------------------------------------------------");
	printf("\n");
	/*for (int i = 0; i < list_get_size(table_symbols); i++)
	{
		table_symbols_t* object = (table_symbols_t*)list_position->data;

		char* variable = (char*)object->variable;
		char* tipo = (char*)object->type;
		char* value = (char*)object->value;
		char* length = (char*)object->length;
		int line = object->line;

		printf("\t%-3s\t| %-15s\t| %-2s\t| %-20s\t|\t %-10i\n", tipo, variable, length, value, line);
		list_position = list_next(list_position);
	}*/
}