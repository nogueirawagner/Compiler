#include "list_symbols.h"
#include <stdlib.h>
#include <stdio.h>


void init_list(symb_node_t* symbol_list)
{
	symbol_list->size = 0;
	symbol_list->next = NULL;
	symbol_list->data = NULL;
	
	return;
}

void add_list(symb_node_t* symbol_list, table_symbols_t* tbs)
{
	symb_node_t * head = NULL;
	head = malloc(sizeof(symb_node_t));
	if (head == NULL) {
		head->data = tbs;
		head->next = NULL;
		head->size++;
	}
	else 
	{
		head->next = malloc(sizeof(symb_node_t));
		head->next->data = tbs;
		head->next->next = NULL;
		head->size++;
	}
}

/* Iterating over a list */
void print_list(symb_node_t * head) {
	symb_node_t * current = head;

	while (current != NULL) {
		printf("%d\n", current->data); /* Ajustar */
		current = current->next;
	}
}

/* Adding an item to the end of the list */
void push(symb_node_t * head, table_symbols_t* tbs) 
{
	symb_node_t * current = (symb_node_t*)malloc(sizeof(symb_node_t));
	//while (current != NULL) {
	//	current = current->next;
	//}

	current->next = malloc(sizeof(symb_node_t));
	current->next->data = tbs;
	current->next->next = NULL;
	current->size++;
}

/* Adding an item to the beginning of the list (pushing to the list) */
void push_begin(symb_node_t ** head, table_symbols_t* tbs) {
	symb_node_t * new_node;
	new_node = malloc(sizeof(symb_node_t));

	new_node->data = tbs;
	new_node->next = *head;
	*head = new_node;
}

/* Removing the first item (popping from the list) */
table_symbols_t* pop(symb_node_t ** head) {
	table_symbols_t* retval;
	symb_node_t * next_node = NULL;

	if (*head == NULL) {
		return;
	}

	next_node = (*head)->next;
	retval = (*head)->data;
	free(*head);
	*head = next_node;

	return retval;
}

/* Removing the last item of the list */
table_symbols_t* remove_last(symb_node_t * head) {
	table_symbols_t* retval;
	if (head->next == NULL) {
		retval = head->data;
		free(head);
		return retval;
	}

	symb_node_t * current = head;
	while (current->next->next != NULL) {
		current = current->next;
	}

	retval = current->next->data;
	free(current->next);
	current->next = NULL;
	return retval;

}

/* Removing a specific item */
table_symbols_t* remove_by_index(symb_node_t ** head, int n) {
	int i = 0;
	table_symbols_t* retval;
	symb_node_t * current = *head;
	symb_node_t * temp_node = NULL;

	if (n == 0) {
		return pop(head);
	}

	for (int i = 0; i < n - 1; i++) {
		if (current->next == NULL) {
			return;
		}
		current = current->next;
	}

	temp_node = current->next;
	retval = temp_node->data;
	current->next = temp_node->next;
	free(temp_node);

	return retval;
}