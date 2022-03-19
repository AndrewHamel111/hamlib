/**
 * \file menu.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"
#include "hamlib/draw.h"
#include "hamlib/menu.h"

#include <stdlib.h>
#include <string.h>

#define GAMEPAD_MENU_DEADZONE 0.1

// TODO brew some coffee and unify much of the code in updateMenu

#define BUTTONS_LENGTH 6
enum buttons
{
    ButtonUp, ButtonDown, ButtonLeft, ButtonRight, ButtonB, ButtonA
};
static bool buttonpressed[BUTTONS_LENGTH] = {false};

// WILL NEED IF I DECIDE TO ALLOW THE JOYSTICK FOR NAVIGATING THE MENU
/** prevents the joystick from freaking out the menu logic when an input is held */
// static bool gamepadAvailable;

uiElement createUiElementButton(Rectangle rectangle, char* string, void (*func)(void))
{
    uiElement element;
    element.rectangle = rectangle;
    strcpy(element.msg, string);
	element.fontsize = -1;
    element.onSelect = func;
    
    element.color = (Color){255,255,255,255};
    element.textColor = (Color){15,15,15,255};
    element.highlightedColor = (Color){255,220,255,255};
	
	// particleSprite uninitialized
	element.drawmode = DmRect | DmText;
	element.highlightmode = HmColor;

	element.isEmpty = false;
	element.nav = (uiElementNav){-1, -1, -1, -1};
    
    return element;
}

uiElement createEmptyUiElement()
{
    uiElement element;
    element.isEmpty = true;
    return element;
}

void setUiElementColors(uiElement* element, Color color, Color textColor, Color highlightedColor)
{
    element->color = color;
    element->textColor = textColor;
    element->highlightedColor = highlightedColor;
}

void setUiElementBehaviour(uiElement* element, void (*func)(void))
{
    element->onSelect = func;
}

void drawUiElement(uiElement element, bool isSelected)
{
	if (element.isEmpty) return;

	Color color = (isSelected && (element.highlightmode == HmColor)) ? (element.highlightedColor) : (element.color);
	Texture2D sprite = (isSelected && (element.highlightmode == HmSpriteswap)) ? element.highlightedsprite : element.sprite;
	
	if ((element.drawmode & DmRect) == DmRect)
	{
    	DrawRectangleRec(element.rectangle, color);
	}
	
	if ((element.drawmode & DmNineslice) == DmNineslice)
	{
		DrawTextureNPatch(sprite, element.nfo, element.rectangle, (Vector2){0,0}, 0.0f, color);
	}
	else if ((element.drawmode & DmSprite) == DmSprite)
	{
		DrawTexturePro(sprite, pureSource(sprite), element.rectangle, getRectCenter(element.rectangle), 0.0f, color);
	}

	if ((element.drawmode & DmText) == DmText)
	{
		Vector2 v = getRectCenter(element.rectangle);
		drawTextAligned(element.msg, v.x, v.y,
						(element.fontsize > 0) ? (element.fontsize) : (element.rectangle.height / 3), element.textColor,
						TaCenter | TaMiddle);
	}
}

menu createMenu(uiElement* element, unsigned char sz)
{
    menu menu;
	menu.elementsCount = sz;
    
    // reallocate the elements data so that after calling createMenu the original list is freed automatically.
    uiElement* newElements = (uiElement*)malloc(sizeof(uiElement) * (menu.elementsCount));
    for(int i = 0; i < sz; i++)
		newElements[i] = element[i];
    // free the original list
    free(element);
    
    menu.elements = newElements;
	menu.elementsCount = sz;
    menu.index = 0;
    menu.wrapbehaviour = MwWrap;
	menu.isGridMenu = false;
	
	menu.lastindex = 0;
	menu.currentindexsetbymouse = false;
	menu.mousedisengaged = false;

    return menu;
}

