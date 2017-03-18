#include <stdio.h>
#include "token_stream.h"

int main(int argc, char** argv) {
	
	source_t* source = ts_open_source("Source.chs");

	token_list_t * ts_inicio;
	token_list_t * ts_next_token;


	token_type_t types[5] = { TK_TYPE, TK_ID, TK_EQUAL, TK_CONST, TK_STM_END };
	int i = 0;
	while (1) 
	{

		token_type_t type_stat_end;
		token_t* tk = ts_get_next_token(source);

		//antes de chamar o proximo utilizar fseek para olhar o valor anterior a source-lastposition

		i++;
		/*ts_next_token = (token_list_t*)malloc(sizeof(token_list_t));
		ts_save_tokens_table_symbols(ts_next_token, &tk);*/

		//if (!ts_is_token_type(tk, types[i]))
		//	goto error;
		//i++;
	}

error:
	"erro ao processar";

	ts_close_source(source);
	return 0;
}
