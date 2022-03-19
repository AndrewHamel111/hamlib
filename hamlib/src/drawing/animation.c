#include "hamlib/animation.h"
#include "hamlib.h"

#include <stdlib.h>

//debug
//#define PRINT_DEBUG

#ifdef PRINT_DEBUG
	#include <stdio.h>
#endif

/// INTERNAL FUNCTIONS ///

static void addTime(spriteAnimation* animation, float frametime);
static void advanceFrame(spriteAnimation* animation);
static void animationUpdatePrivates(spriteAnimation* animation);
static void newLoop(spriteAnimation* animation);

static void addTime(spriteAnimation* animation, float frametime)
{
	animation->secondsOnFrame += frametime;
	while (animation->secondsOnFrame >= animation->secondsPerFrame)
	{
		animation->secondsOnFrame -= animation->secondsPerFrame;
		advanceFrame(animation);

		// account for floating point errors and weirdness
		if (animation->secondsOnFrame < 0)
			animation->secondsOnFrame = 0;
	}
}

static void advanceFrame(spriteAnimation* animation)
{
	switch(animation->animationType)
	{
		case SatOnce:
		{
			if (animation->frameCurrent < animation->spritesLength - 1)
				(animation->frameCurrent)++;
			else if (animation->frameCurrent >= animation->spritesLength - 1)
			{
				animation->playing = false;
			}
			else
			{
				// error case.
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SatOnce: frame_current %d\n", animation->frameCurrent);
#endif
			}
			break;	
		}
		case SatLooped:
		{
			if (animation->frameCurrent < animation->spritesLength - 1)
				(animation->frameCurrent)++;
			else if (animation->frameCurrent == animation->spritesLength - 1)
				animation->frameCurrent = 0;
			else
			{
				// error case.
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SatLooped: frame_current %d\t\n", animation->frameCurrent);
#endif
			}
			break;
		}
		case SatPingPong:
		{
			// if the pingpong is on the way "right" and not at the last frame yet,
			// or it is on the way "left" and not at the first frame yet;
			if (
				(!animation->pingPongReverse && (animation->frameCurrent < animation->spritesLength - 1))
				||
				(animation->pingPongReverse && (animation->frameCurrent > 0))
			)
			{
				animation->frameCurrent += (animation->pingPongReverse) ? -1 : 1;
			}
			// else if we are at the extreme ends and its time to turn the ping pong around
			else if (
				(!animation->pingPongReverse && (animation->frameCurrent >= animation->spritesLength - 1))
				||
				(animation->pingPongReverse && (animation->frameCurrent <= 0))
			)
			{
				animation->pingPongReverse = !animation->pingPongReverse;
				animation->frameCurrent += (animation->pingPongReverse) ? -1 : 1;
			}
			else
			{
				// error case
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SatPingPong: frame_current %d\tping_pong_reverse %d\n", animation->frameCurrent, animation->pingPongReverse);
#endif
			}
			break;
		}
		case SatPingPongOnce:
		{
			if (!(animation->pingPongReverse) && animation->frameCurrent < animation->spritesLength - 1)
				(animation->frameCurrent)++;
			else if (!(animation->pingPongReverse) && animation->frameCurrent >= animation->spritesLength - 1)
			{
				animation->pingPongReverse = true;
				(animation->frameCurrent)--;
			}
			else if ((animation->pingPongReverse) && animation->frameCurrent > 0)
				(animation->frameCurrent)--;
			else if (animation->pingPongReverse && animation->frameCurrent <= 0)
				animation->playing = false;
			else
			{
				// error case
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SatPingPongOnce: frame_current %d\tping_pong_reverse %d\n", animation->frameCurrent, animation->pingPongReverse);
#endif
			}
			break;
		}
		case SatLoopedN:
		{
			if (animation->frameCurrent < animation->spritesLength - 1)
				(animation->frameCurrent)++;
			else if (animation->frameCurrent == animation->spritesLength - 1)
			{
				animation->frameCurrent = 0;
				newLoop(animation);
			}
			else
			{
				// error case.
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SatLooped: frame_current %d\t\n", animation->frameCurrent);
#endif
			}
			break;
		}
		case SatPingPongN:
		{
			if (
				(!animation->pingPongReverse && (animation->frameCurrent < animation->spritesLength - 1))
				||
				(animation->pingPongReverse && (animation->frameCurrent > 0))
			)
			{
				animation->frameCurrent += (animation->pingPongReverse) ? -1 : 1;
			}
			else if (
				(!animation->pingPongReverse && (animation->frameCurrent >= animation->spritesLength - 1))
				||
				(animation->pingPongReverse && (animation->frameCurrent <= 0))
			)
			{
				animation->pingPongReverse = !animation->pingPongReverse;
				animation->frameCurrent += (animation->pingPongReverse) ? -1 : 1;

				if (!animation->pingPongReverse)
					newLoop(animation);
			}
			else
			{
				// error case
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SatPingPong: frame_current %d\tping_pong_reverse %d\n", animation->frameCurrent, animation->pingPongReverse);
#endif
			}
			break;
		}
	}
}

