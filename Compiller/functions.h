#ifndef _FUNCTIONS
#define _FUNCTIONS

#include "token_stream.h"

/* Verifica se arquivo começa com main() */
int fn_main(char value, source_t* source);

/* Função gets */
token_t* fn_gets(source_t* source, token_t* last_token);

/* Função puts */
token_t* fn_puts(source_t* source, token_t* last_token);

#endif // !_FUNCTIONS
