#ifndef _TOKEN_STREAM
#define _TOKEN_STREAM

#include <stdio.h>

// Enumerado de tokens 
typedef enum {
	TK_ID,			/* define o identificador do token */
	TK_TYPE,		/* define se o token é um tipo de dado (int, char, long, dec) */
	TK_EQUAL,		/* define se o token é operador aritmetico de igualdade */
	TK_CONST,		/* define se o token é um valor atribuido */
	TK_STM_END,	/* define se o token é um ponto e vírgula */
	TK_INT,			/* define se o token é um tipo int */
	TK_CHAR,		/* define se o token é um tipo char */
	TK_DEC			/* define se o token é um tipo decimal */
} token_type_t;

// Estrutura para armazenar o Token
typedef struct {
	char* id;
	token_type_t type;
	int line;
} token_t;


// Tabela de símbolos armazenando os tokens
typedef struct {
	token_t * token;
	struct token_list_t * node;
} token_list_t;

// Estrutura que recebe o arquivo quando for lido
typedef struct {
	FILE* source;		/* ponteiro para arquivo */
	int last_pos;		/* última posição */
	char last_read;	/* último caracter lido */
	int line_cur;		/* linha atual */
} source_t;

/******************************************************
	Função para abrir o arquivo
*/
source_t* ts_open_source(char* source);
/******************************************************/

/******************************************************
	Função para fechar o arquivo
*/
void ts_close_source(char* source);
/******************************************************/

/******************************************************
	Função para pegar o próximo token
*/
token_t* ts_get_next_token(source_t* source);
/******************************************************/

/******************************************************
	Função para validar se é um tipo de token
*/
int ts_is_token_type(token_t *token, token_type_t type);
/******************************************************/

/******************************************************
	Função para inserir tokens na tabela de simbolos
*/
token_list_t* ts_save_tokens_table_symbols(token_list_t * token_next, token_t * token);
/******************************************************/

/******************************************************
	Função para pegar o proximo caracter
*/
char ts_get_next_caracter(source_t * source);
/******************************************************/

/******************************************************
	Função para retornar o token quando o delimitador for encontrado
*/
token_t * ts_get_token_delimiter(source_t * source);
/******************************************************/

#endif // !_TOKEN_STREAM
