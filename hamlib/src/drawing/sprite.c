#include "hamlib/sprite.h"

#include <stdlib.h>

// debug
#include <stdio.h>

sprite setSourceRectangle(sprite* spr, Rectangle source)
{
	spr->source = source;
	spr->width = source.width;
	spr->height = source.height;

	return *spr;
}

sprite setAtlas(sprite* spr, Texture2D atlas)
{
	spr->atlas = atlas;

	return *spr;
}

sprite newSprite(Texture2D atlas, Rectangle source)
{
	// particleSprite particleSprite = (particleSprite){atlas, source, source.width, source.height};
	sprite sprite;
	sprite.atlas = atlas;
	sprite.source = source;
	sprite.width = source.width;
	sprite.height = source.height;

	printf("LOG: new particleSprite created: atlas id %d\t source rect x = %.0f\ty = %.0f\tw = %.0f\th = %.0f\n", atlas.id, sprite.source.x, sprite.source.y, sprite.source.width, sprite.source.height);

	return sprite;
}

void drawSprite(sprite spr, int posX, int posY, Color tint)
{
	Rectangle dest = {posX, posY, spr.width, spr.height};

	// drawSpritePro(spr, dest, getRectCenter(dest), 0.0f, tint);
	// drawSpritePro(spr, dest, getRectCenterRelative(dest), 0.0f, tint);
	drawSpritePro(spr, dest, (Vector2) {0, 0}, 0.0f, tint);
}

void drawSpriteV(sprite spr, Vector2 position, Color tint)
{
	Rectangle dest = {position.x, position.y, spr.width, spr.height};
	
	drawSpritePro(spr, dest, (Vector2) {0, 0}, 0.0f, tint);
}

void drawSpritePro(sprite spr, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	DrawTexturePro(spr.atlas, spr.source, dest, origin, rotation, tint);
}

sprite* spriteArrayFromStripAtlas(Texture2D atlas, int cellCount, int cellWidth, int cellHeight)
{
	sprite* sprites;
	
	if ((atlas.width / cellWidth) < cellCount)
		return 0;

	sprites = (sprite*)malloc(sizeof(sprite) * cellCount);
	for(int i = 0; i < cellCount; i++)
		sprites[i] = newSprite(atlas, (Rectangle) {cellWidth * i, 0, cellWidth, cellHeight});

	return sprites;
}

sprite* spriteArrayFromStripAtlasVertical(Texture2D atlas, int cellCount, int cellWidth, int cellHeight)
{
	sprite* sprites;

	if ((atlas.height / cellHeight) < cellCount)
		return 0;

	sprites = (sprite*)malloc(sizeof(sprite) * cellCount);
	for(int i = 0; i < cellCount; i++)
		sprites[i] = newSprite(atlas, (Rectangle) {0, i * cellHeight, cellWidth, cellHeight});

	return sprites;
}
