#include "menu/drawUtils.h"

Color getPressedColor(Color basecolor)
{
	Vector3 hsv = ColorToHSV(basecolor);
	
	if (hsv.y > 0.50f)
		return ColorFromHSV(hsv.x, hsv.y - 0.15f, hsv.z);
	else
		return ColorFromHSV(hsv.x, hsv.y + 0.15f, hsv.z);
}

Color getHighlightColor(Color basecolor)
{
	Vector3 hsv = ColorToHSV(basecolor);
	
	if (hsv.y > 0.50f)
		return ColorFromHSV(hsv.x, hsv.y - 0.1f, hsv.z);
	else
		return ColorFromHSV(hsv.x, hsv.y + 0.1f, hsv.z);
}

void fitDataForSlider(elementDrawData* data)
{
	data->position.y += data->size.y * 0.34f;
	data->size.y -= data->size.y * 0.34f;
	data->rect = (Rectangle) {
		data->position.x,
		data->position.y,
		data->size.x,
		data->size.y
	};
}

void fitDataForTextfield(elementDrawData* data)
{
	// Squash the interactive element into the lower two thirds
	data->position.y += data->size.y * 0.34f;
	data->size.y -= data->size.y * 0.34f;
	data->rect = (Rectangle) {
		data->position.x,
		data->position.y,
		data->size.x,
		data->size.y
	};
}