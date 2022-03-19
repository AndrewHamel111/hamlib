/* TODO
** - consider replacing Texture2D* with Texture2D considering it's basically like 5 ints, pointer might be more performant but could cause issues?
*/

#ifndef HAMLIB_SPRITE_H
#define HAMLIB_SPRITE_H

#include "raylib.h"

typedef struct sprite
{
	Texture2D atlas;
	Rectangle source;

	int width;
	int height;
} sprite;

sprite setAtlas(sprite* spr, Texture2D atlas);
sprite setSourceRectangle(sprite* spr, Rectangle source);
sprite newSprite(Texture2D atlas, Rectangle source);

void drawSprite(sprite spr, int posX, int posY, Color tint);
void drawSpriteV(sprite spr, Vector2 position, Color tint);
void drawSpritePro(sprite spr, Rectangle dest, Vector2 origin, float rotation, Color tint);

/**
 * \brief Returns an array of sprites made from splitting Texture2D atlas into atlas.width / cellWidth number of sprites, all with height cellHeight. For the transpose, try spriteArrayFromStripAtlasVertical
 * \return particleSprite* Array of sprites
 */
sprite* spriteArrayFromStripAtlas(Texture2D atlas, int cellCount, int cellWidth, int cellHeight);
/**
 * \brief Returns an array of sprites made from splitting Texture2D atlas into atlas.width / cellHeight number of sprites, all with width cellWidth.
 * \return particleSprite* Array of sprites
 */
sprite* spriteArrayFromStripAtlasVertical(Texture2D atlas, int cellCount, int cellWidth, int cellHeight);

#endif