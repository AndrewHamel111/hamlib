#ifndef HAMLIB_IMGUI_H
#define HAMLIB_IMGUI_H

#include "raylib.h"
#include <stdarg.h>

/**
 * \brief An immediate GUI function for a mouse-based button.
 * 
 * \param rect Rectangle describing the button
 * \param color Color the button should be drawn in.
 * \return true when the mouse is highlighting the button.
 */
bool button(Rectangle rect, Color color);

/**
 * \brief An immediate GUI function for a mouse-based button.
 * 
 * \param rect Rectangle describing the button
 * \param color Color the button should be drawn in. When highlighted this color is also tinted.
 * \return true when the mouse is highlighting the button.
 */
bool buttonTintable(Rectangle rect, Color color);

/**
 * \brief Fills the rectangle horizontally from left to right, first drawing the bg color over the whole rect, then the fg color part way (based on percent) and then drawing an outline over the whole thing.
 */
void progressBar(Rectangle rectangle, Color bg, Color fg, Color outline, float percent);

/**
 * \brief Like progressBar, but allows to send 3 fg's with 2 additional percents at which the other colors should be shown. I.e.. progressBarTintable(rect, bg, GREEN, outline, percent, YELLOW, 0.4f, RED, 0.15f) would be a lot like the pokemon health bars.
 */
void progressBarTintable(Rectangle rectangle, Color bg, Color fg, Color outline, float percent, Color colorWarning, float percentWarning, Color colorCritical, float percentCritical);

/**
 * \brief NOT YET IMPLEMENTED Same as progress bar, but offers the ability to provide percents in descending order at which the tint should change from one value to another. Requires the use of stdarg. but should be fine!
 * \param rectangle 
 * \param bg 
 * \param outline 
 * \param percent 
 * \param interval_count The number of intervals (pairs) specified in the function
 * \param ... 
 */
// void ProgressBarTintablePro(Rectangle rectangle, Color bg, Color outline, float percent, int interval_count, ...);

#endif