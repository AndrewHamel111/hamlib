#include "menu.h"
#include "menu/shared.h"

#include <stdlib.h>

// Management values
elementDrawOverride newElementDrawOverride = NULL;
Font newElementFont = {0};
char* newElementAllowedCharacters = NULL;

menu menuCreate(Vector2 position, Vector2 size)
{
	menu mnu = {0};
	mnu.position = position;
	mnu.size = size;
	
	mnu.lastInteractionWasMouse = false;
	for(int i = 0; i < MENU_LENGTH; i++)
		mnu.elementIsValid[i] = false;
	
	return mnu;
}

void menuAdd(menu* mnu, menuElement element)
{
	for(int i = 0; i < MENU_LENGTH; i++)
	{
		if (mnu->elementIsValid[i]) // search for an empty space to put the element
			continue;
		
		// an empty space exists at [i].
		mnu->list[i] = element;
		mnu->elementIsValid[i] = true;
		
		if (element.menuElementType == MetImage || element.menuElementType == MetAnimation)
			return;
		
		// for elements with text, we adjust the font size, so it fits
		// TODO put the auto font size changing here. Generalize elementDrawData modifications in Update / Draw to things for drawUtils and call on them here
		
		break;
	}
	
	// if we reach this code, an error has occurred, since the menu didn't have any elements marked as invalid.
}

void setMenuElementOverrides(elementDrawOverride drawFunction)
{
	newElementDrawOverride = drawFunction;
}

void setMenuElementFont(Font font)
{
	newElementFont = font;
}

void setTextFieldAllowedCharacters(char* allowedCharacters)
{
	newElementAllowedCharacters = allowedCharacters;
}

/////////////////////////////
/// Shared implementation ///
/////////////////////////////

void getMenuElementPositionAndSize(menuElement e, menu m, Vector2* position, Vector2* size)
{
	position->x = m.position.x + (m.size.x * e.position.x);
	position->y = m.position.y + (m.size.y * e.position.y);
	size->x = m.size.x * e.size.x;
	size->y = m.size.y * e.size.y;
} // getMenuElementPositionAndSize

/// Determines if this element can be navigated off by simply moving the mouse. For example, Textfields and Sliders will stay highlighted until special conditions are met. For a textfield, it's pressing Enter or Escape to deselect it. For a slider, it's letting go of Mouse Button 1.
bool typeUsesLightHighlight(enum elementType type)
{
	return type != MetSlider && type != MetTextfield;
}