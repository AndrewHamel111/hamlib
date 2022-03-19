/**
 * \file menu.h
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 */

#ifndef HAMLIB_MENU_H
#define HAMLIB_MENU_H

#include "raylib.h"

/*
laying down some ground rules:
2D Menus do not use 2D arrays. It is a single 1D array
of size W * H. Indexes are left to right, top to bottom, i.e.
0 1 2
3 4 5
6 7 8
For ease of access, uiElement elementAt(menu menu, int x, int y)
and                 uiElement* elementRefAt(menu menu, int x, int y)
can be used to navigate the list.
*/

/**
 * \brief A struct that contains override data for menu navigation. Specify indices for any of the components to override where navigating OFF a specific element leads.
 * \details Every uiElement has a uiElementNav struct with -1 for all values. When a value is set, for example left, the user navigating left of this element will lead to the specified index.
 * These values are meaningless without the context of a menu, but then again such is the case for UIElements in general.
 */
typedef struct uiElementNav
{
	signed char right;
	signed char down;
	signed char left;
	signed char up;
} uiElementNav;

typedef enum elementDrawMode
{
	DmSprite = 1, DmRect = 2, DmText = 4, DmNineslice = 8
} elementDrawMode;

typedef enum elementHighlightMode
{
	HmColor, HmSpriteswap
} elementHighlightMode;

typedef struct uiElement
{
    Rectangle rectangle;
    char msg[64];
	signed char fontsize;
    void (*onSelect)(void);
    Color color;
    Color textColor;
    Color highlightedColor;

	Texture2D sprite;

	elementDrawMode drawmode;
	NPatchInfo nfo;

	elementHighlightMode highlightmode;
	Texture2D highlightedsprite;
    
    bool isEmpty;
    uiElementNav nav;
} uiElement;

typedef enum menuWrapBehaviour
{
	MwWrap = 1, MwFall = 2, MwRise = 4, MwNone = 8
} menuWrapBehaviour;

typedef struct menu
{
    uiElement* elements;
    Vector2 sz;
    unsigned char elementsCount;
    signed char index;
    menuWrapBehaviour wrapbehaviour;

	bool isGridMenu;

	signed char lastindex;
	bool currentindexsetbymouse;
	bool mousedisengaged;
} menu;

// uiElement Functions
/**
 * \brief Creates a button uiElement.
 */
uiElement createUiElementButton(Rectangle rectangle, char* string, void (*func)(void));
/**
 * \brief Create a Empty UI Element object
 */
uiElement createEmptyUiElement(void);
/**
 * \brief set the colors of the uiElement
 */
void setUiElementColors(uiElement* element, Color color, Color textColor, Color highlightedColor);
/**
 * \brief set the uiElement's onSelect behaviour.
 */
void setUiElementBehaviour(uiElement* element, void (*func)(void));
/**
 * \brief Draw the uiElement.
 */
void drawUiElement(uiElement element, bool isSelected);

// menu Functions
/**
 * \brief Manage a valid index state. 
 */
void clampMenuIndex(menu* mnu);
void clampMenuGrid(menu* mnu, Vector2 d);
/**
 * \brief Create a menu from a uiElement list.
 */
menu createMenu(uiElement* element, unsigned char sz);
/**
 * \brief Create a menu from a uiElement list.
 */
menu createGridMenu(uiElement* element, Vector2 gridSz);
/**
 * \brief Perform the update function of each uiElement in the menu.
 */
void updateMenu(menu* mnu);
/**
 * \brief Draw each uiElement in the menu.
 */
void drawMenu(menu mnu);
/**
 * \brief Returns the element at the specified x, y position
 */
uiElement elementAt(menu mnu, int x, int y);
/**
 * \brief Returns a reference to the element at the specified x, y position
 */
uiElement* elementRefAt(menu mnu, int x, int y);

void setNavUp(uiElement* element, signed char i);
void setNavRight(uiElement* element, signed char i);
void setNavDown(uiElement* element, signed char i);
void setNavLeft(uiElement* element, signed char i);
/**
 * \brief Sets what element navigating FROM this element will lead to, for each direction.
 */
void setNav(uiElement* element, signed char right, signed char down, signed char left, signed char up);
bool hasCustomNav(uiElement element);

#endif
