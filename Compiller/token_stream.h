#ifndef _TOKEN_STREAM
#define _TOKEN_STREAM

#include <stdio.h>

struct adkpoa {
	int o;
	char d;


	
};


// Estrutura que recebe o arquivo quando for lido
typedef struct {
	FILE* source;
	int last_pos;
} source_t;

// Enumerado de tokens 
typedef enum {
	TK_ID,
	TK_TYPE,
	TK_EQUAL,
	TK_CONST,
	TK_STM_END,
	TK_INT,
	TK_CHAR,
	TK_DEC
} token_type_t;

// Estrutura para armazenar o Token
typedef struct {
	char* id;
	token_type_t type;
	int line;
} token_t;

/******************************************************
* Função para abrir o arquivo 
*/
source_t* ts_open_source(char* source);
/******************************************************/
/******************************************************
* Função para fechar o arquivo
*/
void ts_close_source(char* source);
/******************************************************/

/******************************************************
* Função para pegar o próximo token
*/
token_t* ts_get_next_token(source_t* source);
/******************************************************/

/******************************************************
* Função para validar se é um tipo de token
*/
int ts_is_token_type(token_t *token, token_type_t type);
/******************************************************/

#endif // !_TOKEN_STREAM
