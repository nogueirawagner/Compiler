#include "stack.h"
#include<stdlib.h>

int stack_init(struct stack_t* self)
{
	if (self == NULL)
	{
		return -1;
	}

	self->head = NULL;
	return 0;
}

int stack_empty(struct stack_t* self)
{
	return self->head == NULL;
}

int stack_push(struct stack_t* self, void *data)
{
	struct stack_node_t* newnode = malloc(sizeof(struct stack_node_t));
	if (newnode == NULL)
		return -1;
	else
	{
		newnode->data = data;
		newnode->next = self->head;
		self->head = newnode;
		return 0;
	}

}

void* stack_pop(struct stack_t* self)
{
	if (self->head == NULL)
		return NULL;
	void* data = self->head->data;
	struct stack_node_t* next = self->head->next;
	free(self->head);
	self->head = next;

	return data;
}
