#ifndef MENU_H_
#define MENU_H_

/**
 * \brief A structure describing the individual UI Elements.
 */
typedef struct UIElement
{
    Rectangle rectangle;
    char msg[64];
    void (*onSelect)(void);
    Color color;
    Color textColor;
    Color highlightedColor;
    
} UIElement;

/**
 * \brief A structure containing a list of UIElements that also tracks the current index.
 */
typedef struct Menu
{
    UIElement* elements;
    unsigned char sz;
    signed char index;
    bool wraps;

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
 * \brief Create a Menu from a UIElement list.
 */
Menu CreateMenu(UIElement*, int);
/**
 * \brief Assures that menu->index is within acceptable bounds, subject to the value of menu->sz. 
 */
void ClampMenuIndex(Menu* menu);
/**
 * \brief Perform the update function of each UIElement in the menu.
 */
void UpdateMenu(Menu*);
/**
 * \brief Draw each UIElement in the menu.
 */
void DrawMenu(Menu);

#endif
