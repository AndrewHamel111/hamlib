/**
 * \file drawing.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"
#include "hamlib/draw.h"

#include <string.h>
#include <stdlib.h>

static void drawMultilineTextAligned(const char* text, int posX, int posY, int fontSize, Color color, textAlignment alignment);

static void drawMultilineTextAligned(const char* text, int posX, int posY, int fontSize, Color color, textAlignment alignment)
{
	// TODO consider improving the temp solution I have wherein I compute all the line positions FIRST, then just iterate over them.

	// TODO implement TaMiddle case
	int len = TextLength(text);
	int lineCount = 0; // really 1 less line than total lines but that's okay because 0 lines means no offset

	int j = 0;
	while(text[j] != '\0')
		if (text[j++] == '\n')
			lineCount++;

	bool even = false;

	if (lineCount % 2 == 0)
	{
		// line count is ODD
		// variable is even but remember that actual line count is lineCount+1 therefore number of lines is ODD, so we use the full fontsize method
		even = false;
	}
	else
	{
		even = true;
	}

	// compute line positions only necessary for debugging
	float* linePositions = (float*)malloc(sizeof(float) * (lineCount + 1));

	bool bott = (alignment & TaBottom);
	bool mid = (alignment & TaMiddle);
	int currentLine = bott ? lineCount : 0;

	int leftendpoint, rightendpoint = -1;
	char* tmpText;
	for(int i = 0; i < len + 1; i++)
	{
		if (text[i] == '\0' || text[i] == '\n')
		{
			leftendpoint = rightendpoint;
			rightendpoint = i;

			// find the substring of the current line
			tmpText = (char*)malloc(sizeof(char) * (rightendpoint - leftendpoint));
			strncpy(tmpText, text + leftendpoint + 1, rightendpoint - leftendpoint - 1);
			tmpText[rightendpoint - leftendpoint - 1] = '\0';

			// posY calculation
			int destPosY;
			destPosY = posY + (fontSize * currentLine * (bott ? -1 : 1));
			if (mid)
			{
				destPosY -= fontSize * ((lineCount) / 2); // adjustment for the number of lines
				if (even)
					destPosY -= (fontSize) / 2; // adjustment since an even number of lines should actually be offset by half a line
			}
			
			drawTextAligned(tmpText, posX, destPosY, fontSize, color, alignment);
			linePositions[currentLine] = destPosY; // only used for debugging
			currentLine += (bott) ? -1 : 1;

			free(tmpText);

			if (text[i] == '\0')
				break;
		}
	}

	free(linePositions);
}

void drawTextAligned(const char* text, int posX, int posY, int fontSize, Color color, textAlignment alignment)
{
	int len = TextLength(text);

	// Find out if we need to use the multiline logic here
	bool isMultiLine = false;
	char c = 0;
	while((text + c) < (text + len) && !isMultiLine)
		if (*(text + c) == '\n')
			isMultiLine = true;
		else
			c++;

	if (isMultiLine)
	{
		drawMultilineTextAligned(text, posX, posY, fontSize, color, alignment);
		return;
	}

	// no cases for TaLeft and TaTop as this is default behaviour in raylib.
	int x = posX, y = posY;

	if ((alignment & TaCenter) == TaCenter)
	{
		x -= MeasureText(text, fontSize)/2;
	}
	else if ((alignment & TaRight) == TaRight)
	{
		x -= MeasureText(text, fontSize);
	}

	if ((alignment & TaMiddle) == TaMiddle)
	{
		y -= fontSize/2;
	}
	else if ((alignment & TaBottom) == TaBottom)
	{
		y -= fontSize;
	}

	DrawText(text, x, y, fontSize, color);
}

void drawTextAlignedPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint, textAlignment alignment)
{
	// no cases for TaLeft and TaTop as this is default behaviour in raylib.
	int x = position.x, y = position.y;

	Vector2 measure = MeasureTextEx(font, text, fontSize, 1.0f);
	if ((alignment & TaCenter) == TaCenter)
	{
		x -= measure.x/2;
	}
	else if ((alignment & TaRight) == TaRight)
	{
		x -= measure.x;
	}

	if ((alignment & TaMiddle) == TaMiddle)
	{
		y -= measure.y/2;
	}
	else if ((alignment & TaBottom) == TaBottom)
	{
		y -= measure.y;
	}

	Vector2 newPosition = {x, y};

	DrawTextPro(font, text, newPosition, origin, rotation, fontSize, spacing, tint);
}

void drawTextureScaled(Texture2D texture, Vector2 pos, Vector2 scaling, Color color)
{
	Rectangle src = (Rectangle){0, 0, texture.width, texture.height};
	Rectangle dst = (Rectangle){pos.x + ((1.0f - scaling.x) * texture.width * 0.5f), pos.y + ((1.0f - scaling.y) * texture.height * 0.5f), scaling.x * texture.width, scaling.y * texture.height};
	DrawTexturePro(texture, src, dst, (Vector2){0, 0}, 0, color);
}

void drawRectangleCentered(int posX, int posY, int width, int height, Color color)
{
	drawRectangleCenteredRec((Rectangle) {posX, posY, width, height}, color);
}

void drawRectangleCenteredV(Vector2 position, Vector2 size, Color color)
{
	drawRectangleCenteredRec(recFromVec(position, size), color);
}

void drawRectangleCenteredRec(Rectangle rectangle, Color color)
{
	DrawRectanglePro(rectangle, (Vector2){rectangle.width/2, rectangle.height/2}, 0.0f, color);
}