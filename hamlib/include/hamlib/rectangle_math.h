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

#ifndef HAMLIB_RECTANGLE_MATH_H
#define HAMLIB_RECTANGLE_MATH_H

#include "raymath.h"

// add a vector to a rectangle rectangle (v1 + v2)
Rectangle rectangleAdd(Rectangle v1, Vector2 v2);
// add rectangle and float value
Rectangle rectangleAddValue(Rectangle v, float add);
// Subtract a vector from rectangle's position (v1 - v2)
Rectangle rectangleSubtract(Rectangle v1, Vector2 v2);
// Calculate vector length part of rectangle
float rectangleLength(Rectangle v);
float rectangleDiagonalLength(Rectangle v);
// Calculate rectangle square length (vector part)
float rectangleLengthSqr(Rectangle v);
// Calculate distance between the center of two rectangles
float rectangleDistance(Rectangle v1, Rectangle v2);
// Scale vector (multiply by value)
Rectangle rectangleScale(Rectangle v, float scale);
// Calculate linear interpolation between two vectors
Rectangle rectangleLerp(Rectangle v1, Rectangle v2, float amount);
// Rotate rectangle by angle
Rectangle rectangleRotate(Rectangle v, float angle);
// Move Rectangle towards target
Rectangle rectangleMoveTowards(Rectangle v, Vector2 target, float maxDistance);

#endif