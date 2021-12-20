#include "hamlib/scaled_draw.h"

#include "raylib.h"
#include "hamlib.h"

#include <stdlib.h>
#include <stdarg.h>

static int WIDTH;
static int HEIGHT;
static int FLAGS;

static int TEMPLATE_WIDTH = 1280;
static int TEMPLATE_HEIGHT = 720;

void SetOriginalSize(int width, int height)
{
	TEMPLATE_WIDTH = width;
	TEMPLATE_HEIGHT = height;
}

void SetScalingSize(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
}

void SetScalingFlags(int flags)
{
	FLAGS = flags;
}

static bool SameRatio()
{
	return ((WIDTH / HEIGHT) == (TEMPLATE_WIDTH / TEMPLATE_HEIGHT));
}

/**
 * \brief Scale the value according to change in WIDTH without consideration of SF_MAINT_RATIO
 */
static int GetScaledWidth(int x)
{
	return (int)((float)WIDTH * ((float)x / (float)TEMPLATE_WIDTH));
}

/**
 * \brief Scale the value according to change in HEIGHT without consideration of SF_MAINT_RATIO
 */
static int GetScaledHeight(int y)
{
	return (int)((float)HEIGHT * ((float)y / (float)TEMPLATE_HEIGHT));
}

/**
 * \brief Scale the value according to change in WIDTH
 */
static int GetScaledX(int x)
{
	if (((FLAGS & SF_MAINT_RATIO) == SF_MAINT_RATIO) && !SameRatio())
	{
		
	}
	
	return GetScaledWidth(x);
}

/**
 * \brief Scale the value according to change in HEIGHT
 */
static int GetScaledY(int y)
{
	if (((FLAGS & SF_MAINT_RATIO) == SF_MAINT_RATIO) && !SameRatio())
	{

	}
	
	return GetScaledHeight(y);
}

void ScaledDrawRect(Rectangle rect, Color color)
{
	rect.x = (float)GetScaledX((int)rect.x);
	rect.y = (float)GetScaledY((int)rect.y);
	rect.width = (float)GetScaledX((int)rect.width);
	rect.height = (float)GetScaledY((int)rect.height);

	DrawRectangleRec(rect, color);
}

void ScaledDrawRectCentered(Rectangle rect, Color color)
{
	rect.x -= rect.width / 2;
	rect.y -= rect.height / 2;
	ScaledDrawRect(rect, color);
}

void ScaledDrawSprite(Texture2D sprite, Rectangle dest, float rotation, Color color)
{
	Rectangle src = PureSource(sprite);
	dest.x = (float)GetScaledX((int)dest.x);
	dest.y = (float)GetScaledY((int)dest.y);
	dest.width = (float)GetScaledX((int)dest.width);
	dest.height = (float)GetScaledY((int)dest.height);

	// This adjustment is needed since we want to be able to use rotation with respect to the center of the sprite
	Rectangle _dest = (Rectangle){GetCenter(dest).x, GetCenter(dest).y, dest.width, dest.height};

	DrawTexturePro(sprite, src, _dest, GetCenterRelative(dest), rotation, color);
}

void ScaledDrawSpriteCentered(Texture2D sprite, Rectangle dest, float rotation, Color color)
{
	dest.x -= sprite.width / 2;
	dest.y -= sprite.height / 2;

	ScaledDrawSprite(sprite, dest, rotation, color);
}

void ScaledDrawText(const char* text, int posX, int posY, int fontsize, Color color)
{
	posX = GetScaledX(posX);
	posY = GetScaledY(posY);
	fontsize = GetScaledY(fontsize);

	DrawText(text, posX, posY, fontsize, color);
}

void ScaledDrawTextVec(const char* text, Vector2 pos, int fontsize, Color color)
{
	ScaledDrawText(text, (int)(pos.x), (int)(pos.y), fontsize, color);
}

void ScaledDrawTextAligned(const char* text, int posX, int posY, int fontsize, Color color, TEXT_ALIGNMENT alignment)
{
	posX = GetScaledX(posX);
	posY = GetScaledY(posY);
	fontsize = GetScaledY(fontsize);

	DrawTextAligned(text, posX, posY, fontsize, color, alignment);
}

void ScaledDrawTextVecAligned(const char* text, Vector2 pos, int fontsize, Color color, TEXT_ALIGNMENT alignment)
{
	int posX = GetScaledX((int)pos.x);
	int posY = GetScaledY((int)pos.y);
	fontsize = GetScaledY(fontsize);

	DrawTextAligned(text, posX, posY, fontsize, color, alignment);
}

void ScaledDrawPoly(Vector2 center, int sides, float radius, float rotation, Color color)
{
	center.x = GetScaledX(center.x);
	center.y = GetScaledY(center.y);
	radius = MIN(GetScaledX(radius), GetScaledY(radius));

	DrawPoly(center, sides, radius, rotation, color);
}

void ScaledDrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color)
{
	center.x = GetScaledX(center.x);
	center.y = GetScaledY(center.y);
	radius = (MIN(GetScaledX(radius), GetScaledY(radius)) + MAX(GetScaledX(radius), GetScaledY(radius)))/2.0f;

	DrawPolyLines(center, sides, radius, rotation, color);
}