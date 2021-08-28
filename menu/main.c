// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"

#include "menu.h"

#define BUTTONS_LENGTH 6
enum BUTTONS
{
    BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A
};

// UIElement handlers
void ButtonOnSelect();

Menu menus[4];
unsigned char menuindex = 0;

void InitMenus();
void NextMenu();

int main(void)
{
    
//////--------------------------------------------------------------------------------------
////// Initialization
//////--------------------------------------------------------------------------------------
	
	InitWindow(800, 600, "Menus Demo"); // create window
    SetTargetFPS(50);

    bool buttonPressed[BUTTONS_LENGTH] = {false}; // see enum BUTTONS

	InitMenus();
    

	while (!WindowShouldClose()) // loop
	{		
    
//////--------------------------------------------------------------------------------------
////// PRE-UPDATE (INPUT)
//////--------------------------------------------------------------------------------------
    
        buttonPressed[BUTTON_UP]    = IsKeyPressed(KEY_W); // poll inputs
        buttonPressed[BUTTON_DOWN]  = IsKeyPressed(KEY_S);
        buttonPressed[BUTTON_LEFT]  = IsKeyPressed(KEY_A);
        buttonPressed[BUTTON_RIGHT] = IsKeyPressed(KEY_D);
        buttonPressed[BUTTON_B]     = IsKeyPressed(KEY_L);
        buttonPressed[BUTTON_A]     = IsKeyPressed(KEY_SEMICOLON);

		if (IsKeyPressed(KEY_SPACE))
			NextMenu();
        
//////--------------------------------------------------------------------------------------
////// UPDATE
//////--------------------------------------------------------------------------------------
		
        UpdateMenu(menus + menuindex);
        
//////--------------------------------------------------------------------------------------
////// DRAW
//////--------------------------------------------------------------------------------------
		BeginDrawing();
            
            ClearBackground((Color){240,240,240,255});
            
            DrawMenu(menus[menuindex]);
            
		EndDrawing();
	} // /DRAW 
    
    CloseWindow();

	return 0;
}

void ButtonOnSelect()
{
    
}

void InitMenus()
{
    UIElement* elements;

	// linear menu 1
	elements = (UIElement*)malloc(sizeof(UIElement) * 3);
    elements[0] = CreateUIElementButton((Rectangle){10, 10, 300, 100}, "Button 1", &ButtonOnSelect);
    elements[1] = CreateUIElementButton((Rectangle){10, 120, 300, 100}, "Button 2", &ButtonOnSelect);
    elements[2] = CreateUIElementButton((Rectangle){10 ,450, 300, 100}, "Button 3", &ButtonOnSelect);
    menus[0] = CreateMenu(elements, 3);
	
	// linear menu 2
	elements = (UIElement*)malloc(sizeof(UIElement) * 4);
    elements[0] = CreateUIElementButton((Rectangle){10, 490, 100, 100}, "1", &ButtonOnSelect);
    elements[1] = CreateUIElementButton((Rectangle){470, 490, 100, 100}, "2", &ButtonOnSelect);
    elements[2] = CreateUIElementButton((Rectangle){580 ,490, 100, 100}, "3", &ButtonOnSelect);
    elements[3] = CreateUIElementButton((Rectangle){690 ,490, 100, 100}, "4", &ButtonOnSelect);
    menus[1] = CreateMenu(elements, 4);

	// grid menu 1
    elements = (UIElement*)malloc(sizeof(UIElement) * 6);
    elements[0] = CreateUIElementButton((Rectangle){10, 10, 200, 100}, "Butt. 1", &ButtonOnSelect);
    elements[1] = CreateUIElementButton((Rectangle){380, 10, 200, 100}, "Butt. 2", &ButtonOnSelect);
    elements[2] = CreateUIElementButton((Rectangle){590, 10, 200, 100}, "Butt. 3", &ButtonOnSelect);
	elements[3] = CreateEmptyUIElement();
	elements[4] = CreateEmptyUIElement();
    elements[5] = CreateUIElementButton((Rectangle){590, 450, 200, 100}, "Butt. 4", &ButtonOnSelect);
    menus[2] = CreateGridMenu(elements, (Vector2){3,2});

	// grid menu 2
    elements = (UIElement*)malloc(sizeof(UIElement) * 12);
    elements[0] = CreateUIElementButton((Rectangle){10, 10, 66, 66}, "1", &ButtonOnSelect);
    elements[1] = CreateUIElementButton((Rectangle){110, 10, 66, 66}, "2", &ButtonOnSelect);
    elements[2] = CreateUIElementButton((Rectangle){210, 10, 66, 66}, "3", &ButtonOnSelect);
    elements[3] = CreateUIElementButton((Rectangle){310, 10, 66, 66}, "4", &ButtonOnSelect);
	
    elements[4] = CreateUIElementButton((Rectangle){10, 86, 66, 66}, "5", &ButtonOnSelect);
    elements[5] = CreateUIElementButton((Rectangle){110, 86, 66, 66}, "6", &ButtonOnSelect);
    elements[6] = CreateUIElementButton((Rectangle){210, 86, 66, 66}, "7", &ButtonOnSelect);
    elements[7] = CreateUIElementButton((Rectangle){310, 86, 66, 66}, "8", &ButtonOnSelect);
	
    elements[8] = CreateUIElementButton((Rectangle){10 ,162, 66, 66}, "9", &ButtonOnSelect);
    elements[9] = CreateUIElementButton((Rectangle){110 ,162, 66, 66}, "10", &ButtonOnSelect);
    elements[10] = CreateUIElementButton((Rectangle){210 ,162, 66, 66}, "11", &ButtonOnSelect);
    elements[11] = CreateUIElementButton((Rectangle){310, 162, 66, 66}, "12", &ButtonOnSelect);
	
	elements[1].isEmpty = true;
	elements[2].isEmpty = true;
	elements[10].isEmpty = true;
    menus[3] = CreateGridMenu(elements, (Vector2){4,3});
}

void NextMenu()
{
	menuindex++;
	if (menuindex == 4)
		menuindex = 0;
}