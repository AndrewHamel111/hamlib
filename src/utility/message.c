#include "hamlib.h"
/// MESSAGE.C ///////////////////////////////////////////////////////////////////////////
#include "hamlib/lists/queue.h"
#include "hamlib/draw.h"
#include "easings.h"
#include <string.h>

typedef struct message
{
	// body of the message
	char text[255];
	// start time of message being displayed
	float showTime;
	// how long the message should be displayed
	float showDuration;
	// color of the text
	Color textColor;
	// color of the box behind the text
	Color color;
} message;

queue messageQueue = {0};
static Vector2 messageStartPos = {1280 / 2, 720 + 50};
static Vector2 messageEndPos = {1280 / 2, 720 - 50};

/// \brief To change the duration of the message show period, check hamlib.h to find the related #define directives.
void showMessage(const char* text, float time, Color color)
{
	// TODO consider changing this function to take the box color, not text color, then use tintcolor(color,1.0f) to get a visible color
	Color bgColor;
	short r = color.r, g = color.g, b = color.b;
	if (r + g + b < (unsigned short)(255*1.5f))
		bgColor = (Color){240, 240, 240, 255};
	else
		bgColor = (Color){20, 20, 20, 255};
	
	showMessagePro(text, time, color, bgColor);
}

void showMessagePro(const char* text, float time, Color textColor, Color color)
{
	message msg = {0};
	strcpy(msg.text, text);
	msg.showDuration = time;
	msg.color = color;
	msg.textColor = textColor;
	
	enqueue(&messageQueue, &text, sizeof(text));
}

void showError(const char* msg, float time)
{
	showMessagePro(msg, time, RED, (Color) {200, 200, 200, 100});
}

void clearMessages(void)
{
	clearQueue(&messageQueue);
}

void drawMessage(float frametime)
{
	if (messageQueue.length < 1)
	{
		return;
	} // first escape clause

	// update queue state
	message* msg = (message*)(messageQueue.mList->value);
	if (msg->showTime > msg->showDuration)
		dequeue(&messageQueue, NULL);
	else
		msg->showTime += frametime;

	if (messageQueue.length < 1)
	{
		return;
	} // second escape clause since we may have dequeued the last message

	msg = (message*)(messageQueue.mList->value);

	Vector2 v;
	float time = msg->showTime;
	float tVec = EaseBackOut(CLAMP(time, 0, MESSAGE_EASE_TIME), 0, 1.0f, MESSAGE_EASE_TIME);
	v = (Vector2){
		LERP(messageStartPos.x, messageEndPos.x, tVec),
		LERP(messageStartPos.y, messageEndPos.y, tVec)
	};

	Color textColor = msg->textColor;
	Color color = msg->color;
	float tColor = CLAMP(time, msg->showDuration - MESSAGE_FADE_TIME, msg->showDuration) - (msg->showDuration - MESSAGE_FADE_TIME);
	textColor.a = EaseLinearOut(tColor, 255, -255, 1.0f);
	color.a = EaseLinearOut(tColor, 255, -255, 1.0f);

	// calculate size of rectangle and draw it
	Vector2 size = MeasureTextEx(GetFontDefault(), msg->text, MESSAGE_FONT_SIZE, 1.0f);
	size = Vector2Scale(size, 1.2f);
	Rectangle rec = {v.x, v.y, size.x, size.y};
	Vector2 origin = {rec.width/2, 0};
	DrawRectanglePro(rec, origin, 0.0f, color);

	// draw text
	drawTextAligned(msg->text, v.x, v.y, MESSAGE_FONT_SIZE, textColor, TaCenter | TaMiddle);
}

void setMessagePositions(float startX, float startY, float endX, float endY)
{
	setMessagePositionsVec((Vector2) {startX, startY}, (Vector2) {endX, endY});
}

void setMessagePositionsVec(Vector2 start, Vector2 end)
{
	messageStartPos = start;
	messageEndPos = end;
}
/// MESSAGE.C ///////////////////////////////////////////////////////////////////////////