/**
 * \file drawing.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"

#include <string.h>
#include <stdlib.h>

void DrawMultilineTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment);

void DrawMultilineTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment)
{
	// TODO implement TA_MIDDLE case
	int len = TextLength(text);
	int lineCount = 0; // really 1 less line than total lines but that's okay because 0 lines means no offset

	int j = 0;
	while(text[j] != '\0')
		if (text[j++] == '\n')
			lineCount++;

	bool bott = (alignment & TA_BOTTOM);
	int currentLine = bott ? lineCount : 0;

	int leftendpoint = -1, rightendpoint = -1;
	char* tmp_text;
	for(int i = 0; i < len + 1; i++)
	{
		if (text[i] == '\0' || text[i] == '\n')
		{
			leftendpoint = rightendpoint;
			rightendpoint = i;

			tmp_text = (char*)malloc(sizeof(char) * (rightendpoint - leftendpoint));
			strncpy(tmp_text, text + leftendpoint + 1, rightendpoint - leftendpoint - 1);
			tmp_text[rightendpoint - leftendpoint - 1] = '\0';

			DrawTextAligned(tmp_text, posX, posY + (fontSize * currentLine * (bott ? -1 : 1)), fontSize, color, alignment);
			currentLine += (bott) ? -1 : 1;

			free(tmp_text);

			if (text[i] == '\0')
				break;
		}
	}
}

void DrawTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment)
{
	int len = TextLength(text);

	// Find out if we need to use the multiline logic here
	bool isMultiLine = false;
	char* c;
	c = text;
	while(c < (text + len) && !isMultiLine)
		if (*c == '\n')
			isMultiLine = true;
		else
			c++;

	if (isMultiLine)
	{
		DrawMultilineTextAligned(text, posX, posY, fontSize, color, alignment);
		return;
	}

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

void DrawTextAlignedPro(const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint, TEXT_ALIGNMENT alignment)
{
	// no cases for TA_LEFT and TA_TOP as this is default behaviour in raylib.
	int x = position.x, y = position.y;

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

	//origin = VecAdd(origin, VecDiff(position, (Vector2){x,y}));

	DrawTextPro(GetFontDefault(), text, (Vector2){x,y}, origin, rotation, fontSize, spacing, tint);
}

void DrawTextureScaled(Texture2D texture, Vector2 pos, Vector2 scaling, Color color)
{
	Rectangle src = (Rectangle){0, 0, texture.width, texture.height};
	Rectangle dst = (Rectangle){pos.x + ((1.0f - scaling.x) * texture.width * 0.5f), pos.y + ((1.0f - scaling.y) * texture.height * 0.5f), scaling.x * texture.width, scaling.y * texture.height};
	DrawTexturePro(texture, src, dst, (Vector2){0, 0}, 0, color);
}