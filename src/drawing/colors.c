/**
 * \file colors.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"
#include <stdlib.h>

Color randomcolor()
{
	return (Color){rand() % 256,rand() % 256,rand() % 256,255};
}

Color tintcolor(Color color, float amount)
{
	if (amount > 1.0f || amount < 0.0f)
		amount = 0.5f;

	unsigned char max = MAX(MAX(color.r,color.g),color.b);
	bool getsDarker = max > 160;

	if (amount == 1.0f) return getsDarker ? BLACK : WHITE;

	if (color.r == max)
	{
		unsigned char del = LERP(color.r, getsDarker ? 0 : 255, amount);
		color.g = CLAMP(del - (color.r - color.g), 0, 255);
		color.b = CLAMP(del - (color.r - color.b), 0, 255);
		return (Color){del, color.g, color.b, 255};
	}
	else if (color.g == max)
	{
		unsigned char del = LERP(color.g, getsDarker ? 0 : 255, amount);
		color.r = CLAMP(del - (color.g - color.r), 0, 255);
		color.b = CLAMP(del - (color.g - color.b), 0, 255);
		return (Color){color.r, del, color.b, 255};
	}
	else
	{
		unsigned char del = LERP(color.b, getsDarker ? 0 : 255, amount);
		color.r = CLAMP(del - (color.b - color.r), 0, 255);
		color.g = CLAMP(del - (color.b - color.g), 0, 255);
		return (Color){color.r, color.g, del, 255};
	}
}

Color fadecolor(Color color, float amount)
{
	amount = CLAMP(amount, 0, 1);
	return (Color){color.r, color.g, color.b, LERP(color.a, 0, amount)};
}

Color lerpcolor(Color c1, Color c2, float t)
{
	t = CLAMP(t, 0, 1);
	return (Color){LERP(c1.r, c2.r, t), LERP(c1.g, c2.g, t), LERP(c1.b, c2.b, t), LERP(c1.a, c2.a, t)};
}