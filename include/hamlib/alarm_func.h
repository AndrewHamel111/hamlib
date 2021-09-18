#ifndef ALARM_FUNC_H
#define ALARM_FUNC_H

/**
 * \brief Set the alarm and trigger onSet
 */
void seta(alarm*);

/**
 * \brief Set the given alarm only if it's not already set
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

#endif