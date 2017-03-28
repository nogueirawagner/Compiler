/**********************************************************
 * Header File : list.h
 * 
 * Purpose : Interface for a linked list in C
 * Author  : Leam Delaney
 * 
 * This code is not covered by any kind of warranty.
 * Please feel free to use and redistribute this code.
 *********************************************************/

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/* Struct for linked list elements. */
typedef struct list_element_t_
{
    void			*data;
    struct list_element_t_		*next;
}list_element_t;

/* Struct for link-list data structure */
typedef struct linked_list_t_
{
    int		size;
    int 	(*match)(const void *key1, const void *key2);
    void	(*destroy)(void *data);
    
    /* The two elements below define the two elements
     * which must be known for a standard linked list */
    list_element_t	*head;
    list_element_t *tail;
}linked_list_t;

/**********************************************************
 * Public interface for link list
 *********************************************************/

/* list_initialize
 * Return type : void 
 * 
 * Description : Initializes the linked list of type linked_list_t.
 *  All linked lists must be initialized before any other operations
 *  can be ran on them. The destroy argument is put here as a way to
 *  free allocated data when list_destroy is used.*/
void list_initialize(linked_list_t *list, void (*destroy)(void *data));
/* list_destroy
 * Return type : void
 * 
 * Description : Will destroy a list which is passed to it in the
 *  form of a pointer to a linked_list_t pointer. When a list has been
 *  destroyed, no more operations may be carried out unless the
 *  list_initialize function is called again. The list_destroy
 *  function removes all elements from the list, and calls the
 *  function passed in as destroy during the list_initialize, once
 *  for every element during the destroy process. Destroy will not
 *  be performed if the destroy function was passed in as NULL.
 *
 * Runtime : O(n)
 */
void list_destroy (linked_list_t *list);

/* list_insert_next
 * Return type : int - 0 = success,
 *                     1 = failure.
 * 
 * Description : Adds an element to the list (passed as "list")
 *  AFTER the element (passed as "element"). If "element" is NULL,
 *  then the new element will be added as the head of the list.
 *  The new element will contain a pointer to "data". This means the
 *  memory referenced by "data" needs to remain valid for the lifetime
 *  of the element in the list. It is the responsibility of the developer
 *  to manage this storage. 
 * 
 * Runtime : O(1)
 */ 
int list_insert_next (linked_list_t  *list,
                      list_element_t *element,
                      const void  *data);

/* list_remove_next
 * Return type : int - 0 = success,
 *                     1 = failure.
 *
 * Description : Removes the element form the list (passed as "list")
 *  AFTER the element (passed as "element"). If "element is NULL, then
 *  the element which represents the head of the list will be removed.
 *  When this function returns, "data" will still point to the data which
 *  was stored in the element which has been removed. It is the responsibility
 *  of the programmer to manage this storage. 
 *
 * Runtime : O(1)
 */
int list_remove_next (linked_list_t  *list,
                      list_element_t *list_element_t,
                      void	  **data); 

/* list_get_size
 * Return type : int - The number of elements in the linked list.
 *
 * Description : Gets the value of list->size and returns it.
 *
 * Runtime : O(1)
*/
int list_get_size(linked_list_t* list);

/* list_head
 * Return type : list_element_t* - Returns a pointer to the head element.
 * 
 * Runtime : O(1)
*/
list_element_t* list_head(linked_list_t* list);


/* list_tail
 * Return type : list_element_t* - Returns a pointer to the tail element.
 * 
 * Runtime : O(1)
*/
list_element_t* list_tail(linked_list_t* list);

/* list_is_head
 *
 * Return type : int 1 - Yes
 *                   0 - No
 * 
 * Runtime : O(1)
*/
int list_is_head (linked_list_t* list, list_element_t* element);

/* list_is_tail
 * Return type : int 1 - Yes
 *                   0 - No
 *
 * Runtime : O(1)
*/
int list_is_tail (linked_list_t* list, list_element_t* element);


/* list_data
 * Return type : The data within the element.
 *
 * Runtime : O(1)
*/
void* list_data(list_element_t* element);

/* list_next
 * Return type : The next node after the specified one
 * 
 * Runtime : O(1)
*/
list_element_t* list_next(list_element_t* element);
#endif
