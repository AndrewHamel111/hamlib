/* TODO
** - consider replacing Texture2D* with Texture2D considering it's basically like 5 ints, pointer might be more performant but could cause issues?
*/

#ifndef SPRITE_H_
#define SPRITE_H_

#include "raylib.h"

typedef struct Sprite
{
	Texture2D atlas;
	Rectangle source;

	int width;
	int height;
} Sprite;

Sprite SetAtlas(Sprite* sprite, Texture2D atlas);
Sprite SetSourceRectangle(Sprite* sprite, Rectangle source);
Sprite NewSprite(Texture2D atlas, Rectangle source);

void DrawSprite(Sprite sprite, int posX, int posY, Color tint);
void DrawSpriteV(Sprite sprite, Vector2 position, Color tint);
void DrawSpritePro(Sprite sprite, Rectangle dest, Vector2 origin, float rotation, Color tint);

/**
 * \brief Returns an array of sprites made from splitting Texture2D atlas into atlas.width / cellWidth number of sprites, all with height cellHeight. For the transpose, try SpriteArrayFromStripAtlasVertical
 * \return Sprite* Array of sprites
 */
Sprite* SpriteArrayFromStripAtlas(Texture2D atlas, int cellCount, int cellWidth, int cellHeight);
/**
 * \brief Returns an array of sprites made from splitting Texture2D atlas into atlas.width / cellHeight number of sprites, all with width cellWidth.
 * \return Sprite* Array of sprites
 */
Sprite* SpriteArrayFromStripAtlasVertical(Texture2D atlas, int cellCount, int cellWidth, int cellHeight);

#endif