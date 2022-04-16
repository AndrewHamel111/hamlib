#include "menu.h"
#include "menu/shared.h"

#include <stdlib.h>
#include <string.h>
#include <easings.h>

#define MENU_TIME_OPEN 0.73f
#define MENU_TIME_CLOSE 0.62f

// Management values
elementDrawOverride newElementDrawOverride = NULL;
Font newElementFont = {0};
char* newElementAllowedCharacters = NULL;

menu menuCreate(Vector2 position, Vector2 size)
{
	menu mnu = {0};
	mnu.position = position;
	mnu.size = size;
	
	mnu.openPosition = position;
	mnu.closedPosition = (Vector2){-size.x, 0};
	
	mnu.lastInteractionWasMouse = false;
	for(int i = 0; i < MENU_LENGTH; i++)
		mnu.elementIsValid[i] = false;
	
	menuOpen(&mnu);
	
	return mnu;
}

menuElement* menuAdd(menu* mnu, menuElement element)
{
	menuElement* value = NULL;
	
	for(int i = 0; i < MENU_LENGTH; i++)
	{
		if (mnu->elementIsValid[i]) // search for an empty space to put the element
			continue;
		
		// an empty space exists at [i].
		mnu->list[i] = element;
		strcpy(mnu->list[i].elementName, element.elementName);
		mnu->elementIsValid[i] = true;
		
		value = mnu->list + i;
		
		if (element.menuElementType == MetImage || element.menuElementType == MetAnimation)
			break;
		
		// for elements with text, we adjust the font size, so it fits
		// TODO put the auto font size changing here. Generalize elementDrawData modifications in Update / Draw to things for drawUtils and call on them here
		
//		Vector2 position, size;
//		getMenuElementPositionAndSize(element, *mnu, &position, &size);
//
//		getMenuElementTextArea(element, *mnu, &position, &size);
//
		break;
	}
	
	return value;
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
	if (position != NULL)
	{
		position->x = m.position.x + (m.size.x * e.position.x);
		position->y = m.position.y + (m.size.y * e.position.y);
	}
	
	if (size != NULL)
	{
		size->x = m.size.x * e.size.x;
		size->y = m.size.y * e.size.y;
	}
} // getMenuElementPositionAndSize

void getMenuElementTextArea(menuElement e, menu m, Vector2* position, Vector2* size)
{
	// TODO check everywhere that computes textarea size and consolidate them here.
	if (e.menuElementType == MetLabel)
	{
	
	}
	else if (e.menuElementType == MetButton)
	{
	
	}
	else if (e.menuElementType == MetTextfield)
	{
	
	}
	else if (e.menuElementType == MetImage || e.menuElementType == MetAnimation)
	{
		if (position != NULL)
			*position = Vector2Zero();
		if (size != NULL)
			*size = Vector2Zero();
		return;
	}
}

/// Determines if this element can be navigated off by simply moving the mouse. For example, Textfields and Sliders will stay highlighted until special conditions are met. For a textfield, it's pressing Enter or Escape to deselect it. For a slider, it's letting go of Mouse Button 1.
bool typeUsesLightHighlight(enum elementType type)
{
	return type != MetSlider && type != MetTextfield;
}

void menuHighlightElement(menuElement *element, menu *mnu, bool highlightedByMouse)
{
	if (element == NULL)
	{
		menuHighlightNone(mnu);
		return;
	}
	else if (mnu->highlightedElement != NULL)
	{
		menuHighlightNone(mnu);
	}
	
	element->highlighted = true;
	mnu->lastInteractionWasMouse = highlightedByMouse;
	mnu->highlightedElement = element;
	if (element->onElementHighlighted != NULL)
		element->onElementHighlighted(element);
} // menuHighlightElement

void menuHighlightNone(menu *mnu)
{
	menuElement* e = mnu->highlightedElement;
	
	if (e != NULL)
	{
		e->highlighted = false;
		if (e->onElementHighlightedOff != NULL)
			e->onElementHighlightedOff(e);
	}
	
	mnu->highlightedElement = NULL;
}

void menuClose(menu* mnu)
{
	if (mnu->state == MNU_CLOSED)
	{
		return;
	}
	
	float t = 0;
	
	if (mnu->transitionCanBeInterrupted)
	{
		if (mnu->state == MNU_OPENING)
		{
			timingData d = mnu->transitionTiming;
			if (d.duration != 0)
			{
				t = 1 - d.time / d.duration;
			}
		}
	}
	else
	{
		if (mnu->state != MNU_OPEN)
		{
			return;
		}
	}
	
	menuHighlightNone(mnu);
	
	mnu->state = MNU_CLOSING;
	mnu->transitionTiming = (timingData){.time = t * MENU_TIME_CLOSE, .duration = MENU_TIME_CLOSE};
}

void menuOpen(menu* mnu)
{
	if (mnu->state == MNU_OPEN)
	{
		return;
	}
	
	float t = 0;
	
	if (mnu->transitionCanBeInterrupted)
	{
		if (mnu->state == MNU_CLOSING)
		{
			timingData d = mnu->transitionTiming;
			if (d.duration != 0)
			{
				t = 1 - d.time / d.duration;
			}
		}
	}
	else
	{
		if (mnu->state != MNU_CLOSED)
		{
			return;
		}
	}
	
	mnu->state = MNU_OPENING;
	mnu->transitionTiming = (timingData){.time = t * MENU_TIME_OPEN, .duration = MENU_TIME_OPEN};
}

void handleMenuTransition(menu* mnu)
{
	Vector2 startPos, endPos;
	
	startPos = mnu->closedPosition;
	endPos = mnu->openPosition;
	
	mnu->transitionTiming.time += frametime;
	float t = mnu->transitionTiming.time / mnu->transitionTiming.duration; // norm
	
	if (mnu->state == MNU_CLOSING)
	{
		t = 1 - t;
	}
	
	Vector2 diff = Vector2Subtract(endPos, startPos);
	
	mnu->position.x = EaseQuadInOut(t, startPos.x, diff.x, 1.0f);
	mnu->position.y = EaseQuadInOut(t, startPos.y, diff.y, 1.0f);
	
	if (t > 1 || t < 0)
	{
		if (mnu->state == MNU_CLOSING)
		{
			mnu->state = MNU_CLOSED;
			if (mnu->onMenuClosed != NULL)
			{
				mnu->onMenuClosed(*mnu);
			}
		}
		
		if (mnu->state == MNU_OPENING)
		{
			mnu->state = MNU_OPEN;
			if (mnu->onMenuOpened != NULL)
			{
				mnu->onMenuOpened(*mnu);
			}
		}
	}
}