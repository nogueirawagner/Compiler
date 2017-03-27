#ifndef __STACK_H__
#define __STACK_H__

typedef struct stack_node_t
{
	void* data;
	struct stack_node_t* next;
}stack_node_t;

typedef struct stack_t
{
	struct stack_node_t* head;
}stack_t;

int stack_init(struct stack_t* self);
int stack_empty(struct stack_t* self);
int stack_push(struct stack_t* self, void* data);
void* stack_pop(struct stack_t* self);

#endif
