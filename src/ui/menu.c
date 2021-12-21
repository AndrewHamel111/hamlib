/**
 * \file menu.c
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#include "hamlib.h"
#include "hamlib/menu.h"

#include <stdlib.h>
#include <string.h>

#define GAMEPAD_MENU_DEADZONE 0.1

// TODO brew some coffee and unify much of the code in UpdateMenu

#define BUTTONS_LENGTH 6
enum BUTTONS
{
    BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A
};
static bool buttonpressed[BUTTONS_LENGTH] = {false};

UIElement CreateUIElementButton(Rectangle rectangle, char* msg, void (*func)(void))
{
    UIElement element;
    element.rectangle = rectangle;
    strcpy(element.msg, msg);
	element.fontsize = -1;
    element.onSelect = func;
    
    element.color = (Color){255,255,255,255};
    element.textColor = (Color){15,15,15,255};
    element.highlightedColor = (Color){255,220,255,255};
	
	// sprite uninitialized
	element.drawmode = DM_RECT | DM_TEXT;
	element.highlightmode = HM_COLOR;

	element.isEmpty = false;
	element.nav = (UIElementNav){-1, -1, -1, -1};
    
    return element;
}

UIElement CreateEmptyUIElement()
{
    UIElement element;
    element.isEmpty = true;
    return element;
}

void SetUIElementColors(UIElement* element, Color color, Color textColor, Color highlightedColor)
{
    element->color = color;
    element->textColor = textColor;
    element->highlightedColor = highlightedColor;
}

void SetUIElementBehaviour(UIElement* element, void (*func)(void))
{
    element->onSelect = func;
}

void DrawUIElement(UIElement element, bool isSelected)
{
	if (element.isEmpty) return;

	Color color = (isSelected && (element.highlightmode == HM_COLOR)) ? (element.highlightedColor) : (element.color);
	Texture2D sprite = (isSelected && (element.highlightmode == HM_SPRITESWAP)) ? element.highlightedsprite : element.sprite;
	
	if ((element.drawmode & DM_RECT) == DM_RECT)
	{
    	DrawRectangleRec(element.rectangle, color);
	}
	
	if ((element.drawmode & DM_NINESLICE) == DM_NINESLICE)
	{
		DrawTextureNPatch(sprite, element.nfo, element.rectangle, (Vector2){0,0}, 0.0f, color);
	}
	else if ((element.drawmode & DM_SPRITE) == DM_SPRITE)
	{
		DrawTexturePro(sprite, PureSource(sprite), element.rectangle, GetCenter(element.rectangle), 0.0f, color);
	}

	if ((element.drawmode & DM_TEXT) == DM_TEXT)
	{
		Vector2 v = GetCenter(element.rectangle);
		DrawTextAligned(element.msg, v.x, v.y, (element.fontsize > 0) ? (element.fontsize) : (element.rectangle.height/3), element.textColor, TA_CENTER | TA_MIDDLE);
	}
}

Menu CreateMenu(UIElement* elements, unsigned char sz)
{
    Menu menu;
	menu._sz = sz;
    
    // reallocate the elements data so that after calling CreateMenu the original list is freed automatically.
    UIElement* _elements = (UIElement*)malloc(sizeof(UIElement) * (menu._sz));
    for(int i = 0; i < sz; i++)
        _elements[i] = elements[i];
    // free the original list
    free(elements);
    
    menu.elements = _elements;
	menu._sz = sz;
    menu.index = 0;
    menu.wrapbehaviour = MW_WRAP;
	menu.isGridMenu = false;
	
	menu.lastindex = 0;
	menu.currentindexsetbymouse = false;
	menu.mousedisengaged = false;

    return menu;
}

Menu CreateGridMenu(UIElement* elements, Vector2 sz)
{
    Menu menu;
	menu._sz = sz.x * sz.y;
    
    // reallocate the elements data so that after calling CreateMenu the original list is freed automatically.
    UIElement* _elements = (UIElement*)malloc(sizeof(UIElement) * (menu._sz));
    for(int i = 0; i < menu._sz; i++)
        _elements[i] = elements[i];
    // free the original list
    free(elements);
    
    menu.elements = _elements;
    menu.sz = sz;
    menu.index = 0;
    menu.wrapbehaviour = MW_WRAP;
	menu.isGridMenu = true;
	
	menu.lastindex = 0;
	menu.currentindexsetbymouse = false;
	menu.mousedisengaged = false;

    return menu;
}

void ClampMenuIndex(Menu* menu)
{
	if (menu->index >= menu->_sz)
		menu->index = (menu->wrapbehaviour == MW_WRAP) ? 0 : (menu->_sz - 1);
	else if (menu->index < 0)
		menu->index = (menu->wrapbehaviour == MW_WRAP) ? (menu->_sz - 1) : 0;
}

void ClampMenuGrid(Menu* menu, Vector2 d)
{
	if (menu->index >= menu->_sz)
		menu->index = ((menu->wrapbehaviour & MW_WRAP) == MW_WRAP) ? (menu->index % menu->_sz) : menu->_sz - 1;
	else if (menu->index < 0)
		menu->index = ((menu->wrapbehaviour & MW_WRAP) == MW_WRAP) ? menu->_sz + menu->index : 0;

	// removes the "michaela behaviour"
	if (((menu->wrapbehaviour & MW_WRAP) == MW_WRAP))
	{
		if (!(menu->index % (char)(menu->sz.x)) && (d.x > 0))
		{
			if (menu->index == 0)
				menu->index = menu->_sz - menu->sz.x;
			else
				menu->index -= menu->sz.x;
		}
		else if (!((menu->index + 1) % (char)(menu->sz.x)) && (d.x < 0))
		{
			if (menu->index == (menu->_sz - 1))
				menu->index = menu->sz.x - 1;
			else
				menu->index += menu->sz.x;
		}
	}

	if (menu->wrapbehaviour == MW_WRAP)
	{
		// escape empty elements
		while(menu->elements[menu->index].isEmpty && !((d.x == 0) && (d.y == 0)))
		{
			signed char flat_d = d.x + (d.y * menu->sz.x);
			menu->index += flat_d;

			if (menu->index >= menu->_sz)
				menu->index = menu->index % menu->_sz;
			else if (menu->index < 0)
				menu->index = menu->_sz + menu->index;
		}
	}
	else if (((menu->wrapbehaviour & MW_RISE) == MW_RISE) || ((menu->wrapbehaviour & MW_FALL) == MW_FALL))
	{
		signed char step = ((menu->wrapbehaviour & MW_RISE) == MW_RISE) ? (-menu->sz.x) : (menu->sz.x);
		signed char esc = 0; // attempted steps. if we loop around the entire menu we will just abandon the input
		while(menu->elements[menu->index].isEmpty && (++esc < (signed char)(1.15f * menu->sz.y)))
		{
			menu->index += step;
		}

		if (esc >= (signed char)(1.15f * menu->sz.y))
		{
			// undo the input that got us here
			signed char flat_d = d.x + (d.y * menu->sz.x);
			flat_d *= -1;
			menu->index += flat_d;
		}
	}
	else if (menu->wrapbehaviour == MW_NONE)
	{
		if (menu->index >= menu->_sz)
			menu->index = menu->_sz - 1;
		else if (menu->index < 0)
			menu->index = 0;
	}
}

void UpdateMenu(Menu* menu)
{
	buttonpressed[BUTTON_UP] = IsKeyPressed(KEY_W); // poll inputs
	buttonpressed[BUTTON_DOWN] = IsKeyPressed(KEY_S);
	buttonpressed[BUTTON_LEFT] = IsKeyPressed(KEY_A);
	buttonpressed[BUTTON_RIGHT] = IsKeyPressed(KEY_D);
	buttonpressed[BUTTON_B] = IsKeyPressed(KEY_L);
	buttonpressed[BUTTON_A] = IsKeyPressed(KEY_SEMICOLON);

	buttonpressed[BUTTON_A] = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	int i = 0;
	while(IsGamepadAvailable(i))
	{
		buttonpressed[BUTTON_UP] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_Y) < -GAMEPAD_MENU_DEADZONE;
		buttonpressed[BUTTON_DOWN] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_Y) > GAMEPAD_MENU_DEADZONE;
		buttonpressed[BUTTON_LEFT] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_X) < -GAMEPAD_MENU_DEADZONE;
		buttonpressed[BUTTON_RIGHT] |= GetGamepadAxisMovement(i, GAMEPAD_AXIS_LEFT_X) > GAMEPAD_MENU_DEADZONE;

		buttonpressed[BUTTON_A] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_MIDDLE_RIGHT);

		buttonpressed[BUTTON_UP] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_UP);
		buttonpressed[BUTTON_DOWN] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
		buttonpressed[BUTTON_LEFT] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
		buttonpressed[BUTTON_RIGHT] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
		buttonpressed[BUTTON_B] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
		buttonpressed[BUTTON_A] |= IsGamepadButtonPressed(i, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);

		i++;
	}

	bool confirm = buttonpressed[BUTTON_A]; // || buttonpressed[BUTTON_B];
	bool back = buttonpressed[BUTTON_B];
	
	if (menu->isGridMenu)
	{ // grid case
		Vector2 d = (Vector2){(signed char)buttonpressed[BUTTON_RIGHT] - (signed char)buttonpressed[BUTTON_LEFT], (signed char)buttonpressed[BUTTON_DOWN] - (signed char)buttonpressed[BUTTON_UP]};

		// check for mouse navigation
		bool flag = false;
		for(int i = 0; i < menu->_sz; i++)
		{
			if (CheckCollisionPointRec(GetMousePosition(), (menu->elements[i]).rectangle) && !((menu->elements[i]).isEmpty))
			{
				menu->lastindex = menu->index;
				menu->index = i;
				menu->currentindexsetbymouse = true;
				menu->mousedisengaged = false;
				flag = true;
				d = (Vector2){0,0};
			}
		}

		if (!flag && menu->currentindexsetbymouse)
		{
			menu->mousedisengaged = true;
			menu->index = -1;
		}
		
		signed char flat_d = d.x + (d.y * menu->sz.x);

		if(flat_d != 0)
		{
			UIElement e = menu->elements[(menu->index == -1) ? menu->lastindex : menu->index];

			menu->mousedisengaged = false;
			menu->currentindexsetbymouse = false;
			if (menu->index == -1)
			{
				if (HasCustomNav(e))
				{
					if ((e.nav.left > -1) && (d.x < 0))
					{
						menu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (d.x > 0))
					{
						menu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (d.y < 0))
					{
						menu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (d.y > 0))
					{
						menu->index = e.nav.down;
					}
				}
				else
				{
					menu->index = menu->lastindex + flat_d;
					ClampMenuGrid(menu, d);
				}
			}
			else 
			{
				menu->lastindex = menu->index;
				if (HasCustomNav(e))
				{
					if ((e.nav.left > -1) && (d.x < 0))
					{
						menu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (d.x > 0))
					{
						menu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (d.y < 0))
					{
						menu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (d.y > 0))
					{
						menu->index = e.nav.down;
					}
				}
				else
				{
					menu->index += flat_d;
					ClampMenuGrid(menu, d);
				}
			}

		}
	} // grid case
	else
	{ // linear case
		signed char d = (signed char)(buttonpressed[BUTTON_DOWN] || buttonpressed[BUTTON_RIGHT]) - (signed char)(buttonpressed[BUTTON_UP] || buttonpressed[BUTTON_LEFT]);

		Vector2 v_d = (Vector2){(signed char)buttonpressed[BUTTON_RIGHT] - (signed char)buttonpressed[BUTTON_LEFT], (signed char)buttonpressed[BUTTON_DOWN] - (signed char)buttonpressed[BUTTON_UP]};

		// check for mouse navigation
		bool flag = false;
		for(int i = 0; i < menu->_sz; i++)
		{
			if (CheckCollisionPointRec(GetMousePosition(), (menu->elements[i]).rectangle))
			{
				menu->lastindex = menu->index;
				menu->index = i;
				menu->currentindexsetbymouse = true;
				menu->mousedisengaged = false;
				flag = true;
				d = 0;
			}
		}

		if (!flag && menu->currentindexsetbymouse)
		{
			menu->mousedisengaged = true;
			menu->index = -1;
		}

		if(d != 0)
		{
			UIElement e = menu->elements[(menu->index == -1) ? menu->lastindex : menu->index];

			menu->mousedisengaged = false;
			menu->currentindexsetbymouse = false;
			if (menu->index == -1)
			{
				if (HasCustomNav(e))
				{
					if ((e.nav.left > -1) && (v_d.x < 0))
					{
						menu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (v_d.x > 0))
					{
						menu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (v_d.y < 0))
					{
						menu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (v_d.y > 0))
					{
						menu->index = e.nav.down;
					}
				}
				else
				{
					menu->index = menu->lastindex + d;
					ClampMenuIndex(menu);
				}
			}
			else 
			{
				menu->lastindex = menu->index;
				if (HasCustomNav(e))
				{
					if ((e.nav.left > -1) && (v_d.x < 0))
					{
						menu->index = e.nav.left;
					}
					else if ((e.nav.right > -1) && (v_d.x > 0))
					{
						menu->index = e.nav.right;
					}
					else if ((e.nav.up > -1) && (v_d.y < 0))
					{
						menu->index = e.nav.up;
					}
					else if ((e.nav.down > -1) && (v_d.y > 0))
					{
						menu->index = e.nav.down;
					}
				}
				else
				{
					menu->index += d;
					ClampMenuIndex(menu);
				}
			}
		}
	} // linear case

    if (confirm)
    {
		if (menu->index != -1)
        	(*(menu->elements[menu->index].onSelect))();
    }
}

void DrawMenu(Menu menu)
{
	int sz = (menu.isGridMenu) ? menu.sz.x * menu.sz.y : menu._sz;
    for(int i = 0; i < sz; i++)
        DrawUIElement(menu.elements[i], menu.index == i);
}

UIElement ElementAt(Menu menu, int x, int y)
{
	if (menu.isGridMenu)
	{
		x = x % (int)menu.sz.x; // bounds for x and y. for valid input,
		y = y % (int)menu.sz.y; // this changes nothing
		
		return menu.elements[(y * (int)menu.sz.x) + x];
	}
	else
		return *(menu.elements);
}

UIElement* ElementRefAt(Menu menu, int x, int y)
{
	if (menu.isGridMenu)
	{
		x = x % (int)menu.sz.x; // bounds for x and y. for valid input,
		y = y % (int)menu.sz.y; // this changes nothing
		
		return menu.elements + ((y * (int)menu.sz.x) + x);
	}
	else
		return menu.elements;
}

void SetNavUp(UIElement* e, signed char i)
{
	UIElementNav n = e->nav;
	n.up = i;
	e->nav = n;
}

void SetNavRight(UIElement* e, signed char i)
{
	UIElementNav n = e->nav;
	n.right = i;
	e->nav = n;
}

void SetNavDown(UIElement* e, signed char i)
{
	UIElementNav n = e->nav;
	n.down = i;
	e->nav = n;
}

void SetNavLeft(UIElement* e, signed char i)
{
	UIElementNav n = e->nav;
	n.left = i;
	e->nav = n;
}

void SetNav(UIElement* e, signed char right, signed char down, signed char left, signed char up)
{
	e->nav = (UIElementNav){right, down, left, up};
}

bool HasCustomNav(UIElement e)
{
	return ((e.nav.down > -1) || (e.nav.up > -1)) || ((e.nav.left > -1) || (e.nav.right > -1));
}
