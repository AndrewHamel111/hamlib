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

#include "raylib.h"
#include "hamlib.h"
#include "hamlib/particle.h"

extern particle* particles[];
extern unsigned short particlesLength;

static const Vector2 particleDecel = (Vector2){10.0f, 10.0f};
static const Vector2 particleAccel = (Vector2){10.0f, 10.0f};
static const float particleShrink = 2.0f;
static const float particleGrow = 4.0f;

static void destroyParticle(particle** ptr)
{
	free(*ptr);
	*ptr = NULL;
}

void destroyAllParticles(void)
{
	for(int i = 0; i < particlesLength; i++)
		if (particles[i] != NULL)
			destroyParticle(particles + i);
}

void createParticle(particle p)
{
	int index = -1;
	float minlife = 100.0f;
	int minlifeIndex = -1;
	for(int i = 0; (i < particlesLength) && (index == -1); i++)
	{
		if (particles[i] == NULL)
			index = i;
		else
		{
			if (particles[i]->life < minlife)
			{
				minlife = particles[i]->life;
				minlifeIndex = i;
			}
		}
	}

	// can't find an empty space
	if (index == -1)
	{
		// destroy the particle closest to death
		destroyParticle(particles + minlifeIndex);
		index = minlifeIndex;
	}
	
	particles[index] = (particle*)malloc(sizeof(particle));
	*particles[index] = p;
}

static void updateParticle(particle** ptr, float frametime)
{
	particle* p = *ptr;
	p->life -= frametime;
	if (p->life < 0)
	{
		destroyParticle(ptr);
		return;
	}
	if (p->anim == PaiStatic) return;

	p->pos.x += (p->vel.x * frametime);
	p->pos.y += (p->vel.y * frametime);
	p->rotation += (p->rotationPerSecond * frametime);

	if ((p->anim & PaiTint) == PaiTint)
	{
		p->color = tintcolor(p->colorInitial, 1.0f - (p->life / p->lifeInitial));
	}
	else if ((p->anim & PaiTintTo) == PaiTintTo)
	{
		p->color = tintcolor(p->colorInitial, 1.0f - (p->life / p->lifeInitial));
	}

	if ((p->anim & PaiFade) == PaiFade)
	{
		p->color = Fade(p->colorInitial, 1.0f - CLAMP(p->life, 0.0f, 1.0f));
		// note: use this instead for a really sick effect
		// p->color = Fade(p->colorInitial, 1.0f - CLAMP(p->life, 0.0f, 1.0f));
	}
	else if ((p->anim & PaiFadeImmediate) == PaiFadeImmediate)
	{
		p->color = Fade(p->colorInitial, 1.0f - (p->life / p->lifeInitial));
	}

	if ((p->anim & PaiShrink) == PaiShrink)
	{
		reducebyf(&(p->size), particleShrink * frametime * p->animFactor);
	}
	else if ((p->anim & PaiGrow) == PaiGrow)
	{
		increasebyf(&(p->size), particleGrow * frametime * p->animFactor);
	}

	if ((p->anim & PaiDecelerate) == PaiDecelerate)
	{
		reducebyf(&(p->vel.x), particleDecel.x * frametime * p->animFactor);
		reducebyf(&(p->vel.y), particleDecel.y * frametime * p->animFactor);
	}
	else if ((p->anim & PaiAccelerate) == PaiAccelerate)
	{
		increasebyf(&(p->vel.x), particleAccel.x * frametime * p->animFactor);
		increasebyf(&(p->vel.y), particleAccel.y * frametime * p->animFactor);
	}
	else if ((p->anim & PaiCustomAcc) == PaiCustomAcc)
	{
		if (p->customAcc.x < 0)
			reducebyf(&(p->vel.x), p->customAcc.x * frametime * p->animFactor);
		else
			increasebyf(&(p->vel.x), p->customAcc.x * frametime * p->animFactor);
		if (p->customAcc.y < 0)
			reducebyf(&(p->vel.y), p->customAcc.y * frametime * p->animFactor);
		else
			increasebyf(&(p->vel.y), p->customAcc.y * frametime * p->animFactor);
	}
}

void updateParticles(float frametime)
{
	for(int i = 0; i < particlesLength; i++)
		if ((particles[i]) != NULL)
			updateParticle(particles + i, frametime);
}

static void drawParticle(particle p)
{
	if (p.displayType == PdiPixel)
	{
		DrawPixelV(p.pos, p.color);
		//DrawRectangleV(p.pos, (Vector2){8,8}, p.pixel.color);
	}
	else if (p.displayType == PdiShape)
	{
		if (p.particleShape.shapeType == PsiRect)
		{
			if (p.particleShape.shapeDisplayType == PsdiSolid)
				DrawPoly(p.pos, 4, p.size, p.rotation, p.color);
			else
				DrawPolyLines(p.pos, 4, p.size, p.rotation, p.color);
		}
		else if (p.particleShape.shapeType == PsiCircle)
		{
			if (p.particleShape.shapeDisplayType == PsdiSolid)
				DrawCircleV(p.pos, p.size, p.color);
			else
				DrawCircleLines(p.pos.x, p.pos.y, p.size, p.color);
		}
		else if (p.particleShape.shapeType == PsiTriangle)
		{
			if (p.particleShape.shapeDisplayType == PsdiSolid)
				DrawPoly(p.pos, 3, p.size, p.rotation, p.color);
			else
				DrawPolyLines(p.pos, 3, p.size, p.rotation, p.color);
		}
	}
	else if (p.displayType == PdiPixel)
	{
		Rectangle r = (Rectangle){p.pos.x, p.pos.y, p.size, p.size};
		DrawTexturePro(p.particleSprite.sprite, pureSource(p.particleSprite.sprite), r, getRectCenterRelative(r), p.rotation, p.color);
	}
}

void drawParticles(void)
{
	for(int i = 0; i < particlesLength; i++)
		if (particles[i] != NULL)
			drawParticle(*(particles[i]));
}
