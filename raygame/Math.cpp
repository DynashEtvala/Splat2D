#include "Math.h"
#include <math.h>

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