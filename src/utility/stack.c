#include "hamlib/stack.h"

#include <stdlib.h>
#include <string.h>

// Manangement
Stack* CreateStack(unsigned char type_size)
{
	Stack* newstack = (Stack*)malloc(sizeof(Stack));
	newstack->type_size = type_size;
	newstack->_topindex = -1;
	newstack->stack_array = malloc(sizeof(void*) * MAX_STACK);
	for(int i = 0; i < MAX_STACK; i++)
		newstack->stack_array[i] = NULL;
	return newstack;
}

void StackClear(Stack* stack)
{
	for(int i = 0; i < MAX_STACK; i++)
		free(stack->stack_array[i]);
	stack->_topindex = -1;
}

void StackFree(Stack* stack)
{
	StackClear(stack);
	free(stack->stack_array);
	free(stack);
}

// Add and Remove
void* StackTop(Stack* stack)
{
	if (StackSize(stack) == 0) return NULL;
	return stack->stack_array[stack->_topindex];
}

void StackPop(Stack* stack)
{
	if (StackSize(stack) == 0) return;

	free(stack->stack_array[stack->_topindex]);
	stack->_topindex--;
}

unsigned char StackPush(Stack* stack, void* value)
{
	if (stack->_topindex == MAX_STACK - 1) return MAX_STACK;

	stack->_topindex++;
	stack->stack_array[stack->_topindex] = malloc(stack->type_size);
	memcpy(stack->stack_array[stack->_topindex], value, stack->type_size);

	return stack->_topindex + 1;
}

// Info
unsigned char StackSize(Stack* stack)
{
	return stack->_topindex + 1;
}

unsigned char StackTypeSize(Stack* stack)
{
	return stack->type_size;
}
