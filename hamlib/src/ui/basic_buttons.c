/**
 * \file basic_buttons.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"
#include "hamlib/imgui.h"

bool button(Rectangle rect, Color color)
{
	DrawRectangleRec(rect, color);
	return CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool buttonTintable(Rectangle rect, Color color)
{
	bool flag = CheckCollisionPointRec(GetMousePosition(), rect);
	DrawRectangleRec(rect, flag ? tintcolor(color, 0.6f) : color);
	return flag && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void progressBar(Rectangle rectangle, Color bg, Color fg, Color outline, float percent)
{
	DrawRectangleRec(rectangle, bg);
	DrawRectangle(rectangle.x, rectangle.y, rectangle.width * percent, rectangle.height, fg);
	DrawRectangleLinesEx(rectangle, rectangle.height/15, outline);
}

void progressBarTintable(Rectangle rectangle, Color bg, Color fg, Color outline, float percent, Color colorWarning, float percentWarning, Color colorCritical, float percentCritical)
{
	Color color = (percent <= percentCritical) ? colorCritical : (percent <= percentWarning) ? colorWarning : fg;
	progressBar(rectangle, bg, color, outline, percent);
}