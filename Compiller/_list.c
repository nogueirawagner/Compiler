/********************************************************************
		 https://github.com/LeamDelaney/linked_list
********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "_list.h"

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

	/* Loop to remove each element from the list */
	while (list_get_size(list) > 0)
	{
		if (list_remove_next(list, NULL, (void**)&data) == 0 &&
			list->destroy != NULL)
		{
			/* Call the function that the user defines for destroy */
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(linked_list_t));
	return;
}

int list_insert_next(linked_list_t* list, list_element_t* element, const void* data)
{
	list_element_t *newElement;

	/* Attempt to allocate the memory for the new element */
	if ((newElement = (list_element_t *)malloc(sizeof(list_element_t))) == NULL)
	{
		printf("list_insert_next : Falha ao inserir novo elemento \n");
		return -1;
	}
	newElement->data = (void*)data;

	if (element == NULL)
	{
		/* Add the new element at the HEAD of the list */
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

	/* Can't remove from an empty list */
	if (list_get_size(list) == 0)
	{
		printf("list_remove_next : N�o � poss�vel remover item de uma lista vazia \n");
		return -1;
	}

	/* Continue to remove element from list */
	if (element == NULL)
	{
		/* Remove element at head of list */
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
		/* Remove element from anywhere else in the list */
		if (element->next == NULL)
		{
			/*If there is no element after the current one, fail */
			printf("list_remove_next : Nao ha elementos depois do ultimo. \n");
			return -1;
		}
		*data = element->data;
		oldElement = element->next;
		element->next = element->next->next;

		if (element->next == NULL)
		{
			/* Setting new tail of list */
			list->tail = element;
		}
	}

	/* Free the storage which was allocated in the list for old element. */
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