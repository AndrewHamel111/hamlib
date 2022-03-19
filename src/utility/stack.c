#include "hamlib/lists/stack.h"

#include <stdlib.h>
#include <string.h>

// Manangement
stack* createStack(unsigned char typeSize)
{
	stack* newstack = (stack*)malloc(sizeof(stack));
	newstack->typeSize = typeSize;
	newstack->topindex = -1;
	newstack->stackArray = malloc(sizeof(void*) * MAX_STACK);
	for(int i = 0; i < MAX_STACK; i++)
		newstack->stackArray[i] = NULL;
	return newstack;
}

void stackClear(stack* s)
{
	for(int i = 0; i < MAX_STACK; i++)
		free(s->stackArray[i]);
	s->topindex = -1;
}

void stackFree(stack* s)
{
	stackClear(s);
	free(s->stackArray);
	free(s);
}

// add and removeNode
void* stackTop(stack* s)
{
	if (stackSize(s) == 0) return NULL;
	return s->stackArray[s->topindex];
}

void stackPop(stack* s)
{
	if (stackSize(s) == 0) return;

	free(s->stackArray[s->topindex]);
	s->topindex--;
}

unsigned char stackPush(stack* s, void* voidPtr)
{
	if (s->topindex == MAX_STACK - 1) return MAX_STACK;

	s->topindex++;
	s->stackArray[s->topindex] = malloc(s->typeSize);
	memcpy(s->stackArray[s->topindex], voidPtr, s->typeSize);

	return s->topindex + 1;
}

// Info
unsigned char stackSize(stack* s)
{
	return s->topindex + 1;
}

unsigned char stackTypeSize(stack* s)
{
	return s->typeSize;
}
