// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"
#include "hamlib.h"

#include "menu.h"

#define BUTTONS_LENGTH 6
enum BUTTONS
{
    BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A
};

// UIElement handlers
void ButtonOnSelect();

Menu menu;

void InitMenu();

int main(void)
{
    
//////--------------------------------------------------------------------------------------
////// Initialization
//////--------------------------------------------------------------------------------------
	
	InitWindow(1200, 800, "Menus Demo"); // create window
    SetTargetFPS(50);

    bool buttonPressed[BUTTONS_LENGTH] = {false}; // see enum BUTTONS

	InitMenu();
    

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

void InitMenu()
{
	// playmenu
    UIElement* elements = (UIElement*)malloc(sizeof(UIElement) * 5);
	int w = 266, h = 200;
	elements[0] = CreateUIElementButton((Rectangle){100, 150, w, h}, "LEVEL ONE", &ButtonOnSelect);
	elements[1] = CreateUIElementButton((Rectangle){466, 150, w, h}, "CLASS CLOWN", &ButtonOnSelect);
	elements[2] = CreateUIElementButton((Rectangle){832, 150, w, h}, "AFTER-SCHOOL\nSPECIAL", &ButtonOnSelect);
	elements[3] = CreateUIElementButton((Rectangle){284, 450, w, h}, "HEAD CHEF", &ButtonOnSelect);
	elements[4] = CreateUIElementButton((Rectangle){650, 450, w, h}, "CUSTOMER\nSERVICES", &ButtonOnSelect);
	elements[0].fontsize = elements[1].fontsize = elements[2].fontsize = elements[3].fontsize = elements[4].fontsize = 30;
	menu = CreateMenu(elements, 5);
	menu.wrapbehaviour = MW_NONE;
	//						->   V  <-  ^
	SetNav(menu.elements + 0, 1, 3, -1, -1);
	SetNav(menu.elements + 1, 2, 3, 0, -1);
	SetNav(menu.elements + 2, -1, 4, 1, -1);
	SetNav(menu.elements + 3, 4, -1, 0, 1);
	SetNav(menu.elements + 4, 2, -1, 3, 1);
}
