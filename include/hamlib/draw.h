#ifndef HAMLIB_DRAW_H_
#define HAMLIB_DRAW_H_

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
void DrawTextAligned(const char* text, int posX, int posY, int fontSize, Color color, TEXT_ALIGNMENT alignment);

void DrawTextAlignedPro(const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint, TEXT_ALIGNMENT alignment);

/**
 * \brief Draws a texture using DrawTexturePro.
 * 
 * \param texture Texture to draw
 * \param pos Position to draw at
 * \param scaling X and Y scaling to use
 */
void DrawTextureScaled(Texture2D texture, Vector2 pos, Vector2 scaling, Color color);
//// FROM OLD HAMLIB.H ///////////

//// FROM CENTERED_DRAW.H ///////////
void DrawRectangleCentered(int posX, int posY, int width, int height, Color color);
void DrawRectangleCenteredV(Vector2 position, Vector2 size, Color color);
void DrawRectangleCenteredRec(Rectangle rectangle, Color color);
//// FROM CENTERED_DRAW.H ///////////


//// FROM SCALED_DRAW.H ///////////
enum ScalingFlags
{
	SF_MAINT_RATIO, SF_CENTERED
};

/** \brief Call at the start of program to assign WIDTH and HEIGHT variables to the size of your game variables are in terms of.
* \param width Set width of screen
* \param height Set height of screen
*/
void SetOriginalSize(int width, int height);

/** \brief Call at the start of program to initialize the target width and height for the window.
* \param width Set width of screen
* \param height Set height of screen
*/
void SetScalingSize(int width, int height);

/** \brief Set the scaling flags used by ScaledDraw* functions.
 * \param flags 
 */
void SetScalingFlags(int flags);

/** \brief Unset the scaling flags used by ScaledDraw* functions.
 * \param flags 
 */
void UnsetScalingFlags(int flags);

/**
 * \brief Draw Rectangle scaled subject to ScalingParameters
 */
void ScaledDrawRect(Rectangle rect, Color color);

/**
 * \brief Draw Sprite scaled subject to ScalingParameters
 */
void ScaledDrawTexture(Texture2D sprite, Rectangle dest, float rotation, Color color);

/**
 * \brief Draw Text scaled subject to ScalingParameters
 */
void ScaledDrawText(const char* text, int posX, int posY, int fontsize, Color color);

/**
 * \brief Draw Text scaled subject to ScalingParameters
 * \param pos Position of the text
 */
void ScaledDrawTextVec(const char* text, Vector2 pos, int fontsize, Color color);

/**
 * \brief Draw Text scaled subject to ScalingParameters with alignment
 * \param alignment Use bitwise or | to combine alignment details.
 */
void ScaledDrawTextAligned(const char* text, int posX, int posY, int fontsize, Color color, TEXT_ALIGNMENT alignment);

/**
 * \brief Draw Text scaled subject to ScalingParameters with alignment
 * \param pos Position of the text
 * \param alignment Use bitwise or | to combine alignment details.
 */
void ScaledDrawTextVecAligned(const char* text, Vector2 pos, int fontsize, Color color, TEXT_ALIGNMENT alignment);

/**
 * \brief Draw Polygon scaled subject to ScalingParameters
 */
void ScaledDrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);

/**
 * \brief Draw Polygon lines scaled subject to ScalingParameters
 */
void ScaledDrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);

/**
 * \brief Draw Sprite scaled subject to ScalingParameters
 */
void ScaledDrawSprite(Sprite sprite, int posX, int posY, Color tint);

/**
 * \brief Draw Sprite scaled subject to ScalingParameters
 */
void ScaledDrawSpritePro(Sprite sprite, Rectangle dest, Vector2 origin, float rotation, Color tint);

/**
 * \brief Draw SpriteAnimation scaled subject to ScalingParameters
 */
void ScaledDrawSpriteAnimation(float frametime, SpriteAnimation* animation, int posX, int posY, Color tint);

/**
 * \brief Draw SpriteAnimation scaled subject to ScalingParameters
 */
void ScaledDrawSpriteAnimationPro(float frametime, SpriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint);
//// FROM SCALED_DRAW.H ///////////

#endif