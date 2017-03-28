#ifndef __UTILS_H__
#define __UTILS_H__

/* Verifica se � caracter A...Z ou a...z */
int is_alphanumeric(char value);

/* Verifica se � n�merico */
int is_numeric(char value);

/* Verifica se � caracter e-comercial '&' */
int is_caracter_ampersand(char value);

/* Verifica se � caracter ponto e v�rgula ';' */
int is_caracter_semicolon(char value);

/* Verifica se � caracter v�rgula ',' */
int is_caracter_comma(char value);

/* Verifica se � operador de igualdade '=' */
int is_caracter_equals(char value);

/* Verifica se � espa�o em branco */
int is_space(char value);

/* Verifica se � final de linha */
int is_new_line(char value);

/* Verifica se � um caracter '\n' */
int is_caracter_smash_line(char value);

/* Verifica se � abre chaves */
int is_caracter_key_opened(char value);

/* Verificar se ponteiros s�o iguais */
int ts_are_equal(char* pointer1, char* pointer2);

/* Define se o token � um tipo de dado */
int is_token_type_data(char* value);

/* Define se o token � uma vari�vel */
int is_token_variable(char* value);

#endif
