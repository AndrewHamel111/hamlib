#include "menu.h"
#include "menu/shared.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void updateMenuElement(menuElement* element, menu* mnu);

static int getShiftedKey(int key);

void menuUpdate(menu* mnu)
{
	if (mnu->state == MNU_OPENING || mnu->state == MNU_CLOSING)
	{
		handleMenuTransition(mnu);
		return;
	}
	
	if (mnu->state == MNU_CLOSED)
	{
		return;
	}
	
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
					menuHighlightNone(mnu);
			}
			else if (element->menuElementType == MetSlider)
			{
				if (!mouseIsDown && !mouseIsPressed)
				{
					menuHighlightNone(mnu);
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
		
		bool shiftHeld = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
		
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
				menuHighlightNone(mnu);
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
					if (!shiftHeld)
					{
						// here we use the "shifted key" because even though shift isn't being held, raylib's KEY values are actually the caps and we want the lower-case when they aren't holding shift.
						key = getShiftedKey(key);
					}
				}
				else
				{
					if (shiftHeld)
					{
						key = getShiftedKey(key);
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

static int getShiftedKey(int key)
{
	if (key >= 'A' && key <= 'Z')
	{
		return tolower(key);
	}
	
	if (key >= 'a' && key <= 'z')
	{
		return toupper(key);
	}
	
	switch (key)
	{
		case '1':
			return '!';
		case '2':
			return '@';
		case '3':
			return '#';
		case '4':
			return '$';
		case '5':
			return '%';
		case '6':
			return '^';
		case '7':
			return '&';
		case '8':
			return '*';
		case '9':
			return '(';
		case '0':
			return ')';
		case '-':
			return '_';
		case '=':
			return '+';
		case '\\':
			return '|';
		case ';':
			return ':';
		case '\'':
			return '\"';
		case ',':
			return '<';
		case '.':
			return '>';
		case '/':
			return '?';
		case '[':
			return '{';
		case ']':
			return '}';
		case '`':
			return '~';
		default:
			// do nothing
			break;
	}
	
	// LOG here
	return key; // no meaningful shift found, so we just return the base value.
}