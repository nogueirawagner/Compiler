#ifndef __UTILS_H__
#define __UTILS_H__

/* Verifica se é caracter A...Z ou a...z */
int is_alphanumeric(char value);

/* Verifica se é númerico */
int is_numeric(char value);

/* Verifica se é caracter e-comercial '&' */
int is_caracter_ampersand(char value);

/* Verifica se é caracter ponto e vírgula ';' */
int is_caracter_semicolon(char value);

/* Verifica se é caracter vírgula ',' */
int is_caracter_comma(char value);

/* Verifica se é operador de igualdade '=' */
int is_caracter_equals(char value);

/* Verifica se é espaço em branco */
int is_space(char value);

/* Verifica se é final de linha */
int is_new_line(char value);

/* Verifica se é um caracter '\n' */
int is_caracter_smash_line(char value);

/* Verifica se é abre chaves */
int is_caracter_key_opened(char value);

/* Verificar se ponteiros são iguais */
int ts_are_equal(char* pointer1, char* pointer2);

/* Define se o token é um tipo de dado */
int is_token_type_data(char* value);

/* Define se o token é uma variável */
int is_token_variable(char* value);

#endif
