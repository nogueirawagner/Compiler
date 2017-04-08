#ifndef _TOKEN_STREAM
#define _TOKEN_STREAM

#include <stdio.h>

/* Enumerado de tokens */
typedef enum {
	TK_ID,			/* define o identificador do token ou variavel */
	TK_TYPE,		/* define se o token � um tipo de dado (int, char, long, dec) */
	TK_EQUAL,		/* define se o token � operador aritmetico de igualdade */
	TK_CONST,		/* define se o token � um valor atribuido */
	TK_STM_END  /* define se o token � um ponto e v�rgula */
} token_type_t;

/* Estrutura para armazenar o Token */
typedef struct {
	char * id;
	token_type_t type;
	int line;
} token_t;

/* Tabela de tokens armazenados */
typedef struct {
	token_t * token;
	struct token_stack_t * next_token;
} token_stack_t;

/* Tabela de simbols */
typedef struct {
	char* type;
	char* variable;
	char* length;
	char* value;
	int line;
} table_symbols_t;

/* Estrutura que recebe o arquivo quando for lido */
typedef struct {
	FILE * source;			/* ponteiro para arquivo */
	int last_pos;			/* �ltima posi��o */
	char last_read;			/* �ltimo caracter lido */
	int line_cur;			/* linha atual */
	int init_pos_line;		/* posi��o onde come�a a linha atual */
} source_t;

/* Fun��o para abrir o arquivo */
source_t * ts_open_source(char* source);

/* Fun��o para fechar o arquivo */
void ts_close_source(char* source);

/* Fun��o para pegar o pr�ximo token */
token_t* ts_get_next_token(source_t* source, token_t* last_token, char* last_type);

/* Fun��o para pegar o proximo caracter */
char ts_get_next_caracter(source_t * source);

/* Fun��o para retornar o token quando o delimitador for encontrado */
token_t * ts_get_token_delimiter(source_t * source);

/* Verifica se � um token v�lido */
int is_token_valid(token_t* token, source_t* source);


#endif // !_TOKEN_STREAM
