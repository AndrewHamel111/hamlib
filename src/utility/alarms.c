#include "hamlib/alarm_def.h"
#include "hamlib/alarm_func.h"
#include "hamlib/alarm_registry.h"
#define REPEATING_ALARM_CARRIES_TIME

#include <stdlib.h>

void seta(alarm* _alarm)
{
	_alarm->flag = true;
	_alarm->time = _alarm->time_initial;
	if ((_alarm->onSet) != NULL)
		(*(_alarm->onSet))();
}

void setaonce(alarm* _alarm)
{
	if (!(_alarm->flag))
		seta(_alarm);
}

void unseta(alarm* _alarm)
{
	if (!(_alarm->flag)) return;

	_alarm->flag = false;
	_alarm->time = 0.0f;
}

void skipa(alarm* _alarm)
{
	if (!(_alarm->flag)) return;

	unseta(_alarm);
	if (_alarm->onEnd != NULL)
		(*(_alarm->onEnd))();
}

void ticka(alarm* _alarm, float frametime)
{
	if (!(_alarm->flag)) return;

	_alarm->time -= frametime;

	if ((_alarm->time <= 0.0f))
	{

		float temp = _alarm->time; // save the extra negative time to add to the alarm's timer for the next loop IF the alarm loops.
		skipa(_alarm);
		
		if (_alarm->repeats)
		{
			seta(_alarm);
#ifdef REPEATING_ALARM_CARRIES_TIME
			_alarm->time -= temp;
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
	alarm.time = alarm.time_initial = time;

	return alarm;
}

////////////////////
// alarm registry //
/////.////.///./////
alarm** registered_alarms;
unsigned short alarms_length;

void registera(alarm* a)
{
	if (alarms_length == 0)
	{
		registered_alarms = (alarm**)malloc(sizeof(alarm*));
	}
	else
	{
		alarms_length++;
		registered_alarms = (alarm**)realloc(registered_alarms, alarms_length * sizeof(alarm*));
	}

	registered_alarms[alarms_length] = a;
	alarms_length++;
}

void unregistera(alarm* a)
{
	if ((alarms_length == 1) && (registered_alarms[0] == a))
	{
		unregisterall();
		return;
	}
	
	int aindex = -1;
	for(int i = 0; (aindex == -1) && (i < alarms_length); i++)
	{
		if (registered_alarms[i] == a)
		{
			aindex = i;
		}
	}

	if (aindex == -1) return;

	for(int i = aindex; i < (alarms_length - 1); i++)
		registered_alarms[i] = registered_alarms[i + 1];

	alarms_length--;
	registered_alarms = (alarm**)realloc(registered_alarms, alarms_length * sizeof(alarm*));
}

void unregisterall(void)
{
	free(registered_alarms);
	alarms_length = 0;
}


void tickall(float frametime)
{
	for (int i = 0; i < alarms_length; i++)
	{
		ticka(registered_alarms[i], frametime);
	}
}