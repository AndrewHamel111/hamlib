#include "raylib.h"
#include "menu.h"
#include <stdlib.h>
#include <string.h>

#define BUTTONS_LENGTH 6
enum BUTTONS
{
    BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A
};
static bool buttonpressed[BUTTONS_LENGTH] = {false};

typedef signed char sc;

UIElement CreateUIElementButton(Rectangle rectangle, char* msg, void (*func)(void))
{
    UIElement element;
    element.rectangle = rectangle;
    strcpy(element.msg, msg);
    element.onSelect = func;
    
    element.color = (Color){255,255,255,255};
    element.textColor = (Color){15,15,15,255};
    element.highlightedColor = (Color){220,220,220,255};
    
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
    DrawRectangleRec(element.rectangle, isSelected ? element.highlightedColor : element.color);
    DrawText(element.msg, element.rectangle.x + 5, element.rectangle.y + 5, element.rectangle.height/2, element.textColor);
}

Menu CreateMenu(UIElement* elements, int elementCount)
{
    Menu menu;
    
    // reallocate the elements data so that after calling CreateMenu the original list is freed automatically.
    UIElement* _elements = (UIElement*)malloc(sizeof(UIElement) * elementCount);
    for(int i = 0; i < elementCount; i++)
        _elements[i] = elements[i];
    // free the original list
    free(elements);
    
    menu.elements = _elements;
    menu.sz = elementCount;
    menu.index = 0;
    menu.wraps = true;
    return menu;
}

void ClampMenuIndex(Menu* menu)
{
	if (menu->index >= menu->sz)
		menu->index = (menu->wraps) ? 0 : (menu->sz - 1);
	else if (menu->index < 0)
		menu->index = (menu->wraps) ? (menu->sz - 1) : 0;
}

void UpdateMenu(Menu* menu)
{
	buttonpressed[BUTTON_UP] = IsKeyPressed(KEY_W); // poll inputs
	buttonpressed[BUTTON_DOWN] = IsKeyPressed(KEY_S);
	buttonpressed[BUTTON_LEFT] = IsKeyPressed(KEY_A);
	buttonpressed[BUTTON_RIGHT] = IsKeyPressed(KEY_D);
	buttonpressed[BUTTON_B] = IsKeyPressed(KEY_L);
	buttonpressed[BUTTON_A] = IsKeyPressed(KEY_SEMICOLON);
	
	sc d = (sc)(buttonpressed[BUTTON_DOWN] || buttonpressed[BUTTON_RIGHT]) - (sc)(buttonpressed[BUTTON_UP] || buttonpressed[BUTTON_LEFT]);

    // confirm takes priority over menu navigation
    if (buttonpressed[BUTTON_B] || buttonpressed[BUTTON_A])
    {
        (*(menu->elements[menu->index].onSelect))();
    }
    else
    {
		//menu->index += (sc)(buttonpressed[BUTTON_DOWN] || buttonpressed[BUTTON_RIGHT]) ;
		//menu->index -= (sc)(buttonpressed[BUTTON_UP] || buttonpressed[BUTTON_LEFT]) ;
		menu->index += d;

		ClampMenuIndex(menu);
    }
}

void DrawMenu(Menu menu)
{
    for(int i = 0; i < menu.sz; i++)
        DrawUIElement(menu.elements[i], menu.index == i);
}
