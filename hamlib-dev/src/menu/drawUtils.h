#ifndef HAMLIB_DEV_DRAWUTILS_H
#define HAMLIB_DEV_DRAWUTILS_H

#include "raylib.h"

#include "menu.h"

Color getHighlightColor(Color basecolor);
Color getPressedColor(Color basecolor);

void fitDataForSlider(elementDrawData* data);
void fitDataForTextfield(elementDrawData* data);

#endif