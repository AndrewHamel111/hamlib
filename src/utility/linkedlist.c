#include "hamlib/linkedlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node* GetTail(node* head)
{
	if (head == NULL)
		return NULL;

	while(head->next != NULL)
	{
		head = head->next;
	}
	
	return head;
}

node* GetTailAndCount(node* head, unsigned int* length)
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

void Set(node* node, void* value, int sz)
{
	if (node == NULL)
		return;

	free(node->value);
	node->value = malloc(sz);
	memcpy(node->value, value, sz);
	node->sz = sz;
}

node* Add(node* tail, void* value, int sz)
{	
	if ((tail->value == NULL))
	{
		// Consider this the creation of a list
		printf("null tail is being converted to a list\n");
		tail->prev = tail->next = NULL;
		Set(tail, value, sz);
		printf("done\n");
		return tail;
	}
	else
	{
		printf("normal add operation\n");
		node* newnode = (node*)malloc(sizeof(node));
		// set pointers and value
		newnode->next = tail->next;
		newnode->prev = tail;
		Set(newnode, value, sz);

		// attach to list
		if (tail->next != NULL)
			tail->next->prev = newnode;
		tail->next = newnode;
		
		printf("done\n");
		return newnode;
	}
}

node* AddBefore(node* tail, void* value, int sz)
{
	// if we're trying to add "before" the head, just ignore this and put it after head.
	if (tail->prev == NULL)
		return Add(tail, value, sz);

	return Add(tail->prev, value, sz);
}

node* AddBeforeHead(node** head, void* value, int sz)
{
	if ((*head)->prev != NULL)
		return NULL;

	// alloc
	node* newhead = malloc(sizeof(node));

	// set pointers and value
	newhead->next = *head;
	newhead->prev = NULL;
	Set(newhead, value, sz);

	// attach to list
	(*head)->prev = newhead;
	(*head) = newhead;

	return (*head);
}

node* AddToTail(node* head, void* value, int sz)
{
	return Add(GetTail(head), value, sz);
}

void MoveNode(node* src, node* dest)
{
	Add(dest, src->value, src->sz);
	Remove(src);
}

node* Remove(node* _node)
{
	if (_node == NULL)
	{
		printf("node in remove is null.\n");
		return NULL;
	}

	printf("removing node\n");

	if (_node->prev == NULL)
	{
		printf("Tried to remove head using normal remove function.\n");
		return NULL;
	}
	else
	{
		_node->prev->next = _node->next;
		_node->next->prev = _node->prev;
		node* temp = _node->next;
		FreeNode(_node);
		printf("node freed\n");
		return temp;
	}
}

node* RemoveHead(node** head)
{
	node* temp = *head;
	*head = temp->next;
	FreeNode(temp);
	return (*head);
}

node* RemoveAfter(node* head)
{
	Remove(head->next);
	return head->next;
}

void FreeNode(node* node)
{
	free(node->value);
	free(node);
}

void FreeList(node* head)
{
	while(head != NULL)
	{
		node* temp = head;
		head = head->next;
		FreeNode(temp);
	}
}