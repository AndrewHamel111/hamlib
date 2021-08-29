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
	// main menu
    UIElement* elements = (UIElement*)malloc(sizeof(UIElement) * 5);
	elements[0] = CreateUIElementButton((Rectangle){100, 100, 600, 300}, "PLAY", &ButtonOnSelect);
	elements[1] = CreateUIElementButton((Rectangle){100, 500, 600, 200}, "GARAGE", &ButtonOnSelect);
	elements[2] = CreateUIElementButton((Rectangle){800, 150, 300, 150}, "LOCKED", &ButtonOnSelect);
	elements[3] = CreateUIElementButton((Rectangle){800, 350, 300, 150}, "OPTIONS", &ButtonOnSelect);
	elements[4] = CreateUIElementButton((Rectangle){800, 550, 300, 150}, "CREDITS", &ButtonOnSelect);
	/* sprite example with npatch
	Texture2D spr = LoadTexture("button.png");
	elements[0].sprite = spr;
	elements[0].nfo = (NPatchInfo){PureSource(spr), 36, 36, 36, 36, 0};
	elements[0].drawmode = DM_NINESLICE | DM_TEXT;
	elements[0].textColor = WHITE;
	*/
	menu = CreateMenu(elements, 5);
	menu.wrapbehaviour = MW_NONE;
	//						->   V  <-  ^
	SetNav(menu.elements + 0, 2, 1, 2, 1);
	SetNav(menu.elements + 1, 4, 0, 4, 0);
	SetNav(menu.elements + 2, 0, 3, 0, 4);
	SetNav(menu.elements + 3, 0, 4, 0, 2);
	SetNav(menu.elements + 4, 1, 2, 1, 3);
}
