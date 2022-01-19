/**
 * \file menu.h
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef MENU_H_
#define MENU_H_

#include "raylib.h"

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

/**
 * \brief A struct that contains override data for menu navigation. Specify indices for any of the components to override where navigating OFF a specific element leads.
 * \details Every UIElement has a UIElementNav struct with -1 for all values. When a value is set, for example left, the user navigating left of this element will lead to the specified index.
 * These values are meaningless without the context of a menu, but then again such is the case for UIElements in general.
 */
typedef struct UIElementNav
{
	signed char right;
	signed char down;
	signed char left;
	signed char up;
} UIElementNav;

typedef enum ElementDrawMode
{
	DM_SPRITE = 1, DM_RECT = 2, DM_TEXT = 4, DM_NINESLICE = 8
} ElementDrawMode;

typedef enum ElementHighlightMode
{
	HM_COLOR, HM_SPRITESWAP
} ElementHighlightMode;

typedef struct UIElement
{
    Rectangle rectangle;
    char msg[64];
	signed char fontsize;
    void (*onSelect)(void);
    Color color;
    Color textColor;
    Color highlightedColor;

	Texture2D sprite;

	ElementDrawMode drawmode;
	NPatchInfo nfo;

	ElementHighlightMode highlightmode;
	Texture2D highlightedsprite;
    
    bool isEmpty;
    UIElementNav nav;
} UIElement;

typedef enum MenuWrapBehaviour
{
	MW_WRAP = 1, MW_FALL = 2, MW_RISE = 4, MW_NONE = 8
} MenuWrapBehaviour;

typedef struct Menu
{
    UIElement* elements;
    Vector2 sz;
    unsigned char _sz;
    signed char index;
    MenuWrapBehaviour wrapbehaviour;

	bool isGridMenu;

	signed char lastindex;
	bool currentindexsetbymouse;
	bool mousedisengaged;
} Menu;

// UIElement Functions
/**
 * \brief Creates a Button UIElement.
 */
UIElement CreateUIElementButton(Rectangle, char*, void (*func)(void));
/**
 * \brief Create a Empty UI Element object
 */
UIElement CreateEmptyUIElement(void);
/**
 * \brief Set the colors of the UIElement
 */
void SetUIElementColors(UIElement*, Color, Color, Color);
/**
 * \brief Set the UIElement's onSelect behaviour.
 */
void SetUIElementBehaviour(UIElement*, void (*func)(void));
/**
 * \brief Draw the UIElement.
 */
void DrawUIElement(UIElement, bool);

// Menu Functions
/**
 * \brief Manage a valid index state. 
 */
void ClampMenuIndex(Menu*);
void ClampMenuGrid(Menu*, Vector2);
/**
 * \brief Create a Menu from a UIElement list.
 */
Menu CreateMenu(UIElement*, unsigned char);
/**
 * \brief Create a Menu from a UIElement list.
 */
Menu CreateGridMenu(UIElement*, Vector2);
/**
 * \brief Perform the update function of each UIElement in the menu.
 */
void UpdateMenu(Menu*);
/**
 * \brief Draw each UIElement in the menu.
 */
void DrawMenu(Menu);
/**
 * \brief Returns the element at the specified x, y position
 */
UIElement ElementAt(Menu, int, int);
/**
 * \brief Returns a reference to the element at the specified x, y position
 */
UIElement* ElementRefAt(Menu, int, int);

void SetNavUp(UIElement*, signed char);
void SetNavRight(UIElement*, signed char);
void SetNavDown(UIElement*, signed char);
void SetNavLeft(UIElement*, signed char);
/**
 * \brief Sets what element navigating FROM this element will lead to, for each direction.
 */
void SetNav(UIElement*, signed char right, signed char down, signed char left, signed char up);
bool HasCustomNav(UIElement);

#endif
