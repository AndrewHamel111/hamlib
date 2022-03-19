#include "hamlib/lists/queue.h"

#include <string.h>

void enqueue(queue* q, void* value, size_t sz)
{
	if (value == NULL || sz == 0)
		return;
	if (q->mList == NULL)
	{
		initializeQueue(q);
	}

	q->mTail = add(q->mTail, value, sz);
	q->length++;
}

void* dequeue(queue* q, size_t* szOut)
{
	if (q == NULL)
		return NULL;
	if (q->mList == NULL)
		return NULL;
	if (q->length < 1)
		return NULL;
	if (!q->mList->sz || (q->mList->value == NULL))
		return NULL;

	void* vPtr = malloc(q->mList->sz);
	memcpy(vPtr, q->mList->value, q->mList->sz);
	if (szOut != NULL)
		(*szOut) = q->mList->sz;

	q->mList = removeHead(&(q->mList));
	q->length--;

	if (q->length == 0)
		initializeQueue(q);

	return vPtr;
}

void initializeQueue(queue *q)
{
	q->mList = (node*)malloc(sizeof(node));
	q->mList->next = q->mList->prev = q->mList->value = NULL;
	q->mList->sz = 0;

	q->mTail = q->mList;

	q->length = 0;
}

void clearQueue(queue *q)
{
	freeList(q->mList);
	q->mList = q->mTail = NULL;
	initializeQueue(q);
}

int canDequeue(queue *q)
{
	if (q == NULL || q->mList == NULL || (q->length < 1) || !q->mList->sz || (q->mList->value == NULL))
		return 0;

	return 1;
}