menu createGridMenu(uiElement* element, Vector2 gridSz)
{
    menu menu;
	menu.elementsCount = gridSz.x * gridSz.y;
    
    // reallocate the elements data so that after calling createMenu the original list is freed automatically.
    uiElement* newElements = (uiElement*)malloc(sizeof(uiElement) * (menu.elementsCount));
    for(int i = 0; i < menu.elementsCount; i++)
		newElements[i] = element[i];
    // free the original list
    free(element);
    
    menu.elements = newElements;
    menu.sz = gridSz;
    menu.index = 0;
    menu.wrapbehaviour = MwWrap;
	menu.isGridMenu = true;
	
	menu.lastindex = 0;
	menu.currentindexsetbymouse = false;
	menu.mousedisengaged = false;

    return menu;
}

void clampMenuIndex(menu* mnu)
{
	if (mnu->index >= mnu->elementsCount)
		mnu->index = (mnu->wrapbehaviour == MwWrap) ? 0 : (mnu->elementsCount - 1);
	else if (mnu->index < 0)
		mnu->index = (mnu->wrapbehaviour == MwWrap) ? (mnu->elementsCount - 1) : 0;
}

void clampMenuGrid(menu* mnu, Vector2 d)
{
	if (mnu->index >= mnu->elementsCount)
		mnu->index = ((mnu->wrapbehaviour & MwWrap) == MwWrap) ? (mnu->index % mnu->elementsCount) : mnu->elementsCount - 1;
	else if (mnu->index < 0)
		mnu->index = ((mnu->wrapbehaviour & MwWrap) == MwWrap) ? mnu->elementsCount + mnu->index : 0;

	// removes the "michaela behaviour"
	if (((mnu->wrapbehaviour & MwWrap) == MwWrap))
	{
		if (!(mnu->index % (char)(mnu->sz.x)) && (d.x > 0))
		{
			if (mnu->index == 0)
				mnu->index = mnu->elementsCount - mnu->sz.x;
			else
				mnu->index -= mnu->sz.x;
		}
		else if (!((mnu->index + 1) % (char)(mnu->sz.x)) && (d.x < 0))
		{
			if (mnu->index == (mnu->elementsCount - 1))
				mnu->index = mnu->sz.x - 1;
			else
				mnu->index += mnu->sz.x;
		}
	}

	if (mnu->wrapbehaviour == MwWrap)
	{
		// escape empty elements
		while(mnu->elements[mnu->index].isEmpty && !((d.x == 0) && (d.y == 0)))
		{
			signed char flatD = d.x + (d.y * mnu->sz.x);
			mnu->index += flatD;

			if (mnu->index >= mnu->elementsCount)
				mnu->index = mnu->index % mnu->elementsCount;
			else if (mnu->index < 0)
				mnu->index = mnu->elementsCount + mnu->index;
		}
	}
	else if (((mnu->wrapbehaviour & MwRise) == MwRise) || ((mnu->wrapbehaviour & MwFall) == MwFall))
	{
		signed char step = ((mnu->wrapbehaviour & MwRise) == MwRise) ? (-mnu->sz.x) : (mnu->sz.x);
		signed char esc = 0; // attempted steps. if we loop around the entire menu we will just abandon the input
		while(mnu->elements[mnu->index].isEmpty && (++esc < (signed char)(1.15f * mnu->sz.y)))
		{
			mnu->index += step;
		}

		if (esc >= (signed char)(1.15f * mnu->sz.y))
		{
			// undo the input that got us here
			signed char flatD = d.x + (d.y * mnu->sz.x);
			flatD *= -1;
			mnu->index += flatD;
		}
	}
	else if (mnu->wrapbehaviour == MwNone)
	{
		if (mnu->index >= mnu->elementsCount)
			mnu->index = mnu->elementsCount - 1;
		else if (mnu->index < 0)
			mnu->index = 0;
	}
}

