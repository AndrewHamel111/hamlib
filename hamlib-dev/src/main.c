#include <stdlib.h>
#include <time.h>

#define V(x,y) (Vector2){(x),(y)}

#include "menu.h"

// Variables
Texture2D texButton;
NPatchInfo nfoButton;
Texture2D texSliderButton;
NPatchInfo nfoSliderButton;
bool gameQuit = false;

Vector2 mousePosition = {0};
bool mouseIsPressed = false;
bool mouseIsDown = false;
float frametime = 0;

menu mnuMain = {0};

float polygonRadius = 20; // slider demo
float polygonRotation = 0;
float polygonSideCount = 3;
float polygonMaxRadius = 200;
float maxSideCount = 10;

char testTextfieldTextValue[256]; // textfield demo

// Constants
const int screenWidth = 1280;
int screenHeight = 720;
char* menuInputAllowedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_+=<>[](){}";

// Functions
void init(void);
void update(void);
void draw(void);
void uninit(void);

void loadResources(void);
void initMenus(void);

// Button functions
void quitButtonPressed(void);
void radiusChanged(float);
void rotationChanged(float);
void sideCountChanged(float);

// Math helpers
float map(int i, int minIn, int maxIn, int minOut, int maxOut);
float mapf(float f, float minIn, float maxIn, float minOut, float maxOut);

int main(void)
{
	init();
	
	InitWindow(screenWidth, screenHeight, "hamlib-testing");
	SetTargetFPS(144);
	SetExitKey(KEY_ESCAPE);

	loadResources();

	while(!WindowShouldClose() && !gameQuit)
	{
		update();

		BeginDrawing();

			draw();

		EndDrawing();
	}

	CloseWindow();

	uninit();
}

void init(void)
{
	srand(time(0));
}

void update(void)
{
	frametime = GetFrameTime();
	
	mousePosition = GetMousePosition();
	mouseIsPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	mouseIsDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	
	if (IsKeyPressed(KEY_P))
	{
		int x = 1;
	}
	
	menuUpdate(&mnuMain);
}

void draw(void)
{
	ClearBackground(WHITE);
	
	DrawPoly((Vector2){screenWidth*0.75f, screenHeight*0.5f}, polygonSideCount, polygonRadius, polygonRotation, RED);
	
	menuDraw(mnuMain);
}

void uninit(void)
{
	// Unload Resources

	// Free memory
}

void loadResources(void)
{
	texButton = LoadTexture("resources/spr/button.png");
	nfoButton = (NPatchInfo){
		(Rectangle){0,0,32,32},
		15, 15, 15, 15,
		NPATCH_NINE_PATCH
	};
	
	texSliderButton = LoadTexture("resources/spr/button-tiny.png");
	nfoSliderButton = (NPatchInfo){
		(Rectangle){0,0,16,16},
		7, 7, 7, 7,
		NPATCH_NINE_PATCH
	};
	
	initMenus();
}

void initMenus(void)
{
	// main menu init
	setTextFieldAllowedCharacters(menuInputAllowedCharacters);
	setMenuElementFont(GetFontDefault());
	
	mnuMain = menuCreate(V(0,0), V(screenWidth / 2, screenHeight));
	menuAdd(&mnuMain, menuElementCreateLabel(
		0.1f, 0.1f, 0.8f, 0.08f,
		"HELLO WORLD", 1.0f, GetColor(0xffa549FF)
	));
	menuAdd(&mnuMain, menuElementCreateTextfield(
		0.1, 0.2, 0.8, 0.2, "TEST TEXT FIELD", 0.2, testTextfieldTextValue, 12,
		GetColor(0xd0d2beff), GetColor(0x708e7eff), GetColor(0xe0e7d5ff), BLACK
	));
	menuAdd(&mnuMain, menuElementCreateSlider(
		0.1, 0.4, 0.8, 0.08,
		GetColor(0xd0d2beff), GetColor(0x708e7eff), GetColor(0xe0e7d5ff),
		0.0f, SdtButton, radiusChanged
	));
	menuAdd(&mnuMain, menuElementCreateSlider(
		0.1, 0.5, 0.8, 0.08,
		GetColor(0xd0d2beff), GetColor(0x708e7eff), GetColor(0xe0e7d5ff),
		0.0f, SdtCircle, rotationChanged
	));
	menuAdd(&mnuMain, menuElementCreateSlider(
		0.1, 0.6, 0.8, 0.08,
		GetColor(0xd0d2beff), GetColor(0x708e7eff), GetColor(0xe0e7d5ff),
		0.0f, SdtBar, sideCountChanged
	));
	menuAdd(&mnuMain, menuElementCreateButton(
		0.1f, 0.8, 0.8, 0.1,
		GRAY, quitButtonPressed, "Quit", GetColor(0xd43e43FF), 0.5
	));
	
}

void quitButtonPressed(void)
{
	gameQuit = true;
}

void radiusChanged(float f)
{
	polygonRadius = f * polygonMaxRadius;
}

void rotationChanged(float f)
{
	polygonRotation = f * 360;
}

void sideCountChanged(float f)
{
	int sides = round(f * 7.0f);
	polygonSideCount = map(sides, 0, 7, 3, 10);
}

float map(int i, int minIn, int maxIn, int minOut, int maxOut)
{
	return ceil(mapf(i, minIn, maxIn, minOut, maxOut));
}

float mapf(float f, float minIn, float maxIn, float minOut, float maxOut)
{
	return (((f - minIn) / (maxIn - minIn)) * (maxOut - minOut)) + minOut;
}