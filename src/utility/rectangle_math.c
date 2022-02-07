#include "hamlib.h"

// Add a vector to a rectangle rectangle (v1 + v2)
Rectangle RectangleAdd(Rectangle v1, Vector2 v2)
{
    Rectangle result = { v1.x + v2.x, v1.y + v2.y, v1.width, v1.height };

    return result;
}

// Add rectangle and float value
Rectangle RectangleAddValue(Rectangle v, float add)
{
    Rectangle result = { v.x + add, v.y + add, v.width, v.height };

    return result;
}

// Subtract a vector from rectangle's position (v1 - v2)
Rectangle RectangleSubtract(Rectangle v1, Vector2 v2)
{
    Rectangle result = { v1.x - v2.x, v1.y - v2.y, v1.width, v1.height };

    return result;
}

// Calculate vector length part of rectangle
float RectangleLength(Rectangle v)
{
    float result = sqrtf((v.x*v.x) + (v.y*v.y));

    return result;
}

float RectangleDiagonalLength(Rectangle v)
{
    float result = sqrtf((v.width*v.width) + (v.height*v.height));

    return result;
}

// Calculate rectangle square length (vector part)
float RectangleLengthSqr(Rectangle v)
{
    float result = (v.x*v.x) + (v.y*v.y);

    return result;
}

// Calculate distance between the center of two rectangles
float RectangleDistance(Rectangle v1, Rectangle v2)
{
    float result = sqrtf((v1.x + v1.width/2 - v2.x + v2.width/2)*(v1.x + v1.width/2 - v2.x + v2.width/2) + (v1.y + v1.height/2 - v2.y + v2.height/2)*(v1.y + v1.height/2 - v2.y + v2.height/2));

    return result;
}

// Scale vector (multiply by value)
Rectangle RectangleScale(Rectangle v, float scale)
{
    Rectangle result = { v.x*scale, v.y*scale, v.width*scale, v.height*scale };

    return result;
}

// Calculate linear interpolation between two vectors
Rectangle RectangleLerp(Rectangle v1, Rectangle v2, float amount)
{
    Rectangle result = { 0 };

    result.x = v1.x + amount*(v2.x - v1.x);
    result.y = v1.y + amount*(v2.y - v1.y);
    result.width = v1.width + amount*(v2.width - v1.width);
    result.height = v1.height + amount*(v2.height - v1.height);

    return result;
}

// Rotate rectangle by angle
Rectangle RectangleRotate(Rectangle v, float angle)
{
    Rectangle result = { 0 };

    result.x = v.x*cosf(angle) - v.y*sinf(angle);
    result.y = v.x*sinf(angle) + v.y*cosf(angle);

    return result;
}

// Move Rectangle towards target
Rectangle RectangleMoveTowards(Rectangle v, Vector2 target, float maxDistance)
{
    Rectangle result = { 0 };
	Rectangle _target = { target.x, target.y, v.width, v.height };

    float dx = _target.x - v.x;
    float dy = _target.y - v.y;
    float value = (dx*dx) + (dy*dy);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance*maxDistance))) return _target;

    float dist = sqrtf(value);

    result.x = v.x + dx/dist*maxDistance;
    result.y = v.y + dy/dist*maxDistance;

    return result;
}