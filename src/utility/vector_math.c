/**
 * \file vector_math.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"

Vector2 getRectCenter(Rectangle rect)
{
	return (Vector2){rect.x + rect.width/2, rect.y + rect.height/2};
}

Vector2 getRectCenterRelative(Rectangle rect)
{
	return (Vector2){(int)(rect.width/2), (int)(rect.height/2)};
}

// RECTANGLE OTHER OPERATIONS //

Rectangle pureSource(Texture2D texture)
{
	return (Rectangle){0, 0, texture.width, texture.height};
}

Rectangle recFromVec(Vector2 position, Vector2 size)
{
	return (Rectangle){position.x, position.y, size.x, size.y};
}