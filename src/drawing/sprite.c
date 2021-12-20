#include "hamlib/sprite.h"
#include "hamlib.h"

#include <stdlib.h>

// debug
#include <stdio.h>

Sprite SetSourceRectangle(Sprite* sprite, Rectangle source)
{
	// TODO consider checking for "valid" rectangles
	sprite->source = source;
	sprite->width = source.width;
	sprite->height = source.height;

	return *sprite;
}

Sprite SetAtlas(Sprite* sprite, Texture2D atlas)
{
	// TODO consider checking for "valid" atlas
	sprite->atlas = atlas;

	return *sprite;
}

Sprite NewSprite(Texture2D atlas, Rectangle source)
{
	// Sprite sprite = (Sprite){atlas, source, source.width, source.height};
	Sprite sprite;
	sprite.atlas = atlas;
	sprite.source = source;
	sprite.width = source.width;
	sprite.height = source.height;

	printf("LOG: new sprite created: atlas id %d\t source rect x = %.0f\ty = %.0f\tw = %.0f\th = %.0f\n", atlas.id, sprite.source.x, sprite.source.y, sprite.source.width, sprite.source.height);

	return sprite;
}

void DrawSprite(Sprite sprite, int posX, int posY, Color tint)
{
	Rectangle dest = {posX, posY, sprite.width, sprite.height};

	// DrawSpritePro(sprite, dest, GetCenter(dest), 0.0f, tint);
	// DrawSpritePro(sprite, dest, GetCenterRelative(dest), 0.0f, tint);
	DrawSpritePro(sprite, dest, (Vector2){0,0}, 0.0f, tint);
}

void DrawSpriteV(Sprite sprite, Vector2 position, Color tint)
{
	Rectangle dest = {position.x, position.y, sprite.width, sprite.height};

	DrawSpritePro(sprite, dest, (Vector2){0,0}, 0.0f, tint);
}

void DrawSpritePro(Sprite sprite, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	DrawTexturePro(sprite.atlas, sprite.source, dest, origin, rotation, tint);
}

Sprite* SpriteArrayFromStripAtlas(Texture2D atlas, int cellCount, int cellWidth, int cellHeight)
{
	Sprite* sprites;
	// int _i = atlas.width / cellWidth;
	int _i = cellCount;

	if ((atlas.width / cellWidth) < _i)
		return 0;

	sprites = (Sprite*)malloc(sizeof(Sprite) * _i);
	for(int i = 0; i < _i; i++)
		sprites[i] = NewSprite(atlas, (Rectangle){cellWidth * i, 0, cellWidth, cellHeight});

	return sprites;
}

Sprite* SpriteArrayFromStripAtlasVertical(Texture2D atlas, int cellCount, int cellWidth, int cellHeight)
{
	Sprite* sprites;
	int _i = cellCount;

	if ((atlas.height / cellHeight) < _i)
		return 0;

	sprites = (Sprite*)malloc(sizeof(Sprite) * _i);
	for(int i = 0; i < _i; i++)
		sprites[i] = NewSprite(atlas, (Rectangle){0, i * cellHeight, cellWidth, cellHeight});

	return sprites;
}
