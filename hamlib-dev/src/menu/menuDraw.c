#include "menu.h"
#include "menu/shared.h"
#include "menu/drawUtils.h"

#include <stdlib.h>
#include <string.h>

static void drawMenuElement(menuElement* elementPtr, menu mnu);
static void getSliderButtonInfo(Rectangle elementRect, float sliderValue, Rectangle* backgroundRectPtr, Rectangle* fillRectPtr, Rectangle* buttonRectPtr);
elementDrawData getDrawData(menuElement* elementPtr, menu mnu);

static void drawLabel(menuElement element, elementDrawData data);
static void drawImage(menuElement element, elementDrawData data);
static void drawAnimation(menuElement* elementPtr, elementDrawData data);
static void drawButton(menuElement element, elementDrawData data);
static void drawSlider(menuElement element, elementDrawData data);
static void drawTextfield(menuElement element, elementDrawData data);

void menuDraw(menu mnu)
{
	if (mnu.state == MNU_CLOSED)
	{
		return;
	}
	
	DrawRectangle(mnu.position.x, mnu.position.y, mnu.size.x, mnu.size.y, (Color){0xf9,0xf9,0xf9,0xff});
	
	for (int i = 0; i < MENU_LENGTH; i++)
	{
		if (mnu.elementIsValid[i])
			drawMenuElement(mnu.list + i, mnu);
	}
}

static void drawMenuElement(menuElement* elementPtr, menu mnu)
{
	if (elementPtr->hidden)
	{
		return;
	}
	
	menuElement element = *elementPtr;
	
	elementDrawData data = getDrawData(elementPtr, mnu);
	
	if (element.drawFunctionOverride != NULL)
	{
		element.drawFunctionOverride(elementPtr, data);
	}
	
	// Default Drawing
	switch(element.menuElementType)
	{
		case MetLabel:
			drawLabel(element, data);
			break;
		case MetImage:
			drawImage(element, data);
			break;
		case MetAnimation:
			drawAnimation(elementPtr, data);
			break;
		case MetButton:
			drawButton(element, data);
			break;
		case MetSlider:
			drawSlider(element, data);
			break;
		case MetTextfield:
			drawTextfield(element, data);
			break;
		default:
			// do nothing, invalid menuElementType
			DrawText("ERROR, INVALID MENUELEMENTTYPE", data.position.x, data.position.y, data.size.y / 3, RED);
			break;
	}
}

static void drawLabel(menuElement element, elementDrawData data)
{
	int fontSize = data.size.y * element.labelTextFontSize;
	
	if (data.shadow)
	{
		drawTextAlignedPro(element.labelTextFont, element.labelText, data.shadowPosition, Vector2Zero(), 0.0f, fontSize, 1.0f, data.shadowColor, TaLeft | TaBottom);
	}
	
	drawTextAlignedPro(element.labelTextFont, element.labelText, data.position, Vector2Zero(), 0.0f, fontSize, 1.0f, element.labelTextColor, TaLeft | TaBottom);
}

static void drawImage(menuElement element, elementDrawData data)
{
	if (data.shadow)
	{
		DrawTexturePro(element.imageTexture, element.imageSourceRectangle, data.shadowRect, data.origin, 0.0f, data.shadowColor);
	}
	
	DrawTexturePro(element.imageTexture, element.imageSourceRectangle, data.rect, data.origin, 0.0f, element.imageTint);
}

static void drawAnimation(menuElement* elementPtr, elementDrawData data)
{
	if (data.shadow)
	{
		// don't draw the animation, instead draw the current sprite as a sprite. drawing the animation shadow as animation will
		// effectively double the animation speed
		drawSpritePro(getCurrentSprite(elementPtr->animationData), data.shadowRect, data.origin, 0.0f, data.shadowColor);
	}
	
	drawSpriteAnimationPro(frametime, &(elementPtr->animationData), data.rect, data.origin, 0.0f, elementPtr->animationTint);
}

