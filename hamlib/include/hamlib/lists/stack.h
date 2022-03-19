#ifndef HAMLIB_STACK_H
#define HAMLIB_STACK_H

#ifndef MAX_STACK
#define MAX_STACK 32
#endif

/**
 * \brief A stack data structure. The max can be set by defining MAX_STACK
 * 
 */
typedef struct stack
{
	// private
	signed short topindex;
	void** stackArray;

	// public
	unsigned char typeSize;
	unsigned char typePointerSize;
} stack;

// Manangement
/** \brief Call this with sizeof(YOUR_TYPE) and sizeof(YOUR_TYPE*) to manually allocate memory to a stack. Must call stackFree at the end of the stack's life to free the memory. */
stack* createStack(unsigned char typeSize);
/** \brief Clears the stack freeing all internal memory. Maintains the typeSize */
void stackClear(stack* s);
 /** \brief Clears the stack freeing all internal memory, as well as the memory for the stack itself. */ 
void stackFree(stack* s);

// add and removeNode
/** \brief Returns the top element of the stack. */
void* stackTop(stack* s);
/** \brief Pops the top element from the stack.*/
void stackPop(stack* s);
 /** \brief add an element to the top of the stack, returns the new stack size. */
unsigned char stackPush(stack* s, void* voidPtr);

// Info
/** \brief Returns the size of the stack. The top index can be given by stackSize() - 1. */
unsigned char stackSize(stack* s);
/** \brief Returns the size of the type stored in the stack. */
unsigned char stackTypeSize(stack* s);

#endif