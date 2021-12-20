// TODO
/*
instead of using overloaded methods of every single draw function imaginable, we either
A. only make Scaled draws centered, and have a function that allows us to set a ScaledDrawFlag(SF_CENTERED) in program to stop or start centered drawing
OR
B. write some helpers that can move Vector2 position based on Vector2 size to be centered, or Rectangle rectangle to be centered by offsetting it's own rectangle.x, rectangle.y by -rectangle.width/2, -rectangle.height/2
*/

#ifndef CENTERED_DRAW_H
#define CENTERED_DRAW_H

#include "hamlib.h"

void DrawRectangleCentered(int posX, int posY, int width, int height, Color color);
void DrawRectangleCenteredV(Vector2 position, Vector2 size, Color color);
void DrawRectangleCenteredRec(Rectangle rectangle, Color color);

#endif