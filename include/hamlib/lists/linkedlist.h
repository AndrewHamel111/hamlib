#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct node
{
	struct node* next;
	struct node* prev;
	void* value;
	int sz;
} node;

node* GetTail(node* head);
node* GetTailAndCount(node* head, unsigned int* length);

/**
 * \brief Set the values of the specified node, do nothing to the next/prev pointers. 
 * \details It is the caller's responsibility to make sure node is non-null.
 * \param node 
 * \param value 
 * \param sz 
 */
void Set(node* node, void* value, int sz);

/**
 * \brief or AddAfter, inserts the specified value after the pointer. Creates a new list and returns the head if tail == NULL.
 * 
 * \param tail 
 * \param value 
 * \param sz 
 */
node* Add(node* tail, void* value, int sz);

/**
 * \brief Add an element before the current element. If placing something at the front is desired, use AddBeforeHead
 * 
 * \param tail 
 * \param value 
 * \param sz 
 */
node* AddBefore(node* tail, void* value, int sz);
node* AddBeforeHead(node** head, void* value, int sz);

/**
 * \brief WARNING, will traverse the entire list. For large lists this is EXTREMELY bad. Instead, keep track of the tail throughout all the add operations.
 * 
 * \param head 
 * \param value 
 * \param sz 
 * \return node* 
 */
node* AddToTail(node* head, void* value, int sz);

/**
 * \brief Moves src node to immediately after dest.
 * \param src 
 * \param dest 
 */
void MoveNode(node* src, node* dest);

/**
 * \brief Remove the specified node.
 * \param _node 
 * \return node* to the new node at _node's index. If removing a tail it returns the new tail.
 */
node* Remove(node* _node);

/**
 * \brief Remove the head of the list and advance the head to the next element.
 * \param head 
 * \return the new head
 */
node* RemoveHead(node** head);

/**
 * \brief Remove the node AFTER the specified node. 
 * \param head 
 * \return node* to the new node at head->next's index.
 */
node* RemoveAfter(node* head);

void FreeNode(node* node);
void FreeList(node* head);

#endif