static void animationUpdatePrivates(spriteAnimation* animation)
{
	animation->secondsOnFrame = 0;
	if (animation->framesPerSecond != 0)
		animation->secondsPerFrame = 1.0f / (float)(animation->framesPerSecond);
	animation->frameCurrent = 0;
}

static void newLoop(spriteAnimation* animation)
{
	if (animation->animationType == SatLoopedN ||
		animation->animationType == SatPingPongN)
	{
		animation->loopsLeft--;
		if (animation->loopsLeft <= 0)
		{
			animation->playing = false;
			animation->frameCurrent = 0;
		}
	}	
}

/// REAL FUNCTIONS ///

void drawSpriteAnimationBase(float frametime, spriteAnimation* animation)
{
	if (animation->playing)
		addTime(animation, frametime);
}

spriteAnimation newAnimationFromSprites(int spritesLength, sprite sprites[], int framesPerSecond)
{
	spriteAnimation animation;
	animation.baseSprite = sprites[0];
	animation.spritesLength = spritesLength;
	animation.framesPerSecond = framesPerSecond;
	animation.loopCount = 1;
	animation.sourcePositions = (Vector2*)malloc(sizeof(Vector2) * spritesLength);
	for(int i = 0; i < spritesLength; i++)
		animation.sourcePositions[i] = (Vector2){sprites[i].source.x, sprites[i].source.y};

	// unspecified defaults
	animation.animationType = SatLooped;
	animation.pingPongReverse = false;
	animation.playing = true;
	animation.loopsLeft = 0;

	// compute private fields
	animationUpdatePrivates(&animation);
	
	return animation;
}

spriteAnimation newAnimation(int spritesLength, sprite baseSprite, Vector2 sourcePositions[], int framesPerSecond)
{
	spriteAnimation animation;
	animation.baseSprite = baseSprite;
	animation.spritesLength = spritesLength;
	animation.framesPerSecond = framesPerSecond;
	animation.loopCount = 1;
	animation.sourcePositions = (Vector2*)malloc(sizeof(Vector2) * spritesLength);
	for(int i = 0; i < spritesLength; i++)
		animation.sourcePositions[i] = sourcePositions[i];

	// unspecified defaults
	animation.animationType = SatLooped;
	animation.pingPongReverse = false;
	animation.playing = true;
	animation.loopsLeft = 0;

	// compute private fields
	animationUpdatePrivates(&animation);
	
	return animation;
}


spriteAnimation changeAnimationFramerate(spriteAnimation* animation, int framesPerSecond)
{
	animation->framesPerSecond = framesPerSecond;
	animationUpdatePrivates(animation);
	return *animation;
}

spriteAnimation setSpriteAnimationLoops(spriteAnimation* animation, int loopCount, bool usePingPong)
{
	if (usePingPong)
		animation->animationType = SatPingPongN;
	else
		animation->animationType = SatLoopedN;
	animation->loopCount = loopCount;
	animation->loopsLeft = animation->loopCount;
	return *animation;
}

void drawSpriteAnimation(float frametime, spriteAnimation* animation, int posX, int posY, Color tint)
{
	// not calling DrawSpriteBase since we are just preparing a call to DrawPro()

	sprite currentsprite = getCurrentSprite(*animation);
	Rectangle dest = {posX, posY, currentsprite.width, currentsprite.height};
	
	drawSpriteAnimationPro(frametime, animation, dest, getRectCenterRelative(dest), 0.0f, tint);
}

void drawSpriteAnimationV(float frametime, spriteAnimation* animation, Vector2 position, Color tint)
{
	// not calling DrawSpriteBase since we are just preparing a call to DrawPro()
	
	sprite currentsprite = getCurrentSprite(*animation);
	Rectangle dest = {position.x, position.y, currentsprite.width, currentsprite.height};
	
	drawSpriteAnimationPro(frametime, animation, dest, getRectCenterRelative(dest), 0.0f, tint);
}

void drawSpriteAnimationPro(float frametime, spriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	drawSpriteAnimationBase(frametime, animation);
	
	sprite currentsprite = getCurrentSprite(*animation);
	drawSpritePro(currentsprite, dest, origin, rotation, tint);
}

sprite getCurrentSprite(spriteAnimation animation)
{
	sprite currentsprite = animation.baseSprite;
	Vector2 pos = animation.sourcePositions[animation.frameCurrent];
	currentsprite.source.x = pos.x;
	currentsprite.source.y = pos.y;
	return currentsprite;
}

void spriteAnimationReset(spriteAnimation* animation)
{
	animation->frameCurrent = 0;
	animation->secondsOnFrame = false;
	animation->pingPongReverse = false;
	animation->playing = false;
	animation->loopsLeft = animation->loopCount;
}

void spriteAnimationStop(spriteAnimation* animation)
{
	animation->playing = false;
}

void spriteAnimationResume(spriteAnimation* animation)
{
	animation->playing = true;
}

void spriteAnimationStart(spriteAnimation* animation)
{
	spriteAnimationReset(animation);
	spriteAnimationResume(animation);
}