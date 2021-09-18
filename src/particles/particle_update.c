/**
 * \file particle_update.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-10
 * \brief Implemented here are the globally (to hamlib.h) accessible functions for particle handling. In the game's main.c, only Update and Draw should be addressed. The rest of the particle functions are included in other header files for the sake of simplicity.
 * 
 * @copyright Copyright (c) 2021
 */

// TODO implement RAINBOW color anim

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "hamlib.h"
#include "hamlib/particle.h"

extern Particle* particles[];
extern unsigned short particles_length;

static const Vector2 particle_decel = (Vector2){10.0f, 10.0f};
static const Vector2 particle_accel = (Vector2){10.0f, 10.0f};
static const float particle_shrink = 2.0f;
static const float particle_grow = 4.0f;

static void DestroyParticle(Particle** ptr)
{
	free(*ptr);
	*ptr = NULL;
}

void DestroyAllParticles(void)
{
	for(int i = 0; i < particles_length; i++)
		if (particles[i] != NULL)
			DestroyParticle(particles + i);
}

void CreateParticle(Particle p)
{
	int index = -1;
	float minlife = 100.0f;
	int minlife_index = -1;
	for(int i = 0; (i < particles_length) && (index == -1); i++)
	{
		if (particles[i] == NULL)
			index = i;
		else
		{
			if (particles[i]->life < minlife)
			{
				minlife = particles[i]->life;
				minlife_index = i;
			}
		}
	}

	// can't find an empty space
	if (index == -1)
	{
		// destroy the particle closest to death
		DestroyParticle(particles + minlife_index);
		index = minlife_index;
	}
	
	particles[index] = (Particle*)malloc(sizeof(Particle));
	*particles[index] = p;
}

static void UpdateParticle(Particle** ptr, float frametime)
{
	Particle* p = *ptr;
	p->life -= frametime;
	if (p->life < 0)
	{
		DestroyParticle(ptr);
		return;
	}
	if (p->anim == STATIC) return;

	p->pos.x += (p->vel.x * frametime);
	p->pos.y += (p->vel.y * frametime);
	p->rotation += (p->rotation_per_second * frametime);

	if ((p->anim & TINT) == TINT)
	{
		p->color = tintcolor(p->color_initial, 1.0f - (p->life / p->life_initial));
	}
	else if ((p->anim & TINT_TO) == TINT_TO)
	{
		p->color = tintcolor(p->color_initial, 1.0f - (p->life / p->life_initial));
	}

	if ((p->anim & FADE) == FADE)
	{
		p->color = fadecolor(p->color_initial, 1.0f - CLAMP(p->life, 0.0f, 1.0f));
		// note: use this instead for a really sick effect
		// p->color = fadecolor(p->color_initial, 1.0f - CLAMP(p->life, 0.0f, 1.0f));
	}
	else if ((p->anim & FADE_IMMEDIATE) == FADE_IMMEDIATE)
	{
		p->color = fadecolor(p->color_initial, 1.0f - (p->life / p->life_initial));
	}

	if ((p->anim & SHRINK) == SHRINK)
	{
		reducebyf(&(p->size), particle_shrink * frametime * p->anim_factor);
	}
	else if ((p->anim & GROW) == GROW)
	{
		increasebyf(&(p->size), particle_grow * frametime * p->anim_factor);
	}

	if ((p->anim & DECELERATE) == DECELERATE)
	{
		reducebyf(&(p->vel.x), particle_decel.x * frametime * p->anim_factor);
		reducebyf(&(p->vel.y), particle_decel.y * frametime * p->anim_factor);
	}
	else if ((p->anim & ACCELERATE) == ACCELERATE)
	{
		increasebyf(&(p->vel.x), particle_accel.x * frametime * p->anim_factor);
		increasebyf(&(p->vel.y), particle_accel.y * frametime * p->anim_factor);
	}
	else if ((p->anim & CUSTOM_ACC) == CUSTOM_ACC)
	{
		if (p->custom_acc.x < 0)
			reducebyf(&(p->vel.x), p->custom_acc.x * frametime * p->anim_factor);
		else
			increasebyf(&(p->vel.x), p->custom_acc.x * frametime * p->anim_factor);
		if (p->custom_acc.y < 0)
			reducebyf(&(p->vel.y), p->custom_acc.y * frametime * p->anim_factor);
		else
			increasebyf(&(p->vel.y), p->custom_acc.y * frametime * p->anim_factor);
	}
}

void UpdateParticles(float frametime)
{
	for(int i = 0; i < particles_length; i++)
		if ((particles[i]) != NULL)
			UpdateParticle(particles + i, frametime);
}

static void DrawParticle(Particle p)
{
	if (p.display_type == PIXEL)
	{
		DrawPixelV(p.pos, p.color);
		//DrawRectangleV(p.pos, (Vector2){8,8}, p.pixel.color);
	}
	else if (p.display_type == SHAPE)
	{
		if (p.shape.shape_type == RECT)
		{
			if (p.shape.shape_display_type == SOLID)
				DrawPoly(p.pos, 4, p.size, p.rotation, p.color);
			else
				DrawPolyLines(p.pos, 4, p.size, p.rotation, p.color);
		}
		else if (p.shape.shape_type == CIRCLE)
		{
			if (p.shape.shape_display_type == SOLID)
				DrawCircleV(p.pos, p.size, p.color);
			else
				DrawCircleLines(p.pos.x, p.pos.y, p.size, p.color);
		}
		else if (p.shape.shape_type == TRIANGLE)
		{
			if (p.shape.shape_display_type == SOLID)
				DrawPoly(p.pos, 3, p.size, p.rotation, p.color);
			else
				DrawPolyLines(p.pos, 3, p.size, p.rotation, p.color);
		}
	}
	else if (p.display_type == SPRITE)
	{
		Rectangle r = (Rectangle){p.pos.x, p.pos.y, p.size, p.size};
		DrawTexturePro(p.sprite.sprite, PureSource(p.sprite.sprite), r, GetCenterRelative(r), p.rotation, p.color);
	}
}

void DrawParticles(void)
{
	for(int i = 0; i < particles_length; i++)
		if (particles[i] != NULL)
			DrawParticle(*(particles[i]));
}
