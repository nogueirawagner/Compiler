#include "token_stream.h";
#include <Windows.h>

int is_alphanumeric(char value) 
{
	return (value >= 65 && value <= 90) || (value >= 97 && value <= 122) || (value == 95);
}

int is_new_line(char value) {
	return (value == '\n');
}

token_type_t ts_get_type(char* value) 
{
	return TK_ID;
}

source_t* ts_open_source(char* source)
{
	source_t* psource = (source_t*)malloc(sizeof(source_t));
	psource->source = fopen(source, "rb");
	psource->last_pos = 0;

	return psource;
}

void ts_close_source(char* source) 
{
	fclose(source);
}

token_t* ts_get_next_token(source_t* source)
{
	char buffer[255];
	FillMemory(&buffer, 255, 0);
	
	int line = 1;
	token_t token;
	while (1)
	{
		char value = getc(source->source);

		if (is_alphanumeric(value)) 
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = getc(source->source);			
				if (value == ' ') 
				{
					token.id = buffer;
					token.line = line;
					token.type = ts_get_type(token.id);
					return &token;
				}
			}
		}
		else if (value == '&') 
		{
			while (1)
			{
				char scopy[1] = { value };
				strncat(buffer, scopy, 1);

				value = getc(source->source);
				if (value == ' ')
				{
					token.id = buffer;
					token.line = line;
					token.type = ts_get_type(token.id);

					// INSERIR NA TABELA

					return &token;
				}
			}
		}
		else if (value == '=') 
		{
			token.id = "=";
			token.line = line;
			token.type = TK_EQUAL;
			return &token;
		}
		else if (is_new_line(value))
		{
			line++;
		}

		source->last_pos += 1;
	};
	
	return NULL;
}

int ts_is_token_type(token_t *token, token_type_t type)
{
	return (token->type == type);
}