void updateMenu(menu* mnu)
{
	buttonpressed[ButtonUp] = IsKeyPressed(KEY_W); // poll inputs
	buttonpressed[ButtonDown] = IsKeyPressed(KEY_S);
	buttonpressed[ButtonLeft] = IsKeyPressed(KEY_A);
	buttonpressed[ButtonRight] = IsKeyPressed(KEY_D);
	buttonpressed[ButtonB] = IsKeyPressed(KEY_L);
	buttonpressed[ButtonA] = IsKeyPressed(KEY_SEMICOLON);

	buttonpressed[ButtonA] = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	int i = 0;
	while(IsGamepadAvailable(i))
	{
		/*
		if (ABS(GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_X) > GAMEPAD_MENU_DEADZONE ||
		ABS(GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_Y)) > GAMEPAD_MENU_DEADZONE)
			gamepadAvailable = false;

		buttonpressed[BUTTON_UP] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_Y) < -GAMEPAD_MENU_DEADZONE;
		buttonpressed[BUTTON_DOWN] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_Y) > GAMEPAD_MENU_DEADZONE;
		buttonpressed[BUTTON_LEFT] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_X) < -GAMEPAD_MENU_DEADZONE;
		buttonpressed[BUTTON_RIGHT] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_X) > GAMEPAD_MENU_DEADZONE;
		*/

		buttonpressed[ButtonA] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_MIDDLE_RIGHT);

		buttonpressed[ButtonUp] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_UP);
		buttonpressed[ButtonDown] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
		buttonpressed[ButtonLeft] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
		buttonpressed[ButtonRight] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
		buttonpressed[ButtonA] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
		buttonpressed[ButtonB] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);

		i++;
	}
	

	bool confirm = buttonpressed[ButtonA]; // || buttonpressed[BUTTON_B];
	//bool back = buttonpressed[BUTTON_B];
	
	if (mnu->isGridMenu)
	{ // grid case
		Vector2 d = (Vector2){(signed char)buttonpressed[ButtonRight] - (signed char)buttonpressed[ButtonLeft], (signed char)buttonpressed[ButtonDown] - (signed char)buttonpressed[ButtonUp]};

		// check for mouse navigation
		bool flag = false;
		for(int j = 0; j < mnu->elementsCount; j++)
		{
			if (CheckCollisionPointRec(GetMousePosition(), (mnu->elements[j]).rectangle) && !((mnu->elements[j]).isEmpty))
			{
				mnu->lastindex = mnu->index;
				mnu->index = j;
				mnu->currentindexsetbymouse = true;
				mnu->mousedisengaged = false;
				flag = true;
				d = (Vector2){0,0};
			}
		}

		if (!flag && mnu->currentindexsetbymouse)
		{
			mnu->mousedisengaged = true;
			mnu->index = -1;
		}
		
		signed char flatD = d.x + (d.y * mnu->sz.x);

		if(flatD != 0)
		{
			uiElement e = mnu->elements[(mnu->index == -1) ? mnu->lastindex : mnu->index];
			
			mnu->mousedisengaged = false;
			mnu->currentindexsetbymouse = false;
			if (mnu->index == -1)
			{
				if (hasCustomNav(e))
				{
					if ((e.nav.left > -1) && (d.x < 0))
					{
						mnu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (d.x > 0))
					{
						mnu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (d.y < 0))
					{
						mnu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (d.y > 0))
					{
						mnu->index = e.nav.down;
					}
				}
				else
				{
					mnu->index = mnu->lastindex + flatD;
					clampMenuGrid(mnu, d);
				}
			}
			else 
			{
				mnu->lastindex = mnu->index;
				if (hasCustomNav(e))
				{
					if ((e.nav.left > -1) && (d.x < 0))
					{
						mnu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (d.x > 0))
					{
						mnu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (d.y < 0))
					{
						mnu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (d.y > 0))
					{
						mnu->index = e.nav.down;
					}
				}
				else
				{
					mnu->index += flatD;
					clampMenuGrid(mnu, d);
				}
			}

		}
	} // grid case
	else
	{ // linear case
		signed char d = (signed char)(buttonpressed[ButtonDown] || buttonpressed[ButtonRight]) - (signed char)(buttonpressed[ButtonUp] || buttonpressed[ButtonLeft]);

		Vector2 vD = (Vector2){(signed char)buttonpressed[ButtonRight] - (signed char)buttonpressed[ButtonLeft], (signed char)buttonpressed[ButtonDown] - (signed char)buttonpressed[ButtonUp]};

		// check for mouse navigation
		bool flag = false;
		for(int j = 0; j < mnu->elementsCount; j++)
		{
			if (CheckCollisionPointRec(GetMousePosition(), (mnu->elements[j]).rectangle))
			{
				mnu->lastindex = mnu->index;
				mnu->index = j;
				mnu->currentindexsetbymouse = true;
				mnu->mousedisengaged = false;
				flag = true;
				d = 0;
			}
		}

		if (!flag && mnu->currentindexsetbymouse)
		{
			mnu->mousedisengaged = true;
			mnu->index = -1;
		}

		if(d != 0)
		{
			uiElement e = mnu->elements[(mnu->index == -1) ? mnu->lastindex : mnu->index];
			
			mnu->mousedisengaged = false;
			mnu->currentindexsetbymouse = false;
			if (mnu->index == -1)
			{
				if (hasCustomNav(e))
				{
					if ((e.nav.left > -1) && (vD.x < 0))
					{
						mnu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (vD.x > 0))
					{
						mnu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (vD.y < 0))
					{
						mnu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (vD.y > 0))
					{
						mnu->index = e.nav.down;
					}
				}
				else
				{
					mnu->index = mnu->lastindex + d;
					clampMenuIndex(mnu);
				}
			}
			else 
			{
				mnu->lastindex = mnu->index;
				if (hasCustomNav(e))
				{
					if ((e.nav.left > -1) && (vD.x < 0))
					{
						mnu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (vD.x > 0))
					{
						mnu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (vD.y < 0))
					{
						mnu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (vD.y > 0))
					{
						mnu->index = e.nav.down;
					}
				}
				else
				{
					mnu->index += d;
					clampMenuIndex(mnu);
				}
			}
		}
	} // linear case

    if (confirm)
    {
		if (mnu->index != -1)
        	(*(mnu->elements[mnu->index].onSelect))();
    }
}

