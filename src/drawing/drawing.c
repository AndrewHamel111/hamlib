/**
 * \file drawing.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

// TODO: fix const discarding by, instead of accessing text directly, increment c from 0 and add it to text each iteration.

#include "hamlib.h"
#include "hamlib/draw.h"

#include <string.h>
#include <stdlib.h>

void DrawMultilineTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment);

void DrawMultilineTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment)
{
	// TODO consider improving the temp solution I have wherein I compute all the line positions FIRST, then just iterate over them.

	// TODO implement TA_MIDDLE case
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

	bool bott = (alignment & TA_BOTTOM);
	bool mid = (alignment & TA_MIDDLE);
	int currentLine = bott ? lineCount : 0;

	int leftendpoint = -1, rightendpoint = -1;
	char* tmp_text;
	for(int i = 0; i < len + 1; i++)
	{
		if (text[i] == '\0' || text[i] == '\n')
		{
			leftendpoint = rightendpoint;
			rightendpoint = i;

			// find the substring of the current line
			tmp_text = (char*)malloc(sizeof(char) * (rightendpoint - leftendpoint));
			strncpy(tmp_text, text + leftendpoint + 1, rightendpoint - leftendpoint - 1);
			tmp_text[rightendpoint - leftendpoint - 1] = '\0';

			// posY calculation
			int _posY;
			_posY = posY + (fontSize * currentLine * (bott ? -1 : 1));
			if (mid)
			{
				_posY -= fontSize * ((lineCount) / 2); // adjustment for the number of lines
				if (even)
					_posY -= (fontSize) / 2; // adjustment since an even number of lines should actually be offset by half a line
			}

			DrawTextAligned(tmp_text, posX, _posY, fontSize, color, alignment);
			linePositions[currentLine] = _posY; // only used for debugging
			currentLine += (bott) ? -1 : 1;

			free(tmp_text);

			if (text[i] == '\0')
				break;
		}
	}

	free(linePositions);
}

void DrawTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment)
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

void DrawRectangleCentered(int posX, int posY, int width, int height, Color color)
{
	DrawRectangleCenteredRec((Rectangle){posX, posY, width, height}, color);
}

void DrawRectangleCenteredV(Vector2 position, Vector2 size, Color color)
{
	DrawRectangleCenteredRec(RecFromVec(position, size), color);
}

void DrawRectangleCenteredRec(Rectangle rectangle, Color color)
{
	DrawRectangleRec(CenterRect(rectangle), color);
}