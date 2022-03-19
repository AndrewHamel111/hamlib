/**
 * \file hamlib.h
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \brief A small helper library developed to run along side Raylib.
 * \version 0.1
 * \date 2021-08-16
 * 
 * @copyright Copyright (c) 2021
 */

// TODO Move draw.h concepts from microgame madness into draw/ src folder. Some should be in global hamlib.h, others in a specialized hamlib/scaled_draw.h

#ifndef HAMLIB_HAMLIB_H
#define HAMLIB_HAMLIB_H

#include "raylib.h"

#include "hamlib/particle.h"
#include "hamlib/rectangle_math.h"

// macros
/** \brief Square of a value. */
#define SQR(A) ((A) * (A))
/** \brief Minimum of two values. */
#define MIN(A,B) ((A) > (B) ? (B) : (A))
/** \brief Maximum of two values. */
#define MAX(A,B) ((A) < (B) ? (B) : (A))
/** \brief Clamps T by A and B. Identity function when A <= T <= B. */
#define CLAMP(T,A,B) ( (T) < (A) ? (A) : ( (T > B) ? (B) : (T) ) )
/** \brief Clamps T by A and B. Identity function when A <= T <= B. */
#define CLAMP_01(T) ( CLAMP(T, 0.0f, 1.0f) )
/** \brief Similar to Clamp, but when T Exceeds B it returns to A and vice versa. */
#define CYCLE(T,A,B) ( (T) < (A) ? (B) : ( (T > B) ? (A) : (T) ) )
/** \brief Linear-Interpolation between A and B by T. */
#define LERP(A,B,T) ((B)*(T) + (A)*(1 - T))
/** \brief LERP but T is clamped to 0 and 1. */
#define LERP_CLAMP(A,B,T) ((B)*(CLAMP(T,0,1)) + (A)*(1 - (CLAMP(T,0,1))))
/** \brief Finds the Absolute Value */
#define ABS(A) ((A < 0) ? -A : A))

/**
 * \brief An enum used to determine text alignment.
 * \details This enum supports | operations to achieve multiple styles:
 * i.e. TaCenter | TaBottom will have the text centered horizontally but
 * draw upward from posY.
 */
typedef enum textAlignment
{
	TaLeft = 1, /** Horizontal Alignment - all text right of posX */
	TaCenter = 2, /** Horizontal Alignment - centered on posX */
	TaRight = 4, /** Horizontal Alignment - all text left of posX */
	TaTop = 8, /** Vertical Alignment - all text below posY */
	TaMiddle = 16, /** Vertical Alignment - Centered on posY*/
	TaBottom = 32 /** Vertical Alignment - all text above posY*/
} textAlignment;

/**
 * \brief Generates a random color
 * 
 * \return A Random Color
 */
Color randomcolor(void);

/** \fn Color tintcolor(Color color, float amount)
* Tints a color by a normalized amount.
* \param color Color to be tinted.
* \param amount Normalized value. 0.0f returns color, 1.0f returns WHITE or BLACK.
* \returns Tinted color.
*/
Color tintcolor(Color color, float amount);

/**
 * \brief Linearly interpolates from one color to another.
 * 
 * \param c1 First color
 * \param c2 Second color
 * \param t Normalized float (0 <= t <= 1)
 * \return c1 when t == 0, c2 when t == 1 
 */
Color lerpcolor(Color c1, Color c2, float t);

///////////////
// PARTICLES //
///////////////

void createParticle(particle p);
void updateParticles(float frametime);
void drawParticles(void);
void destroyAllParticles(void);

/////////////
// UTILITY //
/////////////

/**
 * \brief Get the Center object
 * 
 * \param rect Rectangle describing object bounds
 * \return Vector2 Vector at the center of the object
 */
Vector2 getRectCenter(Rectangle rect);

/**
 * \brief Get the Center of the Rectangle relative to the top-left corner
 * 
 * \param rect Rectangle describing object bounds
 * \return Vector2 Vector at the center of the object assuming object is at origin
 */
Vector2 getRectCenterRelative(Rectangle rect);

/**
 * \brief Returns a Source Rectangle of the full texture.
 * \details Used best for functions like DrawTexturePro that require a source rectangle even when you don't necessarily need it, for example, for non-atlassed sprites.
 * \param texture The texture to get a pure source from. 
 * \return Rectangle A source rectangle with x,y = 0,0 and width,height matching the image's size.
 */
