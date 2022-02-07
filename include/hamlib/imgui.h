#ifndef HAMLIB_IMGUI_H
#define HAMLIB_IMGUI_H

#include "raylib.h"

/**
 * \brief An immediate GUI function for a mouse-based button.
 * 
 * \param rect Rectangle describing the button
 * \param color Color the button should be drawn in.
 * \return true when the mouse is highlighting the button.
 */
bool Button(Rectangle rect, Color color);

/**
 * \brief An immediate GUI function for a mouse-based button.
 * 
 * \param rect Rectangle describing the button
 * \param color Color the button should be drawn in. When highlighted this color is also tinted.
 * \return true when the mouse is highlighting the button.
 */
bool ButtonTintable(Rectangle rect, Color color);

#endif