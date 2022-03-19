#ifndef HAMLIB_LINKEDLIST_H
#define HAMLIB_LINKEDLIST_H

typedef struct node
{
	struct node* next;
	struct node* prev;
	void* value;
	int sz;
} node;

node* getTail(node* head);
node* getTailAndCount(node* head, unsigned int* length);

/**
 * \brief set the values of the specified n, do nothing to the next/prev pointers.
 * \details It is the caller's responsibility to make sure n is non-null.
 * \param n
 * \param value 
 * \param sz 
 */
void set(node* n, void* value, int sz);

/**
 * \brief or AddAfter, inserts the specified value after the pointer. Creates a new list and returns the head if tail == NULL.
 * 
 * \param tail 
 * \param value 
 * \param sz 
 */
node* add(node* tail, void* value, int sz);

/**
 * \brief add an element before the current element. If placing something at the front is desired, use addBeforeHead
 * 
 * \param tail 
 * \param value 
 * \param sz 
 */
node* addBefore(node* tail, void* value, int sz);
node* addBeforeHead(node** head, void* value, int sz);

/**
 * \brief WARNING, will traverse the entire list. For large lists this is EXTREMELY bad. Instead, keep track of the tail throughout all the add operations.
 * 
 * \param head 
 * \param value 
 * \param sz 
 * \return node* 
 */
node* addToTail(node* head, void* value, int sz);

/**
 * \brief Moves src node to immediately after dest.
 * \param src 
 * \param dest 
 */
void moveNode(node* src, node* dest);

/**
 * \brief removeNode the specified node.
 * \param n
 * \return node* to the new node at n's index. If removing a tail it returns the new tail.
 */
node* removeNode(node* n);

/**
 * \brief removeNode the head of the list and advance the head to the next element.
 * \param head 
 * \return the new head
 */
node* removeHead(node** head);

/**
 * \brief removeNode the node AFTER the specified node.
 * \param head 
 * \return node* to the new node at head->next's index.
 */
node* removeAfter(node* head);

void freeNode(node* n);
void freeList(node* head);

#endif