static void drawButton(menuElement element, elementDrawData data)
{
	Color buttonTint = element.buttonTint;
	
	if (data.shadow)
	{
		DrawTextureNPatch(texButton, nfoButton, data.shadowRect, data.origin, 0.0f, data.shadowColor);
	}
	
	// we test menuOpen separately for the simple reason that elements which are on an opening or closing menu should still be drawn but should not be effected by dip. Consider moving menuOpen into the dip assignment.
	if (data.dip && data.menuOpen)
	{
		data.rect = data.shadowRect;
		buttonTint = getPressedColor(buttonTint);
	}
	else if (element.highlighted && data.menuOpen)
	{
		buttonTint = getHighlightColor(buttonTint);
	}
	
	DrawTextureNPatch(texButton, nfoButton, data.rect, data.origin, 0.0f, buttonTint);
	
	Vector2 textPosition = Vector2Add(data.position, Vector2Scale(data.size, 0.5f));
	drawTextAlignedPro(element.buttonTextFont, element.buttonText, textPosition, Vector2Zero(), 0.0f, element.buttonTextFontSize * data.size.y, 1.0f, element.buttonTextColor, TaCenter | TaMiddle);
}

static void drawSlider(menuElement element, elementDrawData data)
{
	// Squash the interactive element into the lower two thirds
	fitDataForSlider(&data);
	
	Rectangle fillRect = data.rect;
	if (element.sliderDisplayType == SdtBar)
	{
		fillRect.width *= CLAMP_01(element.sliderValue);
		DrawRectangleRec(data.rect, element.sliderBackgroundColor);
		DrawRectangleRec(fillRect, element.sliderFillColor);
		DrawRectangleLinesEx(data.rect, element.sliderOutlineWeight * data.size.y, element.sliderOutlineColor);
	}
	else if (element.sliderDisplayType == SdtCircle || element.sliderDisplayType == SdtButton)
	{
		Rectangle backgroundRect;
		Rectangle buttonRect;
		
		float elementRectPadding = (0.1f * data.rect.height)*4;
		data.rect.x += elementRectPadding;
		data.rect.width -= elementRectPadding * 2.0f;
		
		getSliderButtonInfo(data.rect, element.sliderValue, &backgroundRect, &fillRect, &buttonRect);
		
		data.dip = CheckCollisionPointRec(mousePosition, buttonRect) && mouseIsDown && data.menuOpen;
		
		// Drawing step
		DrawRectangleRec(backgroundRect, element.sliderBackgroundColor);
		DrawRectangleRec(fillRect, element.sliderFillColor);
		if (element.sliderDisplayType == SdtButton)
		{
			DrawTextureNPatch(texSliderButton, nfoSliderButton, buttonRect, data.origin, 0.0f, data.shadowColor);
			if (!data.dip)
				buttonRect.y -= 4;
			DrawTextureNPatch(texSliderButton, nfoSliderButton, buttonRect, data.origin, 0.0f, element.sliderOutlineColor);
		}
		else if (element.sliderDisplayType == SdtCircle)
		{
			DrawCircle(buttonRect.x + buttonRect.width / 2, buttonRect.y + buttonRect.height / 2, buttonRect.width / 2, data.shadowColor);
			if (!data.dip)
				buttonRect.y -= 4;
			DrawCircle(buttonRect.x + buttonRect.width / 2, buttonRect.y + buttonRect.height / 2, buttonRect.width / 2, element.sliderOutlineColor);
		}
	}
	
	// Draw label
	int fontSize = data.size.y * element.sliderTextFontSize;
	Vector2 textPosition = {data.position.x + 0.05f * data.size.x, data.position.y - 0.05f * data.size.y};
	
	if (data.shadow)
	{
		Vector2 textShadowPosition = {textPosition.x, textPosition.y + element.shadowHeight};
		drawTextAlignedPro(element.sliderTextFont, element.sliderText, textShadowPosition, data.origin, 0.0f, fontSize, 1.0f, data.shadowColor, TaLeft | TaBottom);
	}
	
	drawTextAlignedPro(element.sliderTextFont, element.sliderText, textPosition, data.origin, 0.0f, fontSize, 1.0f, element.sliderTextColor, TaLeft | TaBottom);
}

