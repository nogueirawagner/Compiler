#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "token_stream.h"
#include "utils.h"

void list_initialize(linked_list_t *list, void(*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

void list_destroy(linked_list_t *list)
{
	void * data;

	while (list_get_size(list) > 0)
	{
		if (list_remove_next(list, NULL, (void**)&data) == 0 &&
			list->destroy != NULL)
		{
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(linked_list_t));
	return;
}

int list_insert_next(linked_list_t* list, list_element_t* element, const void* data)
{
	list_element_t *newElement;

	if ((newElement = (list_element_t *)malloc(sizeof(list_element_t))) == NULL)
	{
		printf("list_insert_next : Falha ao inserir novo elemento \n");
		return -1;
	}
	newElement->data = (void*)data;

	if (element == NULL)
	{
		if (list_get_size(list) == 0)
			list->tail = newElement;

		newElement->next = list->head;
		list->head = newElement;
	}
	else
	{
		if (element->next == NULL)
			list->tail = newElement;

		newElement->next = element->next;
		element->next = newElement;
	}

	list->size++;
	return 0;
}

int list_remove_next(linked_list_t *list, list_element_t *element, void **data)
{
	list_element_t* oldElement;

	if (list_get_size(list) == 0)
	{
		printf("list_remove_next : Não é possível remover item de uma lista vazia \n");
		return -1;
	}

	if (element == NULL)
	{
		*data = list->head->data;
		oldElement = list->head;
		list->head = list->head->next;

		if (list_get_size(list) == 1)
		{
			list->tail = NULL;
		}
	}
	else
	{
		if (element->next == NULL)
		{
			printf("list_remove_next : Nao ha elementos depois do ultimo. \n");
			return -1;
		}
		*data = element->data;
		oldElement = element->next;
		element->next = element->next->next;

		if (element->next == NULL)
		{
			list->tail = element;
		}
	}

	free(oldElement);

	list->size--;
	return 0;
}

int list_get_size(linked_list_t* list)
{
	return (int)list->size;
}

list_element_t* list_head(linked_list_t* list)
{
	return list->head;
}

list_element_t* list_tail(linked_list_t* list)
{
	return list->tail;
}

int list_is_head(linked_list_t* list, list_element_t* element)
{
	if (element == list->head)
		return 1;
	else
		return 0;
}

int list_is_tail(linked_list_t* list, list_element_t* element)
{
	if (element == list->tail)
		return 1;
	else
		return 0;
}

void* list_data(list_element_t* element)
{
	return element->data;
}

list_element_t* list_next(list_element_t* element)
{
	return element->next;
}
void list_update_tbl_symb(linked_list_t* list, list_element_t* position, char* id, char* value)
{
	for (int i = 0; i < list->size; i++)
	{
		table_symbols_t* object = (table_symbols_t*)position->data;
		char* variable = (char*)object->variable;
		if (ts_are_equal(variable, id))
		{
			object->value = value;
		}
		position = list_next(position);
	}
	return 0;
}

int list_any_tbl_symb(linked_list_t* list, list_element_t* position, char* id, char* type)
{
	for (int i = 0; i < list->size; i++)
	{
		table_symbols_t* object = (table_symbols_t*)position->data;
		char* variable = (char*)object->variable;
		char* tipo = (char*)object->type;
		if (!type)
		{
			if (ts_are_equal(variable, id))
				return 1;
		}
		else if (ts_are_equal(variable, id) && ts_are_equal(tipo, type))
			return 1;
		position = list_next(position);
	}
	return 0;
}

table_symbols_t* list_get_tbl_symb(linked_list_t* list, list_element_t* position, char* id, char* type)
{
	for (int i = 0; i < list->size; i++)
	{
		table_symbols_t* object = (table_symbols_t*)position->data;
		char* variable = (char*)object->variable;
		char* tipo = (char*)object->type;
		if (!type)
		{
			if (ts_are_equal(variable, id))
				return object;
		}
		else if (ts_are_equal(variable, id) && ts_are_equal(tipo, type))
			return object;
		position = list_next(position);
	}
	return NULL;
}