#include "hamlib.h"

/**
 * \file hamlib.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \brief A small helper library developed to run along side Raylib.
 * \version 0.1
 * \date 2021-08-16
 * 
 * @copyright Copyright (c) 2021
 */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void DrawTextAligned(const char* text, int posX, int posY, int fontSize, Color color, enum TEXT_ALIGNMENT alignment)
{
	// no cases for TA_LEFT and TA_TOP as this is default behaviour in raylib.
	int x = posX, y = posY;

	if ((alignment & TA_CENTER) == TA_CENTER)
	{
		x -= MeasureText(text, fontSize)/2;
	}
	else if ((alignment & TA_RIGHT) == TA_RIGHT)
	{
		x -= MeasureText(text, fontSize);
	}

	if ((alignment & TA_MIDDLE) == TA_MIDDLE)
	{
		y -= fontSize/2;
	}
	else if ((alignment & TA_BOTTOM) == TA_BOTTOM)
	{
		y -= fontSize;
	}

	DrawText(text, x, y, fontSize, color);
}

bool VectorInRectangle(Vector2 v, Rectangle r)
{
	return (v.x < r.x + r.width) && (v.x > r.x) && (v.y < r.y + r.height) && (v.y > r.y);
}

float sqrDist(Vector2 a, Vector2 b)
{
	return sqr(a.x - b.x) + sqr(a.y - b.y);
}

float sqrDistEm(int x1, int y1, int x2, int y2)
{
	return sqr(x1 - x2) + sqr(y1 - y2);
}

Vector2 VecAdd(Vector2 a, Vector2 b)
{
	return (Vector2){a.x + b.x, a.y + b.y};
}

Vector2 VecMult(float a, Vector2 v)
{
	return (Vector2){a * v.x, a * v.y};
}

Vector2 VecDiff(Vector2 a, Vector2 b)
{
	return VecAdd(a, VecMult(-1, b));
}

bool VecEq(Vector2 a, Vector2 b)
{
	return a.x == b.x && a.y == b.y;
}

Vector2 GetCenter(Rectangle rect)
{
	return (Vector2){rect.x + rect.width/2, rect.y + rect.height/2};
}

////////////////
// UTITLITIES //
////////////////

void swap(void* a, void* b, int sizeOf)
{
	void* temp = (void*)malloc(sizeOf);
	memcpy(temp, a, sizeOf);
	memcpy(a, b, sizeOf);
	memcpy(b, temp, sizeOf);
	free(temp);
}

///////////
// COLOR //
///////////

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

/////////////
// BUTTONS //
/////////////

bool Button(Rectangle rect, Color color)
{
	DrawRectangleRec(rect, color);
	return VectorInRectangle(GetMousePosition(), rect);
}

bool ButtonTintable(Rectangle rect, Color color)
{
	bool flag = VectorInRectangle(GetMousePosition(), rect);
	DrawRectangleRec(rect, flag ? tintcolor(color, 0.6f) : color);
	return flag;
}

////////////
// ARRAYS //
////////////

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

/////////////
// DRAWING //
/////////////

void DrawTextureScaled(Texture2D texture, Vector2 pos, Vector2 scaling, Color color)
{
	Rectangle src = (Rectangle){0, 0, texture.width, texture.height};
	Rectangle dst = (Rectangle){pos.x + ((1.0f - scaling.x) * texture.width * 0.5f), pos.y + ((1.0f - scaling.y) * texture.height * 0.5f), scaling.x * texture.width, scaling.y * texture.height};
	DrawTexturePro(texture, src, dst, (Vector2){0, 0}, 0, color);
}

////////////////////////////
// RECTANGLES AND VECTORS //
////////////////////////////

Rectangle PureSource(Texture2D texture)
{
	return (Rectangle){0, 0, texture.width, texture.height};
}