static void drawTextfield(menuElement element, elementDrawData data)
{
	fitDataForTextfield(&data);
	
	// Draw input field
	DrawRectangleRec(data.rect, element.textfieldBackgroundColor);
	if (element.highlighted)
		DrawRectangleLinesEx(data.rect, element.textfieldOutlineWeight * data.size.y, element.textfieldHighlightColor);
	
	// Draw inner text
	char textfieldInnerText[257];
	if (element.highlighted && (int)GetTime() % 2)
	{
		strcpy(textfieldInnerText, TextFormat("%s_", element.textfieldTextInput));
	}
	else
	{
		strcpy(textfieldInnerText, element.textfieldTextInput);
	}
	Vector2 textPosition = {data.position.x, data.position.y + data.size.y / 2};
	drawTextAlignedPro(element.textfieldTextInputFont, textfieldInnerText, textPosition, data.origin, 0.0f, data.size.y * 0.8f, 1.0f, element.textfieldTextInputColor, TaLeft | TaMiddle);
	
	// Draw label
	int fontSize = data.size.y * element.textfieldTextLabelFontSize;
	textPosition = (Vector2){data.position.x + 0.05f * data.size.x, data.position.y - 0.05f * data.size.y};
	
	if (data.shadow)
	{
		Vector2 textShadowPosition = {textPosition.x, textPosition.y + element.shadowHeight};
		drawTextAlignedPro(element.textfieldTextLabelFont, element.textfieldTextLabel, textShadowPosition, data.origin, 0.0f, fontSize, 1.0f, data.shadowColor, TaLeft | TaBottom);
	}
	
	drawTextAlignedPro(element.textfieldTextLabelFont, element.textfieldTextLabel, textPosition, data.origin, 0.0f, fontSize, 1.0f, element.textfieldTextLabelColor, TaLeft | TaBottom);
}

static void getSliderButtonInfo(Rectangle elementRect, float sliderValue, Rectangle* backgroundRectPtr, Rectangle* fillRectPtr, Rectangle* buttonRectPtr)
{
	float fillPaddingY = (elementRect.height * 0.1f);
	float fillPaddingX = (elementRect.width * 0.495f);
	float backgroundPaddingY = (elementRect.height * 0.15f);
	
	if (backgroundRectPtr != NULL)
	{
		*backgroundRectPtr = elementRect;
		backgroundRectPtr->y += (elementRect.height * 0.5f) - backgroundPaddingY;
		backgroundRectPtr->height = backgroundPaddingY * 2.0f;
	}
	
	if (fillRectPtr != NULL)
	{
		*fillRectPtr = elementRect;
		fillRectPtr->y += (elementRect.height * 0.5f) - fillPaddingY;
		fillRectPtr->height = fillPaddingY * 2.0f;
		
		fillRectPtr->x += (elementRect.width * 0.5f) - fillPaddingX;
		fillRectPtr->width = fillPaddingX * 2.0f;
		
		fillRectPtr->width *= sliderValue;
	}
	
	if (buttonRectPtr != NULL)
	{
		Rectangle buttonRect = elementRect;
		buttonRect.y += 0.1f * buttonRect.height;
		buttonRect.height *= 0.8f;
		buttonRect.width = buttonRect.height;
		buttonRect.x = elementRect.x - buttonRect.width/2 + (sliderValue * (elementRect.width));
		*buttonRectPtr = buttonRect;
	}
} // getSliderButtonInfo

elementDrawData getDrawData(menuElement* elementPtr, menu mnu)
{
	menuElement element = *elementPtr;
	
	// calculate absolute position and size of element
	Vector2 elementPosition = {0};
	Vector2 elementSize = {0};
	getMenuElementPositionAndSize(element, mnu, &elementPosition, &elementSize);
	Rectangle elementRect = {
		elementPosition.x,
		elementPosition.y,
		elementSize.x,
		elementSize.y
	};
	
	// Shadow
	Vector2 elementShadowPosition = elementPosition;
	elementShadowPosition.y += element.shadowHeight;
	Rectangle elementShadowRect = elementRect;
	elementShadowRect.y = elementShadowPosition.y;
	Color colorShadow = Fade(BLACK, 0.7f);
	bool shadow = (element.shadowHeight != 0);
	bool buttonDip = CheckCollisionPointRec(GetMousePosition(), elementRect) && mouseIsDown;
	
	// Other common parameters
	Vector2 origin = {0,0};
	
	elementDrawData data = {
		.position = elementPosition,
		.size = elementSize,
		.rect = elementRect,
		.origin = origin,
		.shadowPosition = elementShadowPosition,
		.shadowRect = elementShadowRect,
		.shadowColor = colorShadow,
		.shadow = shadow,
		.dip = buttonDip,
		.menuOpen = mnu.state == MNU_OPEN
	};
	
	return data;
}