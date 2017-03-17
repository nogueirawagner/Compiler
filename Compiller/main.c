#include <stdio.h>
#include "token_stream.h"

int main(int argc, char** argv) {

	source_t* source = ts_open_source("C:\\Users\\ephrom\\Documents\\Visual Studio 2015\\Projects\\Compiller\\Debug\\Source.chs");

	token_type_t types[5] = { TK_TYPE, TK_ID, TK_EQUAL, TK_CONSTINT, TK_STM_END };
	int i = 0;
	while (1) 
	{

		token_t* tk = ts_get_next_token(source);
		if (!ts_is_token_type(tk, types[i]))
			break;

		i++;
	}
	return 0;
}