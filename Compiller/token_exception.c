#include <stdio.h>;
#include <stdlib.h>;
#include <string.h>;

void te_generate_exception(char code)
{
	switch (code)
	{
	case 'E001':
		printf("kapodsd");
		exit(1);
	case 'E002':
		printf("aok kodad");
		exit(1);
	default:
		break;
	}
}