#include "Math.h"
#include <math.h>

float VectorLength(Vector2 v)
{
	float result;
	if (v.x != 0 && v.y != 0)
	{
		result = sqrt((v.x * v.x) + (v.y * v.y));
	}
	else if (v.x != 0)
	{
		result = fabsf(v.x);
	}
	else if (v.y != 0)
	{
		result = fabsf(v.y);
	}
	return result;
}

Vector2 NormalizeVector(Vector2* v)
{
	if (v->x != 0 && v->y != 0)
	{
		float l = sqrt((v->x * v->x) + (v->y * v->y));
		v->x = v->x / l;
		v->y = v->y / l;
	}
	else if (v->x != 0)
	{
		if (v->x > 0)
		{
			v->x = 1;
		}
		else
		{
			v->x = -1;
		}
	}
	else if (v->y != 0)
	{
		if (v->y > 0)
		{
			v->y = 1;
		}
		else
		{
			v->y = -1;
		}
	}
	return *v;
}

Vector2 NormalizeVector(Vector2 v)
{
	if (v.x != 0 && v.y != 0)
	{
		float l = sqrt((v.x * v.x) + (v.y * v.y));
		v.x = v.x / l;
		v.y = v.y / l;
	}
	else if (v.x != 0)
	{
		if (v.x > 0)
		{
			v.x = 1;
		}
		else
		{
			v.x = -1;
		}
	}
	else if (v.y != 0)
	{
		if (v.y > 0)
		{
			v.y = 1;
		}
		else
		{
			v.y = -1;
		}
	}
	return v;
}

Vector2 operator+(Vector2 a, Vector2 b)
{
	Vector2 result = { a.x + b.x, a.y + b.y };
	return result;
}

Vector2 operator-(Vector2 a, Vector2 b)
{
	Vector2 result = { a.x - b.x, a.y - b.y };
	return result;
}

Vector2 operator*(Vector2 a, float b)
{
	Vector2 result = { a.x * b, a.y * b };
	return result;
}

float operator*(Vector2 a, Vector2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

Color operator+(Color a, Color b)
{
	return Color{ (unsigned char)(a.r + b.r), (unsigned char)(a.g + b.g), (unsigned char)(a.b + b.b), (unsigned char)(a.a + b.a) };
}

Color operator-(Color a, Color b)
{
	return Color{ (unsigned char)(a.r - b.r), (unsigned char)(a.g - b.g), (unsigned char)(a.b - b.b), (unsigned char)(a.a - b.a) };
}

bool operator==(Color a, Color b)
{
	return (a.a == b.a && a.b == b.b && a.g == b.g && a.r == b.r);
}

bool operator!=(Color a, Color b)
{
	return !(a == b);
}