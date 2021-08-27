 /*******************************************************************************************

    TODO extend this menu example to a 2D menu example
    
********************************************************************************************/

// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"

#include "menu.h"

// UIElement handlers
void Button1OnSelect();
void Button2OnSelect();
void Button3OnSelect();

int main(void)
{
    
//////--------------------------------------------------------------------------------------
////// Initialization
//////--------------------------------------------------------------------------------------
	
	InitWindow(400, 240, "Menus Demo"); // create window
    SetTargetFPS(50);
    
    UIElement* elements = (UIElement*)malloc(sizeof(UIElement) * 3); // create the menu
    elements[0] = CreateUIElementButton((Rectangle){10, 10, 200, 66}, "Button 1", &Button1OnSelect);
    elements[1] = CreateUIElementButton((Rectangle){10, 86, 200, 66}, "Button 2", &Button2OnSelect);
    elements[2] = CreateUIElementButton((Rectangle){10 ,162, 200, 66}, "Button 3", &Button3OnSelect);
    Menu menu;
    menu = CreateMenu(elements, 3);

	while (!WindowShouldClose()) // loop
	{		
    
//////--------------------------------------------------------------------------------------
////// PRE-UPDATE (INPUT)
//////--------------------------------------------------------------------------------------
    
        
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

void Button1OnSelect()
{
    
}

void Button2OnSelect()
{
    
}

void Button3OnSelect()
{
    
}
