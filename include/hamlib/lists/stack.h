#ifndef STACK_H_
#define STACK_H_

#ifndef MAX_STACK
#define MAX_STACK 32
#endif

/**
 * \brief A stack data structure. The max can be set by defining MAX_STACK
 * 
 */
typedef struct Stack
{
	// private
	signed short _topindex;
	void** stack_array;

	// public
	unsigned char type_size;
	unsigned char type_pointer_size;
} Stack;

// Manangement
/** \brief Call this with sizeof(YOUR_TYPE) and sizeof(YOUR_TYPE*) to manually allocate memory to a stack. Must call StackFree at the end of the stack's life to free the memory. */
Stack* CreateStack(unsigned char type_size); 
/** \brief Clears the stack freeing all internal memory. Maintains the type_size */
void StackClear(Stack*);
 /** \brief Clears the stack freeing all internal memory, as well as the memory for the stack itself. */ 
void StackFree(Stack*);

// Add and Remove
/** \brief Returns the top element of the stack. */
void* StackTop(Stack*); 
/** \brief Pops the top element from the stack.*/
void StackPop(Stack*); 
 /** \brief Add an element to the top of the stack, returns the new stack size. */
unsigned char StackPush(Stack*, void*);

// Info
/** \brief Returns the size of the stack. The top index can be given by StackSize() - 1. */
unsigned char StackSize(Stack*); 
/** \brief Returns the size of the type stored in the stack. */
unsigned char StackTypeSize(Stack*); 

#endif