#include "token_stream.h"

void throw_alert(int code, source_t * source) 
{
	switch (code)
	{
		case 1001:
			alert_(source);
	}
}