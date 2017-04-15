#ifndef __UTILS_H__
#define __UTILS_H__

/* Operador Aritimeticos */
typedef enum {
	OP_MULT,	/* multiplicação */
	OP_ADIC,	/* adição */
	OP_SUBT,	/* subtração */
	OP_DIVI		/* divisão */
} operator_aritmetic;

/* Operador Relacionais */
typedef enum {
	OP_MORE_THEN,	/* maior que */
	OP_LESS_THEN,	/* menor que */
	OP_MORE_EQUAL,	/* maior ou igual */
	OP_LESS_EQUAL,	/* menor ou igual */
	OP_EQUAL,		/* igual */
	OP_DIFF			/* diferente */
} operator_relational;

/* Verifica se é caracter A...Z ou a...z */
int is_alphanumeric(char value);

/* Verifica se é caracter a...z */
int is_alphanumeric_tolower(char value);

/* Verifica se é caracter a...z */
int is_alphanumeric_toupper(char value);

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

/* Verifica se é caracter '{' */
int is_caracter_key_opened(char value);

/* Verifica se é caracter '}' */
int is_caracter_key_closed(char value);

/* Verifica se é caracter é letra 'm' */
int is_caracter_m(char value);

/* Verificar se ponteiros são iguais */
int ts_are_equal(char* pointer1, char* pointer2);

/* Define se o token é um tipo de dado */
int is_token_type_data(char* value, source_t* source);

/* Define se é palavra reservada para funcoes */
int is_token_function(char* value, source_t* source);

/* Define se o token é uma variável */
int is_token_variable(char* value);

/* Retorna o tamanho do conteúdo do ponteiro */
int length_content_token(char* value);

/* Verifica se é operadores relacional */
int is_caracter_relational(char value);

/* Verifica se é operador aritmetico */
int is_caracter_arimetic(char value);

/* Verifica se é aspas duplas ' "" '  */
int is_caracter_quotes_plus(char value);

/* Verificar se é caracter ' + '*/
int is_caracter_plus(char value);

/* Verifica se é caracter menos ' - ' */
int is_caracter_less(char value);

/* Verifica se é abre parenteses '(' */
int is_caracter_open_parathesi(char value);

/* Verifica se é fecha parenteses ')' */
int is_caracter_closed_parathesi(char value);

/* Verifica se é ponto '.' */
int is_caracter_point(char value);

/* Verifica se nao é tabulação */
int is_caracter_tab(char value);

/* Verifica tamanho da variavel Ex: dec &x(1.5) ou char &y(15) */
char* any_definition_length(char* value, source_t* source, int isDec);

/* Retorna somente variavel sem tamanho */
char* content_variable_id(char* value);

/* Verifica se tipo do token é uma função */
int is_token_type_function(token_type_t type);

/* Verifica se é caracter inválido para variaveis */
int is_caracter_invalid_var(char value);

#endif // !__UTILS_H__
