#ifndef ALARM_REG_H
#define ALARM_REG_H

#include "alarm_def.h"
#include "alarm_func.h"

void registera(alarm a);
void unregistera(alarm a);
void unregisterall();

void tickall(float frametime);

#endif