#ifndef CENTERED_DRAW_H
#define CENTERED_DRAW_H

#include "hamlib.h"

void DrawRectangleCentered(int posX, int posY, int width, int height, Color color);
void DrawRectangleCenteredV(Vector2 position, Vector2 size, Color color);
void DrawRectangleCenteredRec(Rectangle rectangle, Color color);

#endif