#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "raylib.h"

typedef enum particle_anim_info
{
	STATIC = 1, TINT = 2, FADE = 4, SHRINK = 8, GROW = 16, ACCELERATE = 32, DECELERATE = 64, FADE_IMMEDIATE = 128, CUSTOM_ACC = 256, TINT_TO = 512
} particle_anim_info;

typedef struct Particle
{
	Vector2 pos; // current position
	float rotation; // current rotation in degrees. 0 degrees == RIGHT, 90 degrees == DOWN, etc.
	float size; // diameter of particle
	Vector2 vel; // displacement in pixels per second
	Vector2 custom_acc;
	float rotation_per_second; // rotation in degrees per second
	float life; // seconds left of particle
	float life_initial; // total lifespan of particle
	particle_anim_info anim;
	float anim_factor; // modifier on anim operation (i.e. faster fade, faster decel, slower grow, etc)
	Color color;
	Color color_initial;
	Color color_final;
	enum display_info {
		PIXEL, SHAPE, SPRITE
	} display_type;
	union
	{
		struct display_info_shape // shape
		{
			enum shape_info
			{
				RECT, CIRCLE, TRIANGLE
			} shape_type;
			enum shape_display_info
			{
				SOLID, HOLLOW
			} shape_display_type;
		} shape;

		struct display_info_sprite // sprite
		{
			Vector2 size;
			Texture2D sprite;
		} sprite;
	};
} Particle;

#endif