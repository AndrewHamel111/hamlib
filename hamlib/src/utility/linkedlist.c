#include "hamlib/lists/linkedlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node* getTail(node* head)
{
	if (head == NULL)
		return NULL;

	while(head->next != NULL)
	{
		head = head->next;
	}
	
	return head;
}

node* getTailAndCount(node* head, unsigned int* length)
{
	if (head == NULL)
	{
		*length = 0;
		return NULL;
	}

	unsigned int len = 1;

	while(head->next != NULL)
	{
		head = head->next;
		len++;
	}
	
	*length = len;
	return head;
}

void set(node* n, void* value, int sz)
{
	if (n == NULL)
		return;

	free(n->value);
	n->value = malloc(sz);
	memcpy(n->value, value, sz);
	n->sz = sz;
}

node* add(node* tail, void* value, int sz)
{	
	if (tail->value == NULL)
	{
		// Consider this the creation of a list
		//printf("null tail is being converted to a list\n");
		tail->prev = NULL;
		tail->next = NULL;
		set(tail, value, sz);
		//printf("done\n");
		return tail;
	}
	else
	{
		//printf("normal add operation\n");
		node* newnode = (node*)malloc(sizeof(node));
		// set pointers and value
		newnode->next = tail->next;
		newnode->prev = tail;
		set(newnode, value, sz);

		// attach to list
		if (tail->next != NULL)
			tail->next->prev = newnode;
		tail->next = newnode;
		
		//printf("done\n");
		return newnode;
	}
}

node* addBefore(node* tail, void* value, int sz)
{
	// if we're trying to add "before" the head, just ignore this and put it after head.
	if (tail->prev == NULL)
		return add(tail, value, sz);

	return add(tail->prev, value, sz);
}

node* addBeforeHead(node** head, void* value, int sz)
{
	if ((*head)->prev != NULL)
		return NULL;

	// alloc
	node* newhead = malloc(sizeof(node));

	// set pointers and value
	newhead->next = *head;
	newhead->prev = NULL;
	set(newhead, value, sz);

	// attach to list
	(*head)->prev = newhead;
	(*head) = newhead;

	return (*head);
}

node* addToTail(node* head, void* value, int sz)
{
	return add(getTail(head), value, sz);
}

void moveNode(node* src, node* dest)
{
	add(dest, src->value, src->sz);
	removeNode(src);
}

node* removeNode(node* n)
{
	if (n == NULL)
	{
		//printf("node in remove is null.\n");
		return NULL;
	}

	//printf("removing node\n");

	if (n->prev == NULL)
	{
		//printf("Tried to remove head using normal remove function.\n");
		return NULL;
	}
	else if (n->next == NULL)
	{
		// trying to remove a tail
		node* tail = n->prev;
		tail->next = NULL;
		freeNode(n);
		// return new tail
		return tail;
	}
	else
	{
		n->prev->next = n->next;
		n->next->prev = n->prev;
		node* temp = n->next;
		freeNode(n);
		//printf("node freed\n");
		return temp;
	}
}

node* removeHead(node** head)
{
	node* temp = *head;
	if (temp->next != NULL) // if there's more after the head
		*head = temp->next;
	else // if it's the last element leave it in an "empty list" state
	{
		*head = (node*)malloc(sizeof(node));
		(*head)->value = (*head)->next = (*head)->prev = NULL;
		(*head)->sz = 0;
	}
	freeNode(temp);
	return (*head);
}

node* removeAfter(node* head)
{
	removeNode(head->next);
	return head->next;
}

void freeNode(node* n)
{
	free(n->value);
	free(n);
}

void freeList(node* head)
{
	while(head != NULL)
	{
		node* temp = head;
		head = head->next;
		freeNode(temp);
	}
}