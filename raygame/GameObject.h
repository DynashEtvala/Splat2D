#pragma once
#include "raylib.h"
class GameObject
{
protected:
	Vector2 direction;
	float speed;
public:
	Rectangle collisionRect;

	GameObject();
	~GameObject();
	virtual void Update(class Controller controller);
	Vector2 getCenter();
	float getRadius();
};

