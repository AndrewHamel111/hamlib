#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "raylib.h"
#include "hamlib/alarm_registry.h"

typedef enum animation_info
{
	ANIM_SPRITE, ANIM_TRANSFORM, ANIM_FIELD
} animation_info;

typedef enum animation_field_info
{
	FLOAT, INT, VECTOR2
} animation_field_info;

typedef enum animation_play_type
{
	APT_ONESHOT, APT_LOOP, APT_PINGPONG
} animation_play_type;

typedef struct animation
{
	animation_info info; /** SPRITE, TRANSFORM, FIELD */
	animation_play_type play_type; /** APT_* */
	float animation_period;
	bool playing;
	float time;
	union 
	{
		struct
		{
			Texture2D spritesheet;
			unsigned short frame;
			unsigned short frames;
		};
		struct
		{
			// TODO implement
			Vector2* pos;
			Vector2* size;
			Vector2* rotation;
		};
		struct
		{
			// TODO implement
			animation_field_info field_info;
			union 
			{
				struct
				{
					float* targetf;
				};
				struct
				{
					int* targeti;
				};
				struct
				{
					Vector2* targetv;
				};
			};
			
		};
	};
	
} animation;

animation createspriteanimation(Texture2D spritesheet, unsigned short frames, float animation_period);
void playanimation(animation* anim);
void stopanimation(animation* anim);
void drawanimation(animation* anim, Vector2 pos, float frametime);
void drawanimationpro(animation* anim, Rectangle dest, Vector2 origin, float rotation, Color tint);

Rectangle animationrect(animation anim);

typedef struct animator
{
	animation* animations;
	unsigned char animations_count;
	unsigned char animation_current;
	bool is_playing;
} animator;

#endif