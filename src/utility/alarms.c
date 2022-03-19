#include "hamlib/alarm.h"
#define REPEATING_ALARM_CARRIES_TIME

#include <stdlib.h>

void seta(alarm* a)
{
	a->flag = true;
	a->time = a->timeInitial;
	if ((a->onSet) != NULL)
		(*(a->onSet))();
}

void setaonce(alarm* a)
{
	if (!(a->flag))
		seta(a);
}

void unseta(alarm* a)
{
	if (!(a->flag)) return;
	
	a->flag = false;
	a->time = 0.0f;
}

void skipa(alarm* a)
{
	if (!(a->flag)) return;

	unseta(a);
	if (a->onEnd != NULL)
		(*(a->onEnd))();
}

void ticka(alarm* a, float frametime)
{
	if (!(a->flag)) return;
	
	a->time -= frametime;

	if ((a->time <= 0.0f))
	{

		float temp = a->time; // save the extra negative time to add to the alarm's timer for the next loop IF the alarm loops.
		skipa(a);
		
		if (a->repeats)
		{
			seta(a);
#ifdef REPEATING_ALARM_CARRIES_TIME
			a->time -= temp;
#endif
		}
	}
}

alarm createalarm(float time, bool repeats)
{
	alarm alarm;
	alarm.flag = false;
	alarm.onEnd = alarm.onSet = NULL;
	alarm.repeats = repeats;
	alarm.time = alarm.timeInitial = time;

	return alarm;
}

////////////////////
// alarm registry //
/////.////.///./////
alarm** registeredAlarms;
unsigned short alarmsLength;

void registera(alarm* a)
{
	if (alarmsLength == 0)
	{
		registeredAlarms = (alarm**)malloc(sizeof(alarm*));
	}
	else
	{
		alarmsLength++;
		registeredAlarms = (alarm**)realloc(registeredAlarms, alarmsLength * sizeof(alarm*));
	}
	
	registeredAlarms[alarmsLength] = a;
	alarmsLength++;
}

void unregistera(alarm* a)
{
	if ((alarmsLength == 1) && (registeredAlarms[0] == a))
	{
		unregisterall();
		return;
	}
	
	int aindex = -1;
	for(int i = 0; (aindex == -1) && (i < alarmsLength); i++)
	{
		if (registeredAlarms[i] == a)
		{
			aindex = i;
		}
	}

	if (aindex == -1) return;

	for(int i = aindex; i < (alarmsLength - 1); i++)
		registeredAlarms[i] = registeredAlarms[i + 1];

	alarmsLength--;
	registeredAlarms = (alarm**)realloc(registeredAlarms, alarmsLength * sizeof(alarm*));
}

void unregisterall(void)
{
	free(registeredAlarms);
	alarmsLength = 0;
}


void tickall(float frametime)
{
	for (int i = 0; i < alarmsLength; i++)
	{
		ticka(registeredAlarms[i], frametime);
	}
}