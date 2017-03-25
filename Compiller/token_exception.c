#include <stdio.h>;
#include "token_stream.h";
#include <stdlib.h>;
#include <string.h>;

void te_generate_exception(int code, int line, source_t * source)
{
	switch (code)
	{
	case 1001:
		te_error_main(source);
	case 2:
		te_error_unknown(line);
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





