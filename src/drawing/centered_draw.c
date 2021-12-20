#include "hamlib/centered_draw.h"

void DrawRectangleCentered(int posX, int posY, int width, int height, Color color)
{
	DrawRectangleCenteredRec((Rectangle){posX, posY, width, height}, color);
}

void DrawRectangleCenteredV(Vector2 position, Vector2 size, Color color)
{
	DrawRectangleCenteredRec(RecFromVec(position, size), color);
}

void DrawRectangleCenteredRec(Rectangle rectangle, Color color)
{
	DrawRectangleRec(CenterRect(rectangle), color);
}