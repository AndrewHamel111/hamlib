#include "hamlib.h"

#include <stdlib.h>
#include <math.h>

int randsign(void)
{
	return (rand() % 2) ? 1 : -1;
}

float randsignf(void)
{
	return (float)randsign();
}

void reducebyf(float* f, float r)
{
	if (*f == 0) return;

	if (*f > 0)
	{
		if (*f > r)
			*f -= r;
		else
			*f = 0;
	}
	else
	{
		if (*f < ((-1.0f) * r))
			*f += r;
		else
			*f = 0;
	}
}

void reducebyi(int* f, int r)
{
	if (*f == 0) return;

	if (*f > 0)
	{
		if (*f > r)
			*f -= r;
		else
			*f = 0;
	}
	else
	{
		if (*f < ((-1) * r))
			*f += r;
		else
			*f = 0;
	}
}

void increasebyf(float* f, float r)
{
	if (*f > 0)
		*f += fabsf(r);
	else
		*f -= fabsf(r);
}

void increasebyi(int* f, int r)
{
	if (*f > 0)
		*f += abs(r);
	else
		*f -= abs(r);
}