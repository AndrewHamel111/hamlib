#ifndef ALARM_DEF_H
#define ALARM_DEF_H

#include <stdbool.h>

typedef struct alarm
{
	float time;
	float time_initial;
	bool flag;
	void (*onSet)(void);
	void (*onEnd)(void);
} alarm;

#endif