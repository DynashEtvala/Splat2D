#pragma once
#include "raylib.h"

float VectorLength(Vector2 v);
Vector2 NormalizeVector(Vector2*);
Vector2 NormalizeVector(Vector2);
Vector2 operator+(Vector2 a, Vector2 b); 
Vector2 operator-(Vector2 a, Vector2 b);
Vector2 operator*(Vector2 a, float b);
Color operator+(Color a, Color b);
Color operator-(Color a, Color b);
bool operator==(Color a, Color b);
bool operator!=(Color a, Color b);