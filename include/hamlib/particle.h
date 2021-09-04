#ifndef PARTICLE_H_
#define PARTICLE_H_

typedef enum ColorAnimation
{
	STATIC, RAINBOW, TINT, FADE
} ColorAnimation;

typedef struct Particle
{
	Vector2 pos;
	Vector2 vel;
	Color color;
	ColorAnimation coloranim;
} Particle;

#endif