#ifndef _TOKEN_STREAM
#define _TOKEN_STREAM

#include <stdio.h>

typedef struct {
	FILE* source;
	int last_pos;
} source_t;

typedef enum {
	TK_ID,
	TK_TYPE,
	TK_EQUAL,
	TK_CONST,
	TK_STM_END
} token_type_t;

typedef struct {
	char* id;
	token_type_t type;
	int line;
} token_t;

source_t* ts_open_source(char* source);
token_t* ts_get_next_token(source_t* source);


int ts_is_token_type(token_t *token, token_type_t type);

#endif // !_TOKEN_STREAM
