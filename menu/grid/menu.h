#ifndef MENU_H_
#define MENU_H_

/*
laying down some ground rules:
2D Menus do not use 2D arrays. It is a single 1D array
of size W * H. Indexes are left to right, top to bottom, i.e.
0 1 2
3 4 5
6 7 8
For ease of access, UIElement ElementAt(Menu menu, int x, int y)
and                 UIElement* ElementRefAt(Menu menu, int x, int y)
can be used to navigate the list.
*/

typedef struct UIElement
{
    Rectangle rectangle;
    char msg[64];
    void (*onSelect)(void);
    Color color;
    Color textColor;
    Color highlightedColor;
    
    bool isEmpty; // used to manage 2D menus properly
    
} UIElement;

typedef struct Menu
{
    UIElement* elements;
    Vector2 size;
    int index;
    bool wraps;
} Menu;

// UIElement Functions
UIElement CreateUIElementButton(Rectangle, char*, void (*func)(void));
UIElement CreateEmptyUIElement();
void SetUIElementColors(UIElement*, Color, Color, Color);
void SetUIElementBehaviour(UIElement*, void (*func)(void));
void DrawUIElement(UIElement, bool);

// Menu Functions
Menu CreateMenu(UIElement*, Vector2);
void UpdateMenu(Menu*, Vector2, bool);
void DrawMenu(Menu);
UIElement ElementAt(Menu, int, int);
UIElement* ElementRefAt(Menu, int, int);

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

Menu CreateMenu(UIElement* elements, Vector2 size)
{
    Menu menu;
	int sz = size.x * size.y;
    
    // reallocate the elements data so that after calling CreateMenu the original list is freed automatically.
    UIElement* _elements = (UIElement*)malloc(sizeof(UIElement) * (sz));
    for(int i = 0; i < sz; i++)
        _elements[i] = elements[i];
    // free the original list
    free(elements);
    
    menu.elements = _elements;
    menu.size = size;
    menu.index = 0;
    menu.wraps = true;
    return menu;
}

void UpdateMenu(Menu* menu, Vector2 indexDelta, bool confirm)
{
    // confirm takes priority over menu navigation
    if (confirm)
    {
        (*(menu->elements[menu->index].onSelect))();
    }
    else
    {
		int sz = menu->size.x * menu->size.y;
        menu->index += indexDelta.x + (indexDelta.y * menu->size.x);
        if (menu->index >= sz)
            menu->index = (menu->wraps) ? (menu->index % sz) : sz - 1;
        else if (menu->index < 0)
            menu->index = (menu->wraps) ? sz + menu->index : 0;
		
		// if land on an empty square recursive call to apply the input again
		if (menu->elements[menu->index].isEmpty && !(indexDelta.x == 0 && indexDelta.y == 0))
			UpdateMenu(menu, indexDelta, confirm);
    }
}

void DrawMenu(Menu menu)
{
	int sz = menu.size.x * menu.size.y;
    for(int i = 0; i < sz; i++)
        DrawUIElement(menu.elements[i], menu.index == i);
}

UIElement ElementAt(Menu menu, int x, int y)
{
    x = x % (int)menu.size.x; // bounds for x and y. for valid input,
    y = y % (int)menu.size.y; // this changes nothing
    
    return menu.elements[(y * (int)menu.size.x) + x];
}

UIElement* ElementRefAt(Menu menu, int x, int y)
{
    x = x % (int)menu.size.x; // bounds for x and y. for valid input,
    y = y % (int)menu.size.y; // this changes nothing
    
    return menu.elements + ((y * (int)menu.size.x) + x);
}

#endif