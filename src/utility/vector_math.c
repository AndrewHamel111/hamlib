/**
 * \file vector_math.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"

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

Vector2 GetCenterRelative(Rectangle rect)
{
	return (Vector2){(int)(rect.width/2), (int)(rect.height/2)};
}

// RECTANGLE MATH //

Rectangle RecAdd(Rectangle a, Rectangle b)
{
	return (Rectangle){a.x + b.x, a.y + b.y, a.width, a.height};
}

Rectangle RecAddVec(Rectangle a, Vector2 v)
{
	return (Rectangle){a.x + v.x, a.y + v.y, a.width, a.height};
}

Rectangle RecDiff(Rectangle a, Rectangle b)
{
	return (Rectangle){a.x - b.x, a.y - b.y, a.width, a.height};
}

Rectangle RecScale(float a, Rectangle v)
{
	return (Rectangle){v.x, v.y, a * v.width, a * v.height};
}

Rectangle RecScaleCenter(float a, Rectangle v)
{
	float xd = (v.x * a) - v.x;
	float yd = (v.y * a) - v.y;

	return (Rectangle){v.x - xd/2, v.y = yd/2, a * v.width, a * v.height};
}

// RECTANGLE OTHER OPERATIONS //

Rectangle PureSource(Texture2D texture)
{
	return (Rectangle){0, 0, texture.width, texture.height};
}

Rectangle CenterRect(Rectangle rectangle)
{
	return (Rectangle){rectangle.x - rectangle.width/2, rectangle.y - rectangle.height/2, rectangle.width, rectangle.height};
}

Rectangle RecFromVec(Vector2 position, Vector2 size)
{
	return (Rectangle){position.x, position.y, size.x, size.y};
}