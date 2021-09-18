#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "raylib.h"

typedef struct animator
{
	animation* animations;
	unsigned char animations_count;
	unsigned char animation_current;
	bool is_playing;
} animator;

typedef enum animation_info
{
	SPRITE, TRANSFORM, FIELD
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
	animation_info info;
	animation_play_type play_type;
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
			animation_field_info info;
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

#endif