#pragma once
#include "raylib.h"
#include "GlobalVars.h"
class GameObject
{
protected:
	Vector2 direction;
	float speed;
public:
	Rectangle collisionRect;

	GameObject();
	~GameObject();
	virtual void Draw()=0;
	Vector2 getCenter();
	float getRadius(); 
	bool pointOnScreen(int x, int y)
	{
		if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
		{
			return true;
		}
		return false;
	};

};

