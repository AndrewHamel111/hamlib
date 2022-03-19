#include "hamlib.h"

#include <stdlib.h>
#include <math.h>

float signf(float f)
{
	return (f < 0) ? -1.0f : 1.0f;
}

int sign(int i)
{
	return (i < 0) ? -1 : 1;
}

int randsign(void)
{
	return (rand() % 2) ? 1 : -1;
}

float randsignf(void)
{
	return (rand() % 2) ? 1.0f : -1.0f;
}

void reducebyf(float* f, float r)
{
	if (*f == 0) return;

	if (fabsf(*f) < fabsf(r))
		*f = 0;
	else
		*f -= r * signf(*f);
}

void reducebyi(int* i, int r)
{
	if (*i == 0) return;

	if (abs(*i) < abs(r))
		*i = 0;
	else
		*i -= r * sign(*i);
}

void increasebyf(float* f, float r)
{
	*f += fabsf(r) * signf(*f);
}

void increasebyi(int* i, int r)
{
	*i += abs(r) * sign(*i);
}