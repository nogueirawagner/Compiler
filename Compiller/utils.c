
/* Verifica se � caracter A...Z ou a...z */
int is_alphanumeric(char value)
{
	return (value >= 65 && value <= 90) || (value >= 97 && value <= 122);
}

/* Verifica se � n�merico */
int is_numeric(char value)
{
	return (value >= 48 && value <= 57);
}

/* Verifica se � caracter e-comercial '&' */
int is_caracter_ampersand(char value)
{
	return (value == 38);
}

/* Verifica se � caracter ponto e v�rgula ';' */
int is_caracter_semicolon(char value)
{
	return (value == 59);
}

/* Verifica se � caracter v�rgula ',' */
int is_caracter_comma(char value)
{
	return (value == 44);
}

/* Verifica se � operador de igualdade '=' */
int is_caracter_equals(char value)
{
	return (value == 61);
}

/* Verifica se � espa�o em branco */
int is_space(char value)
{
	return (value == 32);
}

/* Verifica se � final de linha */
int is_new_line(char value)
{
	return (value == 13 || value == 10);
}

/* Verifica se � um caracter '\n' */
int is_caracter_smash_line(char value)
{
	return (value == 10);
}

/* Verifica se � abre chaves */
int is_caracter_key_opened(char value)
{
	return (value == 123);
}

/* Verificar se ponteiros s�o iguais */
int ts_are_equal(char* pointer1, char* pointer2)
{
	return !(strcmp(pointer1, pointer2));
}
