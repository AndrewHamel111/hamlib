#include "menu.h"
#include "menu/shared.h"

#include <string.h>

static menuElement menuElementCreateBase(char* elementName, Vector2 position, Vector2 size, enum elementType type);

static menuElement menuElementCreateBase(char* elementName, Vector2 position, Vector2 size, enum elementType type)
{
	menuElement element = {.position = position, .size = size, .menuElementType = type, .drawFunctionOverride = newElementDrawOverride};
	
	strcpy(element.elementName, elementName);
	
	return element;
}

menuElement menuElementCreateLabel(char* elementName, float positionX, float positionY, float sizeX, float sizeY, char* label, float fontSize, Color color)
{
	return menuElementCreateLabelV(elementName, (Vector2){positionX, positionY}, (Vector2){sizeX, sizeY}, label, fontSize, color);
}

menuElement menuElementCreateImage(char* elementName, float positionX, float positionY, float sizeX, float sizeY, Texture2D image, Rectangle sourceRectangle, Color tint)
{
	return menuElementCreateImageV(elementName, (Vector2){positionX, positionY}, (Vector2){sizeX, sizeY}, image, sourceRectangle, tint);
}

menuElement menuElementCreateAnimation(char* elementName, float positionX, float positionY, float sizeX, float sizeY, spriteAnimation animation, Color tint)
{
	return menuElementCreateAnimationV(elementName, (Vector2){positionX, positionY}, (Vector2){sizeX, sizeY}, animation, tint);
}

menuElement menuElementCreateButton(char* elementName, float positionX, float positionY, float sizeX, float sizeY, Color buttonTint, buttonAction onPressed, char* buttonText, Color buttonTextColor, float buttonTextFontSize)
{
	return menuElementCreateButtonV(elementName, (Vector2){positionX, positionY}, (Vector2){sizeX, sizeY}, buttonTint, onPressed, buttonText, buttonTextColor, buttonTextFontSize);
}

menuElement menuElementCreateSlider(char* elementName, float positionX, float positionY, float sizeX, float sizeY, Color sliderBackgroundColor, Color sliderFillColor, Color sliderExtraColor, float initialValue, sliderDisplayEnum displayType, sliderAction onSliderValueChanged)
{
	return menuElementCreateSliderV(elementName, (Vector2){positionX, positionY}, (Vector2){sizeX, sizeY}, sliderBackgroundColor, sliderFillColor, sliderExtraColor, initialValue, displayType, onSliderValueChanged);
}

menuElement menuElementCreateTextfield(char* elementName, float positionX, float positionY, float sizeX, float sizeY, char* label, float labelFontSize, char* value, int maxLength, Color backgroundColor, Color highlightColor, Color labelColor, Color inputTextColor)
{
	return menuElementCreateTextfieldV(elementName, (Vector2){positionX, positionY}, (Vector2){sizeX, sizeY}, label, labelFontSize, value, maxLength, backgroundColor, highlightColor, labelColor, inputTextColor);
}

menuElement menuElementCreateLabelV(char* elementName, Vector2 position, Vector2 size, char* label, float fontSize, Color color)
{
	menuElement element = menuElementCreateBase(elementName, position, size, MetLabel);
	
	element.position = position;
	element.size = size;
	element.menuElementType = MetLabel;
	
	strncpy(element.labelText, label, MET_MAX_LABEL_LENGTH - 1);
	element.labelTextFontSize = fontSize;
	element.labelTextColor = color;
	element.labelTextFont = newElementFont;
	
	return element;
}

menuElement menuElementCreateImageV(char* elementName, Vector2 position, Vector2 size, Texture2D image, Rectangle sourceRectangle, Color tint)
{
	menuElement element = menuElementCreateBase(elementName, position, size, MetImage);
	
	element.position = position;
	element.size = size;
	element.menuElementType = MetImage;
	
	element.imageTexture = image;
	element.imageSourceRectangle = sourceRectangle;
	element.imageTint = tint;
	
	return element;
}

menuElement menuElementCreateAnimationV(char* elementName, Vector2 position, Vector2 size, spriteAnimation animation, Color tint)
{
	menuElement element = menuElementCreateBase(elementName, position, size, MetAnimation);
	
	element.position = position;
	element.size = size;
	element.menuElementType = MetAnimation;
	
	element.animationData = animation;
	element.imageTint = tint;
	
	return element;
}

menuElement menuElementCreateButtonV(char* elementName, Vector2 position, Vector2 size, Color buttonTint, buttonAction onPressed, char* buttonText, Color buttonTextColor, float buttonTextFontSize)
{
	menuElement element = menuElementCreateBase(elementName, position, size, MetButton);
	
	element.position = position;
	element.size = size;
	element.menuElementType = MetButton;
	element.interactable = true;
	
	element.buttonTint = buttonTint;
	element.buttonOnPressed = onPressed;
	strncpy(element.buttonText, buttonText, MET_MAX_LABEL_LENGTH - 1);
	element.buttonTextColor = buttonTextColor;
	element.buttonTextFontSize = buttonTextFontSize;
	element.buttonTextFont = newElementFont;
	
	return element;
}

menuElement menuElementCreateSliderV(char* elementName, Vector2 position, Vector2 size, Color sliderBackgroundColor, Color sliderFillColor, Color sliderExtraColor, float initialValue, sliderDisplayEnum displayType, sliderAction onSliderValueChanged)
{
	menuElement element = menuElementCreateBase(elementName, position, size, MetSlider);
	
	element.position = position;
	element.size = size;
	element.menuElementType = MetSlider;
	element.interactable = true;
	
	element.sliderBackgroundColor = sliderBackgroundColor;
	element.sliderFillColor = sliderFillColor;
	element.sliderOutlineColor = sliderExtraColor;
	element.sliderValue = initialValue;
	element.sliderDisplayType = displayType;
	element.sliderOnChanged = onSliderValueChanged;
	element.sliderTextFont = newElementFont;
	
	return element;
}

menuElement menuElementCreateTextfieldV(char* elementName, Vector2 position, Vector2 size, char* label, float labelFontSize, char* value, int maxLength, Color backgroundColor, Color highlightColor, Color labelColor, Color inputTextColor)
{
	menuElement element = menuElementCreateBase(elementName, position, size, MetTextfield);
	
	element.position = position;
	element.size = size;
	element.menuElementType = MetTextfield;
	element.interactable = true;
	
	strncpy(element.textfieldTextLabel, label, MET_MAX_LABEL_LENGTH - 1);
	element.textfieldTextLabelFontSize = labelFontSize;
	element.textfieldTextInput = value;
	element.textfieldInputMaxLength = maxLength;
	element.textfieldBackgroundColor = backgroundColor;
	element.textfieldHighlightColor = highlightColor;
	element.textfieldTextLabelColor = labelColor;
	element.textfieldTextInputColor = inputTextColor;
	element.textfieldAllowedCharacters = newElementAllowedCharacters;
	
	element.textfieldTextLabelFont = newElementFont;
	element.textfieldTextInputFont = newElementFont;
	
	return element;
}