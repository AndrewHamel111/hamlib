#include "hamlib.h"

#include <stdlib.h>
#include "easings.h"

const int maxTweens = 30;

typedef struct tween
{
	// TweenInt, TweenFloat, TweenVector
	enum { TweenInt, TweenFloat, TweenVector } type;

	// current progress through the tween
	float time;
	// total duration of the tween
	float duration;
	
	float (*easingFunction)(float,float,float,float);
	
	bool active;

	union 
	{
		struct 
		{
			int* intValue;
			int intStart;
			int intEnd;
		};
		struct 
		{
			float* floatValue;
			float floatStart;
			float floatEnd;
		};
		struct 
		{
			Vector2* vectorValue;
			Vector2 vectorStart;
			Vector2 vectorEnd;
		};
	};
} tween;

static tween *tweens = {0};
static bool needsInit = true;
static float (*nextTweenEasingMethodPtr)(float,float,float,float) = &EaseLinearNone;

static void freeInnerTweenValues(tween* t);
static void tweenInit(void);
static void skipTweenToEnd(tween* t);

static void freeInnerTweenValues(tween* t)
{
	if (t == NULL) return;
	
	(t)->easingFunction = NULL;
	if (t->type == TweenInt)
		t->intValue = NULL;
	else if (t->type == TweenFloat)
		t->floatValue = NULL;
	else
		t->vectorValue = NULL;
}

static void tweenInit(void)
{
	tweens = (tween*)malloc(sizeof(tween) * maxTweens);
	for(int i = 0; i < maxTweens; i++)
	{
		tweens[i] = (tween){0};
		tweens[i].active = false;
		tweens[i].easingFunction = &EaseLinearNone;
	}
	needsInit = false;
}

void tweenUpdate(float frametime)
{
	if (needsInit)
	{
		return;
	}
	
	for(int i = 0; i < maxTweens; i++)
	{
		if (!tweens[i].active) continue; // skip NULL ptr

		tween* t = tweens + i;
		t->time += frametime;
		float easeT = CLAMP(t->time, 0, t->duration);
		if (t->type == TweenInt)
		{
			*(t->intValue) = (int)((*(t->easingFunction))(easeT, (float)(t->intStart), (float)(t->intEnd - t->intStart), t->duration));
		}
		else if (t->type == TweenFloat)
		{
			*(t->floatValue) = (*(t->easingFunction))(easeT, t->floatStart, t->floatEnd - t->floatStart, t->duration);
		}
		else if (t->type == TweenVector)
		{
			t->vectorValue->x = (*(t->easingFunction))(easeT, (float)(t->vectorStart.x), (float)(t->vectorEnd.x - t->vectorStart.x), t->duration);
			t->vectorValue->y = (*(t->easingFunction))(easeT, (float)(t->vectorStart.y), (float)(t->vectorEnd.y - t->vectorStart.y), t->duration);
		}

		if (t->time > t->duration)
		{
			tweens[i].active = false;
		}
	}
}

static void skipTweenToEnd(tween* t)
{
	if (!t->active) return;
	
	switch(t->type)
	{
		case TweenInt:
			*(t->intValue) = t->intEnd;
			break;
		case TweenFloat:
			*(t->floatValue) = t->floatEnd;
			break;
		case TweenVector:
			*(t->vectorValue) = t->vectorEnd;
			break;
	}
}

void tweenClear(void)
{
	if (needsInit)
	{
		return;
	}
	
	for(int i = 0; i < maxTweens; i++)
	{
		if (tweens[i].active)
			skipTweenToEnd(tweens + i);
		tweens[i] = (tween){0};
	}
	
	free(tweens);
}

void setNextTweenEasingMethod(float (*easingMethod)(float,float,float,float))
{
	if (easingMethod == NULL)
		easingMethod = &EaseLinearNone;
	
	nextTweenEasingMethodPtr = easingMethod;
}

void tweenInt(int* value, int start, int end, float time)
{
	if (needsInit)
	{
		tweenInit();
	}
	
	tween t = {0};
	t.type = TweenInt;
	t.active = true;
	t.duration = time;
	t.intStart = start;
	t.intEnd = end;
	t.intValue = value;
	t.easingFunction = nextTweenEasingMethodPtr;
	
	int i = 0;
	for(; (i < maxTweens) && tweens[i].active; i++); // when loops ends we will either have a valid index or be SOL
	
	if (i >= maxTweens) return;
	
	tweens[i] = t;
}

void tweenFloat(float* value, float start, float end, float time)
{
	if (needsInit)
	{
		tweenInit();
	}
	
	tween t = {0};
	t.type = TweenFloat;
	t.active = true;
	t.duration = time;
	t.floatStart = start;
	t.floatEnd = end;
	t.floatValue = value;
	t.easingFunction = nextTweenEasingMethodPtr;
	
	int i = 0;
	for(; (i < maxTweens) && tweens[i].active; i++); // when loops ends we will either have a valid index or be SOL
	
	if (i >= maxTweens) return;
	
	tweens[i] = t;
}

void tweenVector(Vector2* value, Vector2 start, Vector2 end, float time)
{
	if (needsInit)
	{
		tweenInit();
	}
	
	tween t = {0};
	t.type = TweenVector;
	t.active = true;
	t.duration = time;
	t.vectorStart = start;
	t.vectorEnd = end;
	t.vectorValue = value;
	t.easingFunction = nextTweenEasingMethodPtr;

	int i = 0;
	for(; (i < maxTweens) && tweens[i].active; i++); // when loops ends we will either have a valid index or be SOL

	if (i >= maxTweens) return;

	tweens[i] = t;
}