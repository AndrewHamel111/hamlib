#include "menu.h"
#include <stdlib.h>
#include <string.h>

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
    element.onSelect = func;
    
    element.color = (Color){255,255,255,255};
    element.textColor = (Color){15,15,15,255};
    element.highlightedColor = (Color){255,220,255,255};
	
	element.isEmpty = false;
    
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
	
    DrawRectangleRec(element.rectangle, isSelected ? element.highlightedColor : element.color);
    DrawText(element.msg, element.rectangle.x + 5, element.rectangle.y + 5, element.rectangle.height/2, element.textColor);
}

Menu CreateMenu(UIElement* elements, unsigned char sz)
{
    Menu menu;
    
    // reallocate the elements data so that after calling CreateMenu the original list is freed automatically.
    UIElement* _elements = (UIElement*)malloc(sizeof(UIElement) * (menu._sz));
    for(int i = 0; i < sz; i++)
        _elements[i] = elements[i];
    // free the original list
    free(elements);
    
    menu.elements = _elements;
	menu._sz = sz;
    menu.index = 0;
    menu.wraps = true;
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
    menu.wraps = true;
	menu.isGridMenu = true;
	
	menu.lastindex = 0;
	menu.currentindexsetbymouse = false;
	menu.mousedisengaged = false;

    return menu;
}

void ClampMenuIndex(Menu* menu, signed char flat_d)
{
	if (menu->isGridMenu)
	{
		if (menu->index >= menu->_sz)
			menu->index = (menu->wraps) ? (menu->index % menu->_sz) : menu->_sz - 1;
		else if (menu->index < 0)
			menu->index = (menu->wraps) ? menu->_sz + menu->index : 0;

		// if land on an empty square recursive call to apply the input again
		while(menu->elements[menu->index].isEmpty && !(flat_d != 0))
		{
			menu->index += flat_d;

			if (menu->index >= menu->_sz)
				menu->index = (menu->wraps) ? (menu->index % menu->_sz) : menu->_sz - 1;
			else if (menu->index < 0)
				menu->index = (menu->wraps) ? menu->_sz + menu->index : 0;
		}
	}
	else 
	{
		if (menu->index >= menu->_sz)
			menu->index = (menu->wraps) ? 0 : (menu->_sz - 1);
		else if (menu->index < 0)
			menu->index = (menu->wraps) ? (menu->_sz - 1) : 0;
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
	
	if (menu->isGridMenu)
	{ // grid case
		Vector2 d = (Vector2){(signed char)buttonpressed[BUTTON_RIGHT] - (signed char)buttonpressed[BUTTON_LEFT], (signed char)buttonpressed[BUTTON_DOWN] - (signed char)buttonpressed[BUTTON_UP]};

		// check for mouse navigation
		bool flag = false;
		for(char i = 0; i < menu->_sz; i++)
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
			menu->mousedisengaged = false;
			menu->currentindexsetbymouse = false;
			if (menu->index == -1)
			{
				menu->index = menu->lastindex + flat_d;
				ClampMenuIndex(menu, flat_d);
			}
			else
			{
				menu->lastindex = menu->index;
				menu->index += flat_d;
				ClampMenuIndex(menu, flat_d);
			}
		}
	} // grid case
	else
	{ // linear case
		signed char d = (signed char)(buttonpressed[BUTTON_DOWN] || buttonpressed[BUTTON_RIGHT]) - (signed char)(buttonpressed[BUTTON_UP] || buttonpressed[BUTTON_LEFT]);

		// check for mouse navigation
		bool flag = false;
		for(char i = 0; i < menu->_sz; i++)
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
			menu->mousedisengaged = false;
			menu->currentindexsetbymouse = false;
			if (menu->index == -1)
			{
				menu->index = menu->lastindex + d;
				ClampMenuIndex(menu, d);
			}
			else
			{
				menu->lastindex = menu->index;
				menu->index += d;
				ClampMenuIndex(menu, d);
			}
		}
	} // linear case

    if (buttonpressed[BUTTON_B] || buttonpressed[BUTTON_A] || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
		if (menu->index != -1)
        	(*(menu->elements[menu->index].onSelect))();
		else
			menu->index = menu->lastindex;
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