#ifndef SCALED_DRAW_H_
#define SCALED_DRAW_H_

#include "raylib.h"
#include "hamlib.h"

enum ScalingFlags
{
	SF_MAINT_RATIO
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

/**
 * \brief Draw Rectangle scaled subject to ScalingParameters
 */
void ScaledDrawRect(Rectangle rect, Color color);

/**
 * \brief Draw Rectangle scaled subject to ScalingParameters
 * \param rect Rectangle.x, Rectangle.y describes the center of the rectangle.
 */
void ScaledDrawRectCentered(Rectangle rect, Color color);

/**
 * \brief Draw Sprite scaled subject to ScalingParameters
 */
void ScaledDrawSprite(Texture2D sprite, Rectangle dest, float rotation, Color color);

/**
 * \brief Draw Sprite scaled subject to ScalingParameters
 * \param dest Rectangle.x, Rectangle.y describes the center of the sprite.
 */
void ScaledDrawSpriteCentered(Texture2D sprite, Rectangle dest, float rotation, Color color);

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

#endif