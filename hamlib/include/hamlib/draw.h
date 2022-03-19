#ifndef HAMLIB_DRAW_H
#define HAMLIB_DRAW_H

#include "hamlib.h"
#include "animation.h"

//// FROM OLD HAMLIB.H ///////////
/**
 * \brief Draws text with the specified alignment.
 * 
 * \param text Text to be drawn.
 * \param posX X position of the text anchor.
 * \param posY Y position of the text anchor.
 * \param fontSize Height of the drawn text in pixels.
 * \param color Color of the text.
 * \param alignment Enum value describing the text alignment. Use | to specify a horizontal and vertical alignment.
 */
void drawTextAligned(const char* text, int posX, int posY, int fontSize, Color color, textAlignment alignment);

void drawTextAlignedPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint, textAlignment alignment);

/**
 * \brief Draws a texture using DrawTexturePro.
 * 
 * \param texture Texture to draw
 * \param pos Position to draw at
 * \param scaling X and Y scaling to use
 */
void drawTextureScaled(Texture2D texture, Vector2 pos, Vector2 scaling, Color color);
//// FROM OLD HAMLIB.H ///////////

//// FROM CENTERED_DRAW.H ///////////
void drawRectangleCentered(int posX, int posY, int width, int height, Color color);
void drawRectangleCenteredV(Vector2 position, Vector2 size, Color color);
void drawRectangleCenteredRec(Rectangle rectangle, Color color);
//// FROM CENTERED_DRAW.H ///////////


//// FROM SCALED_DRAW.H ///////////
enum scalingFlags
{
	SfMaintRatio, SfCentered
};

/** \brief Call at the start of program to assign WIDTH and HEIGHT variables to the size of your game variables are in terms of.
* \param width set width of screen
* \param height set height of screen
*/
void setTemplateSize(int width, int height);

/** \brief Call at the start of program to initialize the target width and height for the window.
* \param width set width of screen
* \param height set height of screen
*/
void setDesiredSize(int width, int height);

/** \brief set the scaling flags used by ScaledDraw* functions.
 * \param flags 
 */
void setScalingFlags(int flags);

/** \brief Unset the scaling flags used by ScaledDraw* functions.
 * \param flags 
 */
void unsetScalingFlags(int flags);

/**
 * \brief Draw Rectangle scaled subject to ScalingParameters
 */
void scaledDrawRect(Rectangle rect, Color color);

/**
 * \brief Draw particleSprite scaled subject to ScalingParameters
 */
void scaledDrawTexture(Texture2D sprite, Rectangle dest, float rotation, Color color);

/**
 * \brief Draw Text scaled subject to ScalingParameters
 */
void scaledDrawText(const char* text, int posX, int posY, int fontsize, Color color);

/**
 * \brief Draw Text scaled subject to ScalingParameters
 * \param pos Position of the text
 */
void scaledDrawTextVec(const char* text, Vector2 pos, int fontsize, Color color);

/**
 * \brief Draw Text scaled subject to ScalingParameters with alignment
 * \param alignment Use bitwise or | to combine alignment details.
 */
void scaledDrawTextAligned(const char* text, int posX, int posY, int fontsize, Color color, textAlignment alignment);

/**
 * \brief Draw Text scaled subject to ScalingParameters with alignment
 * \param pos Position of the text
 * \param alignment Use bitwise or | to combine alignment details.
 */
void scaledDrawTextVecAligned(const char* text, Vector2 pos, int fontsize, Color color, textAlignment alignment);

/**
 * \brief Draw Polygon scaled subject to ScalingParameters
 */
void scaledDrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);

/**
 * \brief Draw Polygon lines scaled subject to ScalingParameters
 */
void scaledDrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);

/**
 * \brief Draw particleSprite scaled subject to ScalingParameters
 */
void scaledDrawSprite(sprite spr, int posX, int posY, Color tint);

/**
 * \brief Draw particleSprite scaled subject to ScalingParameters
 */
void scaledDrawSpritePro(sprite spr, Rectangle dest, Vector2 origin, float rotation, Color tint);

/**
 * \brief Draw spriteAnimation scaled subject to ScalingParameters
 */
void scaledDrawSpriteAnimation(float frametime, spriteAnimation* animation, int posX, int posY, Color tint);

/**
 * \brief Draw spriteAnimation scaled subject to ScalingParameters
 */
void scaledDrawSpriteAnimationPro(float frametime, spriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint);
//// FROM SCALED_DRAW.H ///////////

#endif