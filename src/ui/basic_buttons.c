/**
 * \file basic_buttons.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"
#include "hamlib/imgui.h"

bool Button(Rectangle rect, Color color)
{
	DrawRectangleRec(rect, color);
	return CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool ButtonTintable(Rectangle rect, Color color)
{
	bool flag = CheckCollisionPointRec(GetMousePosition(), rect);
	DrawRectangleRec(rect, flag ? tintcolor(color, 0.6f) : color);
	return flag && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}