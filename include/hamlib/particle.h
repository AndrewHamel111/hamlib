#ifndef HAMLIB_PARTICLE_H
#define HAMLIB_PARTICLE_H

#include "raylib.h"

// Possible Values: PaiStatic, PaiTint, PaiFade, PaiShrink, PaiGrow, PaiAccelerate, PaiDecelerate, PaiFadeImmediate, PaiCustomAcc, PaiTintTo
typedef enum particleAnimInfo
{
	PaiStatic = 1, PaiTint = 2, PaiFade = 4, PaiShrink = 8, PaiGrow = 16, PaiAccelerate = 32, PaiDecelerate = 64, PaiFadeImmediate = 128, PaiCustomAcc = 256, PaiTintTo = 512
} particleAnimInfo;

typedef struct particle
{
	Vector2 pos; // current position
	float rotation; // current rotation in degrees. 0 degrees == RIGHT, 90 degrees == DOWN, etc.
	float size; // diameter of particle
	Vector2 vel; // displacement in pixels per second
	Vector2 customAcc;
	float rotationPerSecond; // rotation in degrees per second
	float life; // seconds left of particle
	float lifeInitial; // total lifespan of particle
	particleAnimInfo anim;
	float animFactor; // modifier on anim operation (i.e. faster fade, faster decel, slower grow, etc)
	Color color;
	Color colorInitial;
	Color colorFinal;
	// Possible values: PdiPixel, PdiShape, PdiSprite
	enum displayInfo {
		PdiPixel, PdiShape, PdiSprite
	} displayType;
	union
	{
		struct displayInfoShape // particleShape
		{
			// Possible Values: PsiRect, PsiCircle, PsiTriangle
			enum shapeInfo
			{
				PsiRect = 0, PsiCircle = 1, PsiTriangle = 2
			} shapeType;
			// Possible Values: PsdiSolid, PsdiHollow
			enum shapeDisplayInfo
			{
				PsdiSolid = 0, PsdiHollow = 1
			} shapeDisplayType;
		} particleShape;

		struct displayInfoSprite // particleSprite
		{
			Vector2 size;
			Texture2D sprite;
		} particleSprite;
	};
} particle;

#endif