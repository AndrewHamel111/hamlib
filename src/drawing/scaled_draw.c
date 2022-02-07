#include "hamlib/draw.h"

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

	// provide default values to width and height to avoid divide by zero or other problems.
	SetScalingSize(width, height);
}

void SetScalingSize(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
}

void SetScalingFlags(int flags)
{
	FLAGS = FLAGS | flags;
}

void UnsetScalingFlags(int flags)
{
	FLAGS = FLAGS & !flags;
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
	if ((FLAGS & SF_MAINT_RATIO) && !SameRatio())
	{
		// float desiredRatio = (float)TEMPLATE_WIDTH / (float)TEMPLATE_HEIGHT;
		// if (MIN(WIDTH/TEMPLATE_WIDTH, HEIGHT/TEMPLATE_HEIGHT) == WIDTH/TEMPLATE_WIDTH)
		// {

		// }
		// else
		// {

		// }

		// // scale X by whatever Y gets scaled by
		// float exampleY = 100;
		// float adjustment = exampleY / GetScaledX(exampleY);
		// return x * adjustment;
	}
	
	return GetScaledWidth(x);
}

/**
 * \brief Scale the value according to change in HEIGHT
 */
static int GetScaledY(int y)
{
	if ((FLAGS & SF_MAINT_RATIO) && !SameRatio())
	{
		// // scale Y by whatever X gets scaled by
		// float exampleX = 100;
		// float adjustment = exampleX / GetScaledX(exampleX);
		// return y * adjustment;
	}
	
	return GetScaledHeight(y);
}

/**
 * \brief Scale a Vector using the X and Y scaling appropriately.
 */
static Vector2 GetScaledVector(Vector2 vector)
{
	return (Vector2){GetScaledX(vector.x), GetScaledY(vector.y)};
}

/**
 * \brief Scale a Rectangle using X, Y, Width, and Height scaling appropriately.
 */
static Rectangle GetScaledRect(Rectangle rectangle)
{
	return (Rectangle){GetScaledX(rectangle.x), GetScaledY(rectangle.y), GetScaledWidth(rectangle.width), GetScaledHeight(rectangle.height)};
}

void ScaledDrawRect(Rectangle rect, Color color)
{
	if (FLAGS & SF_CENTERED)
	{
		rect.x -= rect.width / 2;
		rect.y -= rect.height / 2;
	}

	// rect.x = (float)GetScaledX((int)rect.x);
	// rect.y = (float)GetScaledY((int)rect.y);
	// rect.width = (float)GetScaledX((int)rect.width);
	// rect.height = (float)GetScaledY((int)rect.height);
	rect = GetScaledRect(rect);

	DrawRectangleRec(rect, color);
}

void ScaledDrawTexture(Texture2D texture, Rectangle dest, float rotation, Color color)
{
	if (FLAGS & SF_CENTERED)
	{
		dest.x -= texture.width / 2;
		dest.y -= texture.height / 2;
	}

	Rectangle src = PureSource(texture);
	// dest.x = (float)GetScaledX((int)dest.x);
	// dest.y = (float)GetScaledY((int)dest.y);
	// dest.width = (float)GetScaledX((int)dest.width);
	// dest.height = (float)GetScaledY((int)dest.height);
	dest = GetScaledRect(dest);

	// This adjustment is needed since we want to be able to use rotation with respect to the center of the sprite
	Rectangle _dest = (Rectangle){GetCenter(dest).x, GetCenter(dest).y, dest.width, dest.height};

	DrawTexturePro(texture, src, _dest, GetCenterRelative(dest), rotation, color);
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
	// center.x = GetScaledX(center.x);
	// center.y = GetScaledY(center.y);
	center = GetScaledVector(center);
	radius = MIN(GetScaledX(radius), GetScaledY(radius));

	DrawPoly(center, sides, radius, rotation, color);
}

void ScaledDrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color)
{
	// center.x = GetScaledX(center.x);
	// center.y = GetScaledY(center.y);
	center = GetScaledVector(center);
	radius = (MIN(GetScaledX(radius), GetScaledY(radius)) + MAX(GetScaledX(radius), GetScaledY(radius)))/2.0f;

	DrawPolyLines(center, sides, radius, rotation, color);
}

void ScaledDrawSprite(Sprite sprite, int posX, int posY, Color tint)
{
	if (FLAGS & SF_CENTERED)
	{
		posX -= sprite.width/2;
		posY -= sprite.height/2;
	}

	posX = GetScaledX(posX);
	posY = GetScaledY(posY);
	sprite.width = GetScaledWidth(sprite.width);
	sprite.height = GetScaledHeight(sprite.height);

	DrawSprite(sprite, posX, posY, tint);
}

void ScaledDrawSpritePro(Sprite sprite, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	if (FLAGS & SF_CENTERED)
	{
		origin = GetCenterRelative(dest);
	}

	// dest.x = GetScaledX(dest.x);
	// dest.y = GetScaledY(dest.y);
	// dest.width = GetScaledWidth(dest.width);
	// dest.height = GetScaledHeight(dest.height);
	dest = GetScaledRect(dest);
	origin = GetScaledVector(origin);

	DrawSpritePro(sprite, dest, origin, rotation, tint);
}

void ScaledDrawSpriteAnimation(float frametime, SpriteAnimation* animation, int posX, int posY, Color tint)
{
	Sprite sprite = animation->sprites[animation->_frame_current];

	if (FLAGS & SF_CENTERED)
	{
		posX -= sprite.width/2;
		posY -= sprite.height/2;
	}

	posX = GetScaledX(posX);
	posY = GetScaledY(posY);
	sprite.width = GetScaledWidth(sprite.width);
	sprite.height = GetScaledHeight(sprite.height);

	DrawSpriteAnimationBase(frametime, animation);
	DrawSprite(sprite, posX, posY, tint);
}

void ScaledDrawSpriteAnimationPro(float frametime, SpriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	Sprite sprite = animation->sprites[animation->_frame_current];

	if (FLAGS & SF_CENTERED)
	{
		origin = GetCenterRelative(dest);
	}

	dest = GetScaledRect(dest);
	origin = GetScaledVector(origin);

	DrawSpriteAnimationBase(frametime, animation);
	DrawSpritePro(sprite, dest, origin, rotation, tint);
}