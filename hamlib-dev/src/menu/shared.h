#ifndef HAMLIB_DEV_SHARED_H
#define HAMLIB_DEV_SHARED_H

extern Texture2D texButton;
extern NPatchInfo nfoButton;
extern Texture2D texSliderButton;
extern NPatchInfo nfoSliderButton;
extern float frametime;
extern Vector2 mousePosition;
extern bool mouseIsPressed;
extern bool mouseIsDown;

extern elementDrawOverride newElementDrawOverride;
extern Font newElementFont;
extern char* newElementAllowedCharacters;

void getMenuElementPositionAndSize(menuElement e, menu m, Vector2* position, Vector2* size);
void getMenuElementTextArea(menuElement e, menu m, Vector2* position, Vector2* size);
bool typeUsesLightHighlight(enum elementType type);

/// \brief Call with null menuElement to remove highlight
void menuHighlightElement(menuElement* element, menu* mnu, bool highlightedByMouse);
void menuHighlightNone(menu *mnu);

void handleMenuTransition(menu* mnu);

#endif