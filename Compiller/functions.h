#ifndef _FUNCTIONS
#define _FUNCTIONS

#include "token_stream.h"

/* Verifica se arquivo come�a com main() */
int fn_main(char value, source_t* source);

/* Fun��o gets */
token_t* fn_gets(source_t* source, token_t* last_token);

/* Fun��o puts */
token_t* fn_puts(source_t* source, token_t* last_token);

#endif // !_FUNCTIONS
