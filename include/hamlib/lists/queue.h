#ifndef QUEUE_H_
#define QUEUE_H_

#include "hamlib/lists/linkedlist.h"
#include <stdlib.h>

/**
 * \brief All Queues should be declared with a zero inializer || queue q = {0}; || to guarantee all add and remove operations work.
 */
typedef struct queue 
{
	node* m_list;
	node* m_tail;

	short length;
} queue;

/**
 * \brief Add something to the queue. If a queue is uninitialized, it will also initialize it. I should be wrapped by the implementer to better serve a specific data type.
 * \param value Address of value to enqueue.
 * \param sz Number of bytes to queue (use sizeof())
 */
void Enqueue(queue* q, void* value, size_t sz);
/**
 * \brief Dequeue the next value. Will return NULL if cannot dequeue. CanDequeue can be used to guard from NULL. I should be wrapped by the implementer to better serve a specific data type.
 * \return void* To heap data, do not directly cast and dereference, store to be freed after being copied to stack data.
 */
void* Dequeue(queue* q, size_t* sz_out);
/**
 * \brief Allocates memory for the queue, sets the length to be 0. Should only be used internally (Enqueue automatically does this)
 */
void InitializeQueue(queue *q);
/**
 * \brief Dequeue's all data, discarding and freeing it.
 */
void ClearQueue(queue *q);
/**
 * \brief 
 * \return true When Dequeue is ready
 * \return false If: queue is null, queue internal list is null, nothing left to dequeue, element at head is invalid (invalid data or invalid size) If this happens, clear the queue.
 */
int CanDequeue(queue *q);

#endif