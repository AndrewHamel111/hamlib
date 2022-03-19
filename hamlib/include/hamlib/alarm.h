#ifndef HAMLIB_ALARM_H
#define HAMLIB_ALARM_H

#if defined(__STDC__) && __STDC_VERSION__ >= 199901L
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false, true } bool;
    #define RL_BOOL_TYPE
#endif

/**
 * \brief alarm struct
 * 
 */
typedef struct alarm
{
	float time;
	float timeInitial;
	bool flag;
	bool repeats;
	void (*onSet)(void);
	void (*onEnd)(void);
} alarm;

/**
 * \brief set the alarm and trigger onSet
 */
void seta(alarm*);

/**
 * \brief set the given alarm only if it's not already set
 */
void setaonce(alarm*);

/**
 * \brief Unset the alarm
 */
void unseta(alarm*);

/**
 * \brief Unset the alarm and trigger onEnd
 */
void skipa(alarm*);

/**
 * \brief Advance the alarm timer and trigger onEnd if the alarm has lapsed.
 */
void ticka(alarm*, float);

/**
 * \brief Initializes all values for an alarm to save values. Must manually set onSet onEnd functions after.
 */
alarm createalarm(float time, bool repeats);

/// REGISTRY ///
#ifndef ALARM_REGISTRY
void registera(alarm* a);
void unregistera(alarm* a);
void unregisterall(void);

void tickall(float frametime);
#endif

#endif