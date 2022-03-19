#ifndef HAMLIB_DEV_MENU_H
#define HAMLIB_DEV_MENU_H

// TODO find a better solution for the hardcoded arrays that isn't a linked-list.

// TODO Light Tasks
// TODO menuAdd returns a pointer to the added menu element
// TODO add allowed background and fill sprites to Slider
// TODO add background sprite to Textfield
// TODO add an element name field so that menu elements can be more easily identified in debugging and code

// TODO Medium Tasks
// TODO Notched Slider that snaps to integer values (Caller specifies a range, and slider button / circle / bar will only fill to the absolute points)
// TODO Buttons and Sliders can have their respective sprites added after creation. When no sprites are specified default primitives are used.
// TODO Textfields can be navigated within to make changes to the middle of the field
// TODO Add Label Text Alignment support
// TODO menuElement font sizes should only be a "max" size, the size actually automatically determined by the element.

// TODO Heavy Tasks
// TODO Layout groups to menus (Horizontal and Vertical Groups)
// TODO Write a logging system. Comments that indicate a log should take place there will start with LOG
// TODO Gamepad / Arrow Key support. Look to my nav example from previous iteration of menu system.

// TODO Menu Management
// TODO Menu Stack
// TODO Menu Tweening Options
// TODO Menu Editor that exports the code to create an editor

// QUESTIONS AND IDEAS
// Do we need a OnTextfieldEdited that is called every time the user navigates off of a textfield or is onHighlightedOff sufficient?
// Consider replacing temporary hardcoded array in struct menu with a light struct that manages a dynamic array.
// Allow Buttons to have a normalized collision rect that defines an extended area outside the draw area where the button can be pressed. Only mousePressed will matter in this rect, any highlight logic will not be affected by this
// When the menu has "nothing" selected, it should keep a reference to the last thing it had selected so that when the user attemps an input it will nav the user based on that. IF for some reason it can't access the nav data of that element or perhaps something is null, fall back to the first interactable element in the menu.
// Tooltip support, when a user hovers an element for a full second a popup text box appears explaining what that element does if element.showTooltip is true.

#include "raylib.h"
#include "hamlib.h"
#include "hamlib/draw.h"
#include "hamlib/animation.h"

#define MET_MAX_LABEL_LENGTH 128
#define MENU_LENGTH 8

typedef void (*voidAction)(void);
typedef voidAction buttonAction;
typedef void (*sliderAction)(float);

// SDT_BAR like a progress bar | SDT_CIRCLE little circle on a bar | SDT_BUTTON uses texSliderButton and nfoSliderButton
typedef enum sliderDisplayEnum
{
	SdtBar, SdtCircle, SdtButton
} sliderDisplayEnum;

struct menuElement; // forward declaration of menuElement type
typedef struct menuElement menuElement;

typedef void (*voidElementAction)(menuElement*); // action typedefs

typedef struct elementDrawData
{
	Vector2 position;
	Vector2 size;
	Rectangle rect;
	Vector2 origin;
	
	Vector2 shadowPosition;
	Rectangle shadowRect;
	Color shadowColor;
	
	bool shadow;
	bool dip;
} elementDrawData;
typedef void (*elementDrawOverride)(menuElement*, elementDrawData); // builtin functionality overrides


struct menuElement // buttons, labels, sliders, images, and more
{
	char elementName[64];
	// A normalized position relative to the parent menu
	Vector2 position;
	// A normalized size relative to the parent menu
	Vector2 size;
	// Determines whether this element should listen for input
	bool interactable;
	// Determines whether this element should be drawn
	bool hidden;
	// Determines whether this element is highlighted
	bool highlighted;
	// In pixels, determines how far below the element the shadow should be. Use 0 for no shadow
	float shadowHeight;
	
	// An action that is performed when the element is navigated onto, or off of.
	voidElementAction onElementHighlighted;
	voidElementAction onElementHighlightedOff;
	
	elementDrawOverride drawFunctionOverride; // builtin functionality overrides
	
	enum elementType
	{
		MetLabel, MetImage, MetAnimation, MetButton, MetSlider, MetTextfield
	} menuElementType;
	
	union
	{
		struct // LABEL
		{
			char labelText[MET_MAX_LABEL_LENGTH];
			Color labelTextColor;
			// Percent of the element's height that should be used for font
			float labelTextFontSize;
			Font labelTextFont;
		};
		
		struct // IMAGE
		{
			Texture2D imageTexture;
			Rectangle imageSourceRectangle;
			Color imageTint;
		};
		
