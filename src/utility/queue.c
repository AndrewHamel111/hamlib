#include "hamlib/lists/queue.h"

#include <string.h>

void Enqueue(queue* q, void* value, size_t sz)
{
	if (value == NULL || sz == 0)
		return;
	if (q->m_list == NULL)
	{
		InitializeQueue(q);
	}

	q->m_tail = Add(q->m_tail, value, sz);
	q->length++;
}

void* Dequeue(queue* q, size_t* sz_out)
{
	if (q == NULL)
		return NULL;
	if (q->m_list == NULL) 
		return NULL;
	if (q->length < 1)
		return NULL;
	if (!q->m_list->sz || (q->m_list->value == NULL))
		return NULL;

	void* v_ptr = malloc(q->m_list->sz); 
	memcpy(v_ptr, q->m_list->value, q->m_list->sz);
	if (sz_out != NULL) 
		(*sz_out) = q->m_list->sz;

	q->m_list = RemoveHead(&(q->m_list)); 
	q->length--;

	if (q->length == 0)
		InitializeQueue(q);

	return v_ptr; 
}

void InitializeQueue(queue *q)
{
	q->m_list = (node*)malloc(sizeof(node));
	q->m_list->next = q->m_list->prev = q->m_list->value = NULL;
	q->m_list->sz = 0;

	q->m_tail = q->m_list;

	q->length = 0;
}

void ClearQueue(queue *q)
{
	FreeList(q->m_list);
	q->m_list = q->m_tail = NULL;
	InitializeQueue(q);
}

int CanDequeue(queue *q)
{
	if (q == NULL || q->m_list == NULL || (q->length < 1) || !q->m_list->sz || (q->m_list->value == NULL))
		return 0;

	return 1;
}