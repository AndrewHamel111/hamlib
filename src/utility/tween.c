#include "hamlib.h"

#include <stdlib.h>

#ifndef MAX_TWEENS
	#define MAX_TWEENS 30
#endif

typedef struct tween
{
	// TT_INT | TT_FLOAT | TT_VECTOR
	enum { TT_INT, TT_FLOAT, TT_VECTOR } type;

	// current progress through the tween
	float time;
	// total duration of the tween
	float duration;

	union 
	{
		struct 
		{
			int* int_value;
			int int_start;
			int int_end;
		};
		struct 
		{
			float* float_value;
			float float_start;
			float float_end;
		};
		struct 
		{
			Vector2* vector_value;
			Vector2 vector_start;
			Vector2 vector_end;
		};
	};
} tween;

static tween* tweens[MAX_TWEENS] = {0};

// TODO: actually implement these. the garbage that is here is just to silence warnings

void tween_update(float frametime)
{
	for(int i = 0; i < MAX_TWEENS; i++)
	{
		if (!tweens[i]) continue; // skip NULL ptr

		tween* t = tweens[i];
		t->time += frametime;
		if (t->type == TT_INT)
		{
			*(t->int_value) = LERP(t->int_start, t->int_end, CLAMP_NORMAL(t->time/t->duration));
		}
		else if (t->type == TT_FLOAT)
		{
			*(t->float_value) = LERP(t->float_start, t->float_end, CLAMP_NORMAL(t->time/t->duration));
		}
		else if (t->type == TT_VECTOR)
		{
			t->vector_value->x = LERP(t->vector_start.x, t->vector_end.x, CLAMP_NORMAL(t->time/t->duration));
			t->vector_value->y = LERP(t->vector_start.y, t->vector_end.y, CLAMP_NORMAL(t->time/t->duration));
		}

		if (t->time > t->duration)
		{
			free(tweens[i]);
		}
	}
}

void tween_int(int* value, int start, int end, float time)
{
	tween t = {0};
	t.type = TT_INT;
	t.duration = time;
	t.int_start = start;
	t.int_end = end;
	t.int_value = value;

	int i;
	for(i = 0; i < MAX_TWEENS && (!tweens[i]); i++); // when loops ends we will either have a valid index or be SOL

	if (i >= MAX_TWEENS) return;

	tweens[i] = (tween*)malloc(sizeof(tween));
	*(tweens[i]) = t;
}

void tween_float(float* value, float start, float end, float time)
{
	tween t = {0};
	t.type = TT_FLOAT;
	t.duration = time;
	t.float_start = start;
	t.float_end = end;
	t.float_value = value;

	int i;
	for(i = 0; i < MAX_TWEENS && (!tweens[i]); i++); // when loops ends we will either have a valid index or be SOL

	if (i >= MAX_TWEENS) return;

	tweens[i] = (tween*)malloc(sizeof(tween));
	*(tweens[i]) = t;
}

void tween_vector(Vector2* value, Vector2 start, Vector2 end, float time)
{
	tween t = {0};
	t.type = TT_VECTOR;
	t.duration = time;
	t.vector_start = start;
	t.vector_end = end;
	t.vector_value = value;

	int i;
	for(i = 0; i < MAX_TWEENS && (!tweens[i]); i++); // when loops ends we will either have a valid index or be SOL

	if (i >= MAX_TWEENS) return;

	tweens[i] = (tween*)malloc(sizeof(tween));
	*(tweens[i]) = t;
}