		struct // ANIMATION
		{
			spriteAnimation animationData;
			Color animationTint;
		};
		
		struct // BUTTON
		{
			char buttonText[MET_MAX_LABEL_LENGTH];
			Color buttonTextColor;
			// Percent of the element's height that should be used for font
			float buttonTextFontSize;
			Font buttonTextFont;
			Color buttonTint;
			
			buttonAction buttonOnPressed;
		};
		
		struct // SLIDER
		{
			char sliderText[MET_MAX_LABEL_LENGTH];
			Color sliderTextColor;
			float sliderTextFontSize;
			Font sliderTextFont;
			
			Color sliderFillColor;
			Color sliderBackgroundColor;
			// Also used as button color for SDT_BUTTON
			Color sliderOutlineColor;
			float sliderOutlineWeight;
			
			float sliderValue;
			
			sliderDisplayEnum sliderDisplayType;
			
			sliderAction sliderOnChanged;
		};
		
		struct // TEXTFIELD
		{
			char textfieldTextLabel[MET_MAX_LABEL_LENGTH];
			char* textfieldTextInput;
			int textfieldInputMaxLength;
			
			Font textfieldTextLabelFont;
			float textfieldTextLabelFontSize;
			Font textfieldTextInputFont;
			
			Color textfieldBackgroundColor;
			Color textfieldHighlightColor;
			float textfieldOutlineWeight;
			Color textfieldTextLabelColor;
			Color textfieldTextInputColor;
			
			char* textfieldAllowedCharacters;
		};
	};
};

typedef struct menu
{
	Vector2 position;
	Vector2 size;
	
	menuElement list[MENU_LENGTH];
	bool elementIsValid[MENU_LENGTH];
	
	bool lastInteractionWasMouse;
	menuElement* highlightedElement;
} menu;

menu menuCreate(Vector2 position, Vector2 size);
void menuUpdate(menu* mnu);
void menuDraw(menu mnu);
void menuAdd(menu* mnu, menuElement element);

menuElement menuElementCreateLabel(float positionX, float positionY, float sizeX, float sizeY, char* label, float fontSize, Color color);
menuElement menuElementCreateImage(float positionX, float positionY, float sizeX, float sizeY, Texture2D image, Rectangle sourceRectangle, Color tint);
menuElement menuElementCreateAnimation(float positionX, float positionY, float sizeX, float sizeY, spriteAnimation animation, Color tint);
menuElement menuElementCreateButton(float positionX, float positionY, float sizeX, float sizeY, Color buttonTint, buttonAction onPressed, char* buttonText, Color buttonTextColor, float buttonTextFontSize);
menuElement menuElementCreateSlider(float positionX, float positionY, float sizeX, float sizeY, Color sliderBackgroundColor, Color sliderFillColor, Color sliderExtraColor, float initialValue, sliderDisplayEnum displayType, sliderAction onSliderValueChanged);
menuElement menuElementCreateTextfield(float positionX, float positionY, float sizeX, float sizeY, char* label, float labelFontSize, char* value, int maxLength, Color backgroundColor, Color highlightColor, Color labelColor, Color inputTextColor);

menuElement menuElementCreateLabelV(Vector2 position, Vector2 size, char* label, float fontSize, Color color);
menuElement menuElementCreateImageV(Vector2 position, Vector2 size, Texture2D image, Rectangle sourceRectangle, Color tint);
menuElement menuElementCreateAnimationV(Vector2 position, Vector2 size, spriteAnimation animation, Color tint);
menuElement menuElementCreateButtonV(Vector2 position, Vector2 size, Color buttonTint, buttonAction onPressed, char* buttonText, Color buttonTextColor, float buttonTextFontSize);
menuElement menuElementCreateSliderV(Vector2 position, Vector2 size, Color sliderBackgroundColor, Color sliderFillColor, Color sliderExtraColor, float initialValue, sliderDisplayEnum displayType, sliderAction onSliderValueChanged);
menuElement menuElementCreateTextfieldV(Vector2 position, Vector2 size, char* label, float labelFontSize, char* value, int maxLength, Color backgroundColor, Color highlightColor, Color labelColor, Color inputTextColor);

/// Override default values for uncommon fields, only applies to menuElements created after this set call. Set a parameter to NULL to unset a previous value and guarantee use of the default functionality.
/// \param drawFunction
void setMenuElementOverrides(elementDrawOverride drawFunction);

void setMenuElementFont(Font font);
void setTextFieldAllowedCharacters(char* allowedCharacters);

#endif