#include <stdio.h>
#include "token_stream.h"

int main(int argc, char** argv) {

	
	source_t* source = ts_open_source("Source.chs");

	token_type_t types[5] = { TK_TYPE, TK_ID, TK_EQUAL, TK_CONST, TK_STM_END };
	int i = 0;
	while (1) 
	{
		token_t* tk = ts_get_next_token(source);
		if (!ts_is_token_type(tk, types[i]))
			goto error;

		i++;
	}

error:
	"erro ao processar";

	ts_close_source(source);
	return 0;
}
