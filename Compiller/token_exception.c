#include <stdio.h>;
#include <stdlib.h>;
#include <string.h>;

void te_generate_exception(int code)
{
	switch (code)
	{
	case 1001:
		te_error_unknown();
	case 2:
		te_error_unknown();
	default:
		te_error_unknown();
	}
}


int te_error_unknown() {
	printf("Erro desconhecido ");
	getchar();
	return 0;
}






