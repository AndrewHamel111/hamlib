#ifndef HAMLIB_ANIMATION_H
#define HAMLIB_ANIMATION_H

#include "hamlib/sprite.h"

typedef enum spriteAnimationType
{
	SatOnce, SatLooped, SatPingPong, SatPingPongOnce, SatLoopedN, SatPingPongN
} spriteAnimationType;

// typedef struct spriteAnimation // Old system, where an animation is an array of sprites. In truth, an animation is going to share an atlas, so this was a wasteful approach.
// {
// 	particleSprite* sprites;
// 	int spritesLength;
// 	int framesPerSecond;
// 	enum spriteAnimationType animationType;
// 	int loopCount;

// 	// PRIVATE
// 	float secondsOnFrame;
// 	int frameCurrent;
// 	float secondsPerFrame;
// 	bool playing;
// 	bool pingPongReverse;
// 	int loopsLeft;

// } spriteAnimation;

typedef struct spriteAnimation
{
	sprite baseSprite;
	Vector2* sourcePositions;
	int spritesLength;
	int framesPerSecond;
	spriteAnimationType animationType;
	int loopCount;

	// PRIVATE
	float secondsOnFrame;
	int frameCurrent;
	float secondsPerFrame;
	bool playing;
	bool pingPongReverse;
	int loopsLeft;

} spriteAnimation;

/**
 * \brief Every call to DrawSpriteAnimation__ must perform these actions first, to maintain the animation state.
 */
void drawSpriteAnimationBase(float frametime, spriteAnimation* animation);

/**
 * \brief spriteAnimation constructor from minimum fields.
 * 
 * \param spritesLength no. of frames in animation
 * \param sprites array of sprites corresponding to the animation
 * \param framesPerSecond frames per second of the animation
 * \return spriteAnimation
 */
spriteAnimation newAnimationFromSprites(int spritesLength, sprite sprites[], int framesPerSecond);

/**
 * \brief spriteAnimation constructor from minimum fields.
 * 
 * \param spritesLength no. of frames in animation
 * \param baseSprite the base particleSprite of the animation, setting an example for the size of the source rectangles and providing the atlas
 * \param sourcePositions array of positions corresponding to each frame of the animation, where the position is used for the x and y of the source rectangle for frame i
 * \param framesPerSecond frames per second of the animation
 * \return spriteAnimation
 */
spriteAnimation newAnimation(int spritesLength, sprite baseSprite, Vector2 sourcePositions[], int framesPerSecond);
/**
 * \brief set the animations framerate. Sets invalid values to 1.
 * 
 * \return spriteAnimation modified animation
 */
spriteAnimation changeAnimationFramerate(spriteAnimation* animation, int framesPerSecond);

/**
 * \brief set the value of N for N loop styles.
 */
spriteAnimation setSpriteAnimationLoops(spriteAnimation* animation, int loopCount, bool usePingPong);

/**
 * \brief Draws the current particleSprite of the animation. If the animation is not playing, it will simply draw the "locked" frame of the animation until Reset/Start/Stop are called.
 */
void drawSpriteAnimation(float frametime, spriteAnimation* animation, int posX, int posY, Color tint);
/**
 * \brief Draws the current particleSprite of the animation. If the animation is not playing, it will simply draw the "locked" frame of the animation until Reset/Start/Stop are called.
 */
void drawSpriteAnimationV(float frametime, spriteAnimation* animation, Vector2 position, Color tint);
/**
 * \brief Draws the current particleSprite of the animation. If the animation is not playing, it will simply draw the "locked" frame of the animation until Reset/Start/Stop are called.
 */
void drawSpriteAnimationPro(float frametime, spriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint);

/**
 * \brief Simple shorthand to get the current particleSprite.
 */
sprite getCurrentSprite(spriteAnimation animation);

/**
 * \brief Reset all runtime values of spriteAnimation except playing.
 */
void spriteAnimationReset(spriteAnimation* animation);
/**
 * \brief Sets playing = false.
 */
void spriteAnimationStop(spriteAnimation* animation);
/**
 * \brief Sets playing = true.
 */
void spriteAnimationResume(spriteAnimation* animation);
/**
 * \brief Calls ResetAnimation, then ResumeAnimation.
 */
void spriteAnimationStart(spriteAnimation* animation);

#endif