Rectangle pureSource(Texture2D texture);

/**
 * \brief Create a Rectangle from two vectors, using the first as a position and the second as a dimension.
 */
Rectangle recFromVec(Vector2 position, Vector2 size);

/**
 * \brief Swaps two objects in-place.
 * \details The caller should specify the two pointers, followed by a call to sizeof and one of the objects.
 * Optionally, they can use sizeof() with the direct object type, or wrap this function with a custom.
 * I.e., for object Object, they could write a Swap(Object* a, Object* b) { swap(a, b, sizeof(Object)); }
 * \param a 
 * \param b 
 * \param sizeOf sizeOf objects to be swapped.
 */
void swap(void* a, void* b, int sizeOf);

/**
 * \brief A primitive shuffle algorithm. srand() must be set for good results.
 * 
 * \param array Pointer to array to be shuffled.
 * \param count Length of array.
 * \param sizeOf sizeof(ObjectInArray)
 */
void shuffle(void* array, int count, int sizeOf);

/**
 * \brief A primitive shuffle algorithm. Operates in a specified range.
 * 
 * \param array Pointer to array to be shuffled.
 * \param count Length of array.
 * \param sizeOf sizeof(ObjectInArray)
 * \param start starting index of the shuffle range (inclusive).
 * \param end ending index of the shuffle range (inclusive).
 */
void shufflerange(void* array, int count, int sizeOf, int start, int end);

/**
 * \brief Sets the first [count] values of [array] to the values specified in [...]
 * 
 * \param array Pointer to array
 * \param count Number of values
 * \param ... Values to be set
 */
void setintvalues(int* array, int count, ...);

/**
 * \brief Sets the first [count] values of [array] to the values specified in [...]
 * 
 * \param array Pointer to array
 * \param count Number of values
 * \param ... Values to be set
 */
void setcharvalues(unsigned char* array, unsigned char count, ...);
/**
 * \brief Sets the first [count] values of [array] to the values specified in [...]
 * 
 * \param array Pointer to array
 * \param count Number of values
 * \param ... Values to be set
 */
void setfloatvalues(float* array, int count, ...);

/**
 * \brief Frees the specified memory only if it is not already null. 
 * \return true If the memory was non-null and a free occurred. 
 * \return false If the memory was null.
 */
bool smartfree(void* ptr);

/**
 * \brief Returns the sign of the value, 0 considered positive.
 * \return float -1.0f or 1.0f
 */
float signf(float);

/**
 * \brief Returns the sign of the value, 0 considered positive.
 * \return int -1 or 1
 */
int sign(int);

/** \brief Returns either 1 or -1
 */
int randsign(void);

/**
 * \brief Returns either 1.0f or -1.0f
 */
float randsignf(void);

/**
 * \brief Reduce approaches zero
 */
void reducebyf(float*,float);

/**
 * \brief Reduce approaches zero
 */
void reducebyi(int*,int);

/**
 * \brief Increase the magnitude of f by r.
 */
void increasebyf(float* f, float r);

/**
 * \brief Increase the magnitude of f by r.
 */
void increasebyi(int* f, int r);

/// TWEENING ///

void tweenUpdate(float frametime);
void tweenClear(void);

void setNextTweenEasingMethod(float (*easingMethod)(float,float,float,float));
void tweenInt(int* value, int start, int end, float time);
void tweenFloat(float* value, float start, float end, float time);
void tweenVector(Vector2* value, Vector2 start, Vector2 end, float time);

/// MESSAGE QUEUE ///

#ifndef HAMLIB_MESSAGE_H
#define HAMLIB_MESSAGE_H

#ifndef MESSAGE_FONT_SIZE
	#define MESSAGE_FONT_SIZE 45
#endif
#ifndef MESSAGE_EASE_TIME
	#define MESSAGE_EASE_TIME 0.6f
#endif
#ifndef MESSAGE_FADE_TIME
	#define MESSAGE_FADE_TIME 0.8f
#endif
void showMessage(const char* text, float time, Color color);
void showMessagePro(const char* text, float time, Color textColor, Color color);
void showError(const char* msg, float time);
void clearMessages(void);
void drawMessage(float frametime);
void setMessagePositions(float startX, float startY, float endX, float endY);
void setMessagePositionsVec(Vector2 start, Vector2 end);

#endif

#endif