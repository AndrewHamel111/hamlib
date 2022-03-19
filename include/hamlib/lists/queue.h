#ifndef HAMLIB_QUEUE_H
#define HAMLIB_QUEUE_H

#include "hamlib/lists/linkedlist.h"
#include <stdlib.h>

/**
 * \brief All Queues should be declared with a zero inializer || queue q = {0}; || to guarantee all add and remove operations work.
 */
typedef struct queue 
{
	node* mList;
	node* mTail;

	short length;
} queue;

/**
 * \brief add something to the queue. If a queue is uninitialized, it will also initialize it. I should be wrapped by the implementer to better serve a specific data type.
 * \param value Address of value to enqueue.
 * \param sz Number of bytes to queue (use sizeof())
 */
void enqueue(queue* q, void* value, size_t sz);
/**
 * \brief dequeue the next value. Will return NULL if cannot dequeue. canDequeue can be used to guard from NULL. I should be wrapped by the implementer to better serve a specific data type.
 * \return void* To heap data, do not directly cast and dereference, store to be freed after being copied to stack data.
 */
void* dequeue(queue* q, size_t* szOut);
/**
 * \brief Allocates memory for the queue, sets the length to be 0. Should only be used internally (enqueue automatically does this)
 */
void initializeQueue(queue *q);
/**
 * \brief dequeue's all data, discarding and freeing it.
 */
void clearQueue(queue *q);
/**
 * \brief 
 * \return true When dequeue is ready
 * \return false If: queue is null, queue internal list is null, nothing left to dequeue, element at head is invalid (invalid data or invalid size) If this happens, clear the queue.
 */
int canDequeue(queue *q);

#endif