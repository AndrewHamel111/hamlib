/**
 * \file basic_buttons.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"

bool Button(Rectangle rect, Color color)
{
	DrawRectangleRec(rect, color);
	return VectorInRectangle(GetMousePosition(), rect);
}

bool ButtonTintable(Rectangle rect, Color color)
{
	bool flag = VectorInRectangle(GetMousePosition(), rect);
	DrawRectangleRec(rect, flag ? tintcolor(color, 0.6f) : color);
	return flag;
}