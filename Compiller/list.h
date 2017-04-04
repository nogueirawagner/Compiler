#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list_element_t
{
    void* data;
    struct list_element_t* next;
}list_element_t;

typedef struct linked_list_t
{
    int	size;
    int (*match)(const void* key1, const void* key2);
    void (*destroy)(void* data);
    
	list_element_t* head;
    list_element_t* tail;
}linked_list_t;

void list_initialize(linked_list_t* list, void (*destroy)(void* data));

void list_destroy (linked_list_t* list);

int list_insert_next (linked_list_t* list, list_element_t* element, const void* data);

int list_remove_next (linked_list_t* list, list_element_t* list_element_t, void** data); 

int list_get_size(linked_list_t* list);

list_element_t* list_head(linked_list_t* list);

list_element_t* list_tail(linked_list_t* list);

int list_is_head (linked_list_t* list, list_element_t* element);

int list_is_tail (linked_list_t* list, list_element_t* element);

void* list_data(list_element_t* element);

list_element_t* list_next(list_element_t* element);

void list_update(linked_list_t* list);

int list_any(linked_list_t* list, list_element_t* position);

#endif
