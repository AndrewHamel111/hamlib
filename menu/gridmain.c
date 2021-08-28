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

Vector2 GetInputVector(bool* buttonPressed)
{
    return (Vector2){(int)buttonPressed[BUTTON_RIGHT] - (int)buttonPressed[BUTTON_LEFT], (int)buttonPressed[BUTTON_DOWN] - (int)buttonPressed[BUTTON_UP]};
}

int main(void)
{
    
//////--------------------------------------------------------------------------------------
////// Initialization
//////--------------------------------------------------------------------------------------
	
	InitWindow(400, 240, "Menus Demo"); // create window
    SetTargetFPS(50);

    bool buttonPressed[BUTTONS_LENGTH] = {false}; // see enum BUTTONS
    
    UIElement* elements = (UIElement*)malloc(sizeof(UIElement) * 12); // create the menu
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
    Menu menu;
    menu = CreateGridMenu(elements, (Vector2){4,3});

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
        
//////--------------------------------------------------------------------------------------
////// UPDATE
//////--------------------------------------------------------------------------------------
		
        UpdateMenu(&menu);
        
//////--------------------------------------------------------------------------------------
////// DRAW
//////--------------------------------------------------------------------------------------
		BeginDrawing();
            
            ClearBackground((Color){240,240,240,255});
            
            DrawMenu(menu);
            
		EndDrawing();
	} // /DRAW 
    
    CloseWindow();

	return 0;
}

void ButtonOnSelect()
{
    
}
