#ifndef ALARM_REG_H
#define ALARM_REG_H

#include "hamlib/alarm_def.h"

void registera(alarm* a);
void unregistera(alarm* a);
void unregisterall(void);

void tickall(float frametime);

#endif