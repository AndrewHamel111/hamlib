/**
 * \file arrays.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void swap(void* a, void* b, int sizeOf)
{
	void* temp = (void*)malloc(sizeOf);
	memcpy(temp, a, sizeOf);
	memcpy(a, b, sizeOf);
	memcpy(b, temp, sizeOf);
	free(temp);
}

void shuffle(void* array, int count, int sizeOf)
{
	for(int i = 0; i < count; i++)
	{
		int index = i * sizeOf;
		int j = i + (rand() % (count - i));
		int jndex = j * sizeOf;

		swap(array + index, array + jndex, sizeOf);
	}
}

void shufflerange(void* array, int count, int sizeOf, int start, int end)
{
	start = CLAMP(start, 0, count - 1);
	end = CLAMP(end, 0, count - 1);

	shuffle(array + start, end - start + 1, sizeOf);
}

void setintvalues(int* array, int count, ...)
{
	va_list list;
	va_start(list, count);

	for(int i = 0; i < count; i++)
	{
		array[i] = va_arg(list, int);
	}

	va_end(list);
}

void setcharvalues(unsigned char* array, unsigned char count, ...)
{
	va_list list;
	va_start(list, count);

	for(unsigned char i = 0; i < count; i++)
	{
		array[i] = (unsigned char)(va_arg(list, int));
	}

	va_end(list);
}

void setfloatvalues(float* array, int count, ...)
{
	va_list list;
	va_start(list, count);

	for(int i = 0; i < count; i++)
	{
		array[i] = (float)(va_arg(list, double));
	}

	va_end(list);
}