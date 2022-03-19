#include "menu.h"
#include "menu/shared.h"

#include <stdlib.h>
#include <string.h>

static void updateMenuElement(menuElement* element, menu* mnu);
/// \brief Call with null menuElement to remove highlight
static void menuHighlightElement(menuElement* element, menu* mnu, bool highlightedByMouse);
static void menuHightlightNone(menuElement *element, menu *mnu);

void menuUpdate(menu* mnu)
{
	for (int i = 0; i < MENU_LENGTH; i++)
	{
		if (mnu->elementIsValid[i])
			updateMenuElement(mnu->list + i, mnu);
	}
}

static void updateMenuElement(menuElement* element, menu* mnu)
{
	if (element == NULL)
	{
		// TODO: we need some kind of error logging, especially when I start to build actual game projects.
		return;
	}
	
	if(!element->interactable)
	{
		return;
	}
	
	if (element->menuElementType == MetLabel || element->menuElementType == MetImage || element->menuElementType == MetAnimation)
	{
		return;
	}
	
	// calculate absolute position and size of element
	Vector2 elementPosition;
	Vector2 elementSize;
	getMenuElementPositionAndSize(*element, *mnu, &elementPosition, &elementSize);
	Rectangle elementRect = {
		elementPosition.x,
		elementPosition.y,
		elementSize.x,
		elementSize.y
	};
	
	if (!element->highlighted) // if this element is not highlighted but the mouse hovers it anyway
	{
		if (typeUsesLightHighlight(element->menuElementType) && CheckCollisionPointRec(mousePosition, elementRect))
		{
			if (element->menuElementType != MetSlider || (mouseIsPressed && element->menuElementType == MetSlider))
				menuHighlightElement(element, mnu, true);
		}
		else if (element->menuElementType == MetSlider || element->menuElementType == MetTextfield)
		{
			if (CheckCollisionPointRec(mousePosition, elementRect) && mouseIsPressed)
				menuHighlightElement(element, mnu, true);
		}
		
		// TODO: add dpad case
	}
	else // element currently highlighted
	{
		if (mnu->lastInteractionWasMouse)
		{
			if (typeUsesLightHighlight(element->menuElementType))
			{
				if (!CheckCollisionPointRec(mousePosition, elementRect))
					menuHightlightNone(element, mnu);
			}
			else if (element->menuElementType == MetSlider)
			{
				if (!mouseIsDown && !mouseIsPressed)
				{
					menuHightlightNone(element, mnu);
				}
			}
			else if (element->menuElementType == MetTextfield)
			{
				// currently, handled later in the update, while iterating through all the recently pressed keys. I think this is the best method, so I will leave it.
			}
		}
		else
		{
			// TODO: add dpad case
		}
	}
	
	// TODO move these into separate methods
	if (element->menuElementType == MetButton)
	{
		bool pressed = element->highlighted && mouseIsPressed;
		if (pressed && (element->buttonOnPressed != NULL))
			(*(element->buttonOnPressed))();
	}
	else if (element->menuElementType == MetSlider)
	{
		float elementRectPadding = (0.1f * elementSize.y)*4; // notice that this only makes changes to the copies of the values.
		elementPosition.x += elementRectPadding;
		elementSize.x -= elementRectPadding * 2.0f;
		
		if (element->highlighted)
		{
			if (elementSize.x > 0)
				element->sliderValue = (mousePosition.x - elementPosition.x) / elementSize.x;
			
			element->sliderValue = CLAMP_01(element->sliderValue);
			
			if (element->sliderOnChanged != NULL)
			{
				(*(element->sliderOnChanged))(element->sliderValue);
			}
		}
	}
	else if (element->menuElementType == MetTextfield)
	{
		// TODO: implement functionality to scroll within textfield using arrow keys
		
		if (!element->highlighted)
		{
			// LOG: log here
			return;
		}
		
		int key;
		while ( (key = GetKeyPressed()) != 0 )
		{
			if (key == KEY_BACKSPACE)
			{
				// delete
				int len = strlen(element->textfieldTextInput);
				if (len > 0)
				{
					element->textfieldTextInput[len - 1] = '\0';
				}
			}
			else if (key == KEY_ESCAPE || key == KEY_ENTER)
			{
				menuHightlightNone(element, mnu);
			}
			else if (key > 255)
			{
				// LOG here
				continue;
			}
			// TODO: implement delete key functionality
			else
			{
				if (element->textfieldAllowedCharacters != NULL && strchr(element->textfieldAllowedCharacters, key) == NULL)
					continue;
				
				if (key >= KEY_A && key <= KEY_Z)
				{
					if (!IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_RIGHT_SHIFT))
					{
						key = 'a' + (key - 'A');
					}
				}
				// TODO: use this shift-based sieve to add the some other keys that currently can't be typed
				
				// TODO: be prepared for this to break
				int len = strlen(element->textfieldTextInput);
				if (len < element->textfieldInputMaxLength)
				{
					element->textfieldTextInput[len] = key;
					element->textfieldTextInput[len + 1] = '\0';
				}
			}
		}
	}
} // updateMenuElement

static void menuHighlightElement(menuElement *element, menu *mnu, bool highlightedByMouse)
{
	if (element == NULL)
	{
		menuHightlightNone(element, mnu);
		return;
	}
	else if (mnu->highlightedElement != NULL)
	{
		menuHightlightNone(element, mnu);
	}
	
	element->highlighted = true;
	mnu->lastInteractionWasMouse = highlightedByMouse;
	mnu->highlightedElement = element;
	if (element->onElementHighlighted != NULL)
		element->onElementHighlighted(element);
} // menuHighlightElement

static void menuHightlightNone(menuElement *element, menu *mnu)
{
	menuElement* e = mnu->highlightedElement;
	e->highlighted = false;
	if (e->onElementHighlightedOff != NULL)
		e->onElementHighlightedOff(element);
	mnu->highlightedElement = NULL;
}