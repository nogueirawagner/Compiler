#ifndef _TOKEN_STREAM
#define _TOKEN_STREAM

#include <stdio.h>

/* Enumerado de tokens */
typedef enum {
	TK_ID,			/* define o identificador do token ou variavel */
	TK_TYPE,		/* define que token é um tipo de dado (int, char, long, dec) */
	TK_EQUAL,		/* define que token é operador aritmetico de igualdade */
	TK_CONST,		/* define que token é um valor atribuido */
	TK_STM_END,		/* define que token é um ponto e vírgula */
	TK_MAIN,		/* define que token é um main */
	TK_FN_GETS,		/* define se é uma função get */
	TK_FN_IF,		/* define se é uma função if */
	TK_FN_FOR		/* define se é uma for */
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

/* Tabela de funcoes */
typedef struct {
	char* id;
	char* value;
	int line;
} table_functions_t;

/* Estrutura que recebe o arquivo quando for lido */
typedef struct {
	FILE * source;			/* ponteiro para arquivo */
	int last_pos;			/* última posição */
	char last_read;			/* último caracter lido */
	int line_cur;			/* linha atual */
	int init_pos_line;		/* posição onde começa a linha atual */
} source_t;

/* Função para abrir o arquivo */
source_t * ts_open_source(char* source);

/* Função para fechar o arquivo */
void ts_close_source(char* source);

/* Função para pegar o próximo token */
token_t* ts_get_next_token(source_t* source, token_t* last_token, char* last_type);

/* Função para pegar o proximo caracter */
char ts_get_next_caracter(source_t * source);

/* Função para retornar o token quando o delimitador for encontrado */
token_t * ts_get_token_delimiter(source_t * source);

/* Verifica se é um token válido */
int is_token_valid(token_t* token, source_t* source);


#endif // !_TOKEN_STREAM
