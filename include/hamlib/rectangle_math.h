/**
 * \file rectangle_math.h
 * \author Andrew Hamel (me@andrewhamel.ca)
 * \brief A sort of extension on raymath.h, which focuses on implementing Rectangle operations for each of the Vector2 operations.
 * \version 1.0
 * \date 2022-02-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef HAMLIB_RECTANGLE_MATH_H_
#define HAMLIB_RECTANGLE_MATH_H_

#include "raymath.h"

// Add a vector to a rectangle rectangle (v1 + v2)
Rectangle RectangleAdd(Rectangle v1, Vector2 v2);
// Add rectangle and float value
Rectangle RectangleAddValue(Rectangle v, float add);
// Subtract a vector from rectangle's position (v1 - v2)
Rectangle RectangleSubtract(Rectangle v1, Vector2 v2);
// Calculate vector length part of rectangle
float RectangleLength(Rectangle v);
float RectangleDiagonalLength(Rectangle v);
// Calculate rectangle square length (vector part)
float RectangleLengthSqr(Rectangle v);
// Calculate distance between the center of two rectangles
float RectangleDistance(Rectangle v1, Rectangle v2);
// Scale vector (multiply by value)
Rectangle RectangleScale(Rectangle v, float scale);
// Calculate linear interpolation between two vectors
Rectangle RectangleLerp(Rectangle v1, Rectangle v2, float amount);
// Rotate rectangle by angle
Rectangle RectangleRotate(Rectangle v, float angle);
// Move Rectangle towards target
Rectangle RectangleMoveTowards(Rectangle v, Vector2 target, float maxDistance);

#endif