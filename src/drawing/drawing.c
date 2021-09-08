/**
 * \file drawing.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"

void DrawTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment)
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

void DrawTextureScaled(Texture2D texture, Vector2 pos, Vector2 scaling, Color color)
{
	Rectangle src = (Rectangle){0, 0, texture.width, texture.height};
	Rectangle dst = (Rectangle){pos.x + ((1.0f - scaling.x) * texture.width * 0.5f), pos.y + ((1.0f - scaling.y) * texture.height * 0.5f), scaling.x * texture.width, scaling.y * texture.height};
	DrawTexturePro(texture, src, dst, (Vector2){0, 0}, 0, color);
}