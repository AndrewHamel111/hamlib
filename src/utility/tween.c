#include "hamlib.h"

#define MAX_TWEENS 30

struct tween
{
	enum { INT, FLOAT, VECTOR } type;

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
};

static struct tween* tweens[MAX_TWEENS];

void tween_update(float frametime)
{

}

void tween_int(int* value, int start, int end, float time)
{
	// for(int i = 0; i < MAX_TWEENS; i++)

}

void tween_float(float* value, float start, float end, float time)
{

}

void tween_vector(Vector2* value, Vector2 start, Vector2 end, float time)
{

}