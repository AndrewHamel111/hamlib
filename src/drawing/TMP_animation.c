#include "hamlib/TMP_animation.h"
#include "hamlib.h"

#include <stdlib.h>

//debug
//#define PRINT_DEBUG

#ifdef PRINT_DEBUG
	#include <stdio.h>
#endif

/// INTERNAL FUNCTIONS ///

void AddTime(SpriteAnimation* animation, float frametime);
void AdvanceFrame(SpriteAnimation* animation);
void AnimationUpdatePrivates(SpriteAnimation* animation);
void DrawSpriteAnimationBase(SpriteAnimation* animation, float frametime);

void AddTime(SpriteAnimation* animation, float frametime)
{
	animation->_seconds_on_frame += frametime;
	while (animation->_seconds_on_frame >= animation->_seconds_per_frame)
	{
		animation->_seconds_on_frame -= animation->_seconds_per_frame;
		AdvanceFrame(animation);

		// account for floating point errors and weirdness
		if (animation->_seconds_on_frame < 0)
			animation->_seconds_on_frame = 0;
	}
}

void AdvanceFrame(SpriteAnimation* animation)
{
	switch(animation->animation_type)
	{
		case SAT_ONCE:
		{
			if (animation->_frame_current < animation->sprites_length - 1)
				(animation->_frame_current)++;
			else if (animation->_frame_current >= animation->sprites_length - 1)
			{
				animation->_playing = false;
			}
			else
			{
				// error case.
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SAT_ONCE: frame_current %d\n", animation->_frame_current);
#endif
			}
			break;	
		}
		case SAT_LOOPED:
		{
			if (animation->_frame_current < animation->sprites_length - 1)
				(animation->_frame_current)++;
			else if (animation->_frame_current == animation->sprites_length - 1)
				animation->_frame_current = 0;
			else
			{
				// error case.
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SAT_LOOPED: frame_current %d\t\n", animation->_frame_current);
#endif
			}
			break;
		}
		case SAT_PING_PONG:
		{
			// if the pingpong is on the way "right" and not at the last frame yet,
			// or it is on the way "left" and not at the first frame yet;
			if (
				(!animation->_ping_pong_reverse && (animation->_frame_current < animation->sprites_length - 1))
				||
				(animation->_ping_pong_reverse && (animation->_frame_current > 0))
			)
			{
				animation->_frame_current += (animation->_ping_pong_reverse) ? -1: 1;
			}
			// else if we are at the extreme ends and its time to turn the ping pong around
			else if (
				(!animation->_ping_pong_reverse && (animation->_frame_current >= animation->sprites_length - 1))
				||
				(animation->_ping_pong_reverse && (animation->_frame_current <= 0))
			)
			{
				animation->_ping_pong_reverse = !animation->_ping_pong_reverse;
				animation->_frame_current += (animation->_ping_pong_reverse) ? -1: 1;
			}
			else
			{
				// error case
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SAT_PING_PONG: frame_current %d\tping_pong_reverse %d\n", animation->_frame_current, animation->_ping_pong_reverse);
#endif
			}
			break;
		}
		case SAT_PING_PONG_ONCE:
		{
			if (!(animation->_ping_pong_reverse) && animation->_frame_current < animation->sprites_length - 1)
				(animation->_frame_current)++;
			else if (!(animation->_ping_pong_reverse) && animation->_frame_current >= animation->sprites_length - 1)
			{
				animation->_ping_pong_reverse = true;
				(animation->_frame_current)--;
			}
			else if ((animation->_ping_pong_reverse) && animation->_frame_current > 0)
				(animation->_frame_current)--;
			else if (animation->_ping_pong_reverse && animation->_frame_current <= 0)
				animation->_playing = false;
			else
			{
				// error case
#ifdef PRINT_DEBUG
				printf("ERR: Error case in SAT_PING_PONG_ONCE: frame_current %d\tping_pong_reverse %d\n", animation->_frame_current, animation->_ping_pong_reverse);
#endif
			}
			break;
		}
	}
}

void AnimationUpdatePrivates(SpriteAnimation* animation)
{
	animation->_seconds_on_frame = 0;
	if (animation->frames_per_second != 0)
		animation->_seconds_per_frame = 1.0f/animation->frames_per_second;
	animation->_frame_current = 0;
}

/**
 * \brief Every call to DrawSpriteAnimation__ must perform these actions first, to maintain the animation state.
 */
void DrawSpriteAnimationBase(SpriteAnimation* animation, float frametime)
{
	if (animation->_playing)
		AddTime(animation, frametime);
}

/// REAL FUNCTIONS ///

SpriteAnimation NewAnimation(int sprites_length, Sprite sprites[], int frames_per_second)
{
	SpriteAnimation animation;
	animation.sprites = (Sprite*)malloc(sprites_length * sizeof(Sprite));
	animation.sprites_length = sprites_length;
	animation.frames_per_second = frames_per_second;
	for(int i = 0; i < sprites_length; i++)
		animation.sprites[i] = sprites[i];

	// unspecified defaults
	animation.animation_type = SAT_LOOPED;
	animation._ping_pong_reverse = false;
	animation._playing = true;

	// compute private fields
	AnimationUpdatePrivates(&animation);
	
	return animation;
}

SpriteAnimation ChangeAnimationFramerate(SpriteAnimation* animation, int frames_per_second)
{
	animation->frames_per_second = frames_per_second;
	AnimationUpdatePrivates(animation);
	return *animation;
}

void DrawSpriteAnimation(float frametime, SpriteAnimation* animation, int posX, int posY, Color tint)
{
	// not calling DrawSpriteBase since we are just preparing a call to DrawPro()

	Sprite currentsprite = animation->sprites[animation->_frame_current];
	Rectangle dest = {posX, posY, currentsprite.width, currentsprite.height};
	
	DrawSpriteAnimationPro(frametime, animation, dest, GetCenterRelative(dest), 0.0f, tint);
}

void DrawSpriteAnimationV(float frametime, SpriteAnimation* animation, Vector2 position, Color tint)
{
	// not calling DrawSpriteBase since we are just preparing a call to DrawPro()
	
	Sprite currentsprite = animation->sprites[animation->_frame_current];
	Rectangle dest = {position.x, position.y, currentsprite.width, currentsprite.height};
	
	DrawSpriteAnimationPro(frametime, animation, dest, GetCenterRelative(dest), 0.0f, tint);
}

void DrawSpriteAnimationPro(float frametime, SpriteAnimation* animation, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	DrawSpriteAnimationBase(animation, frametime);
	
	DrawSpritePro(animation->sprites[animation->_frame_current], dest, origin, rotation, tint);
}

void SpriteAnimationReset(SpriteAnimation* animation)
{
	animation->_frame_current = 0;
	animation->_seconds_on_frame = false;
	animation->_ping_pong_reverse = false;
	animation->_playing = false;
}

void SpriteAnimationStop(SpriteAnimation* animation)
{
	animation->_playing = false;
}

void SpriteAnimationResume(SpriteAnimation* animation)
{
	animation->_playing = true;
}

void SpriteAnimationStart(SpriteAnimation* animation)
{
	SpriteAnimationReset(animation);
	SpriteAnimationResume(animation);
}