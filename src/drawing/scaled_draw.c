#include "hamlib/draw.h"

static int desiredWidth;
static int desiredHeight;
static int scalingFlags;

static int templateWidth = 1280;
static int templateHeight = 720;

void setTemplateSize(int width, int height)
{
	templateWidth = width;
	templateHeight = height;

	// provide default values to width and height to avoid divide by zero or other problems.
	setDesiredSize(width, height);
}

void setDesiredSize(int width, int height)
{
	desiredWidth = width;
	desiredHeight = height;
}

void setScalingFlags(int flags)
{
	scalingFlags = scalingFlags | flags;
}

void unsetScalingFlags(int flags)
{
	scalingFlags = scalingFlags & !flags;
}

/**
 * \brief Scale the value according to change in WIDTH without consideration of SfMaintRatio
 */
static int getScaledWidth(int x)
{
	return (int)((float)desiredWidth * ((float)x / (float)templateWidth));
}

/**
 * \brief Scale the value according to change in HEIGHT without consideration of SfMaintRatio
 */
static int getScaledHeight(int y)
{
	return (int)((float)desiredHeight * ((float)y / (float)templateHeight));
}

/**
 * \brief Scale the value according to change in WIDTH
 */
static int getScaledX(int x)
{
	return getScaledWidth(x);
}

/**
 * \brief Scale the value according to change in HEIGHT
 */
static int getScaledY(int y)
{
	return getScaledHeight(y);
}

/**
 * \brief Scale a Vector using the X and Y scaling appropriately.
 */
static Vector2 getScaledVector(Vector2 vector)
{
	return (Vector2){getScaledX(vector.x), getScaledY(vector.y)};
}

/**
 * \brief Scale a Rectangle using X, Y, Width, and Height scaling appropriately.
 */
static Rectangle getScaledRect(Rectangle rectangle)
{
	return (Rectangle){getScaledX(rectangle.x), getScaledY(rectangle.y), getScaledWidth(rectangle.width),
					   getScaledHeight(rectangle.height)};
}

void scaledDrawRect(Rectangle rect, Color color)
{
	if (scalingFlags & SfCentered)
	{
		rect.x -= rect.width / 2;
		rect.y -= rect.height / 2;
	}

	// rect.x = (float)GetScaledX((int)rect.x);
	// rect.y = (float)GetScaledY((int)rect.y);
	// rect.width = (float)GetScaledX((int)rect.width);
	// rect.height = (float)GetScaledY((int)rect.height);
	rect = getScaledRect(rect);

	DrawRectangleRec(rect, color);
}

void scaledDrawTexture(Texture2D sprite, Rectangle dest, float rotation, Color color)
{
	if (scalingFlags & SfCentered)
	{
		dest.x -= sprite.width / 2;
		dest.y -= sprite.height / 2;
	}

	Rectangle src = pureSource(sprite);
	// dest.x = (float)GetScaledX((int)dest.x);
	// dest.y = (float)GetScaledY((int)dest.y);
	// dest.width = (float)GetScaledX((int)dest.width);
	// dest.height = (float)GetScaledY((int)dest.height);
	dest = getScaledRect(dest);

	// This adjustment is needed since we want to be able to use rotation with respect to the center of the particleSprite
	Rectangle scaledDest = (Rectangle){getRectCenter(dest).x, getRectCenter(dest).y, dest.width, dest.height};

	DrawTexturePro(sprite, src, scaledDest, getRectCenterRelative(dest), rotation, color);
}

void scaledDrawText(const char* text, int posX, int posY, int fontsize, Color color)
{
	posX = getScaledX(posX);
	posY = getScaledY(posY);
	fontsize = getScaledY(fontsize);

	DrawText(text, posX, posY, fontsize, color);
}

void scaledDrawTextVec(const char* text, Vector2 pos, int fontsize, Color color)
{
	scaledDrawText(text, (int) (pos.x), (int) (pos.y), fontsize, color);
}

void scaledDrawTextAligned(const char* text, int posX, int posY, int fontsize, Color color, textAlignment alignment)
{
	posX = getScaledX(posX);
	posY = getScaledY(posY);
	fontsize = getScaledY(fontsize);
	
	drawTextAligned(text, posX, posY, fontsize, color, alignment);
}

void scaledDrawTextVecAligned(const char* text, Vector2 pos, int fontsize, Color color, textAlignment alignment)
{
	int posX = getScaledX((int) pos.x);
	int posY = getScaledY((int) pos.y);
	fontsize = getScaledY(fontsize);
	
	drawTextAligned(text, posX, posY, fontsize, color, alignment);
}

void scaledDrawPoly(Vector2 center, int sides, float radius, float rotation, Color color)
{
	// center.x = GetScaledX(center.x);
	// center.y = GetScaledY(center.y);
	center = getScaledVector(center);
	radius = MIN(getScaledX(radius), getScaledY(radius));

	DrawPoly(center, sides, radius, rotation, color);
}

void scaledDrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color)
{
	// center.x = GetScaledX(center.x);
	// center.y = GetScaledY(center.y);
	center = getScaledVector(center);
	radius = (MIN(getScaledX(radius), getScaledY(radius)) + MAX(getScaledX(radius), getScaledY(radius))) / 2.0f;

	DrawPolyLines(center, sides, radius, rotation, color);
}

void scaledDrawSprite(sprite spr, int posX, int posY, Color tint)
{
	if (scalingFlags & SfCentered)
	{
		posX -= spr.width / 2;
		posY -= spr.height / 2;
	}

	posX = getScaledX(posX);
	posY = getScaledY(posY);
	spr.width = getScaledWidth(spr.width);
	spr.height = getScaledHeight(spr.height);
	
	drawSprite(spr, posX, posY, tint);
}

void scaledDrawSpritePro(sprite spr, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	if (scalingFlags & SfCentered)
	{
		origin = getRectCenterRelative(dest);
	}

	// dest.x = GetScaledX(dest.x);
	// dest.y = GetScaledY(dest.y);
	// dest.width = GetScaledWidth(dest.width);
	// dest.height = GetScaledHeight(dest.height);
	dest = getScaledRect(dest);
	origin = getScaledVector(origin);
	
	drawSpritePro(spr, dest, origin, rotation, tint);
}

void scaledDrawSpriteAnimation(float frametime, spriteAnimation* animation, int posX, int posY, Color tint)
{
	sprite sprite = getCurrentSprite(*animation);

	if (scalingFlags & SfCentered)
	{
		posX -= sprite.width/2;
		posY -= sprite.height/2;
	}

	posX = getScaledX(posX);
	posY = getScaledY(posY);
	sprite.width = getScaledWidth(sprite.width);
	sprite.height = getScaledHeight(sprite.height);
	
	drawSpriteAnimationBase(frametime, animation);
	drawSprite(sprite, posX, posY, tint);
}

void scaledDrawSpriteAnimationPro(float frametime, spriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	sprite sprite = getCurrentSprite(*animation);

	if (scalingFlags & SfCentered)
	{
		origin = getRectCenterRelative(dest);
	}

	dest = getScaledRect(dest);
	origin = getScaledVector(origin);
	
	drawSpriteAnimationBase(frametime, animation);
	drawSpritePro(sprite, dest, origin, rotation, tint);
}