void drawMenu(menu mnu)
{
	int sz = (mnu.isGridMenu) ? mnu.sz.x * mnu.sz.y : mnu.elementsCount;
    for(int i = 0; i < sz; i++)
		drawUiElement(mnu.elements[i], mnu.index == i);
}

uiElement elementAt(menu mnu, int x, int y)
{
	if (mnu.isGridMenu)
	{
		x = x % (int)mnu.sz.x; // bounds for x and y. for valid input,
		y = y % (int)mnu.sz.y; // this changes nothing
		
		return mnu.elements[(y * (int)mnu.sz.x) + x];
	}
	else
		return *(mnu.elements);
}

uiElement* elementRefAt(menu mnu, int x, int y)
{
	if (mnu.isGridMenu)
	{
		x = x % (int)mnu.sz.x; // bounds for x and y. for valid input,
		y = y % (int)mnu.sz.y; // this changes nothing
		
		return mnu.elements + ((y * (int)mnu.sz.x) + x);
	}
	else
		return mnu.elements;
}

void setNavUp(uiElement* element, signed char i)
{
	uiElementNav n = element->nav;
	n.up = i;
	element->nav = n;
}

void setNavRight(uiElement* element, signed char i)
{
	uiElementNav n = element->nav;
	n.right = i;
	element->nav = n;
}

void setNavDown(uiElement* element, signed char i)
{
	uiElementNav n = element->nav;
	n.down = i;
	element->nav = n;
}

void setNavLeft(uiElement* element, signed char i)
{
	uiElementNav n = element->nav;
	n.left = i;
	element->nav = n;
}

void setNav(uiElement* element, signed char right, signed char down, signed char left, signed char up)
{
	element->nav = (uiElementNav){right, down, left, up};
}

bool hasCustomNav(uiElement element)
{
	return ((element.nav.down > -1) || (element.nav.up > -1)) || ((element.nav.left > -1) || (element.nav.right > -1));
}
