#include "GameObject.h"

#include "Controller.h"

GameObject::GameObject()
{
	speed = 5;
	direction.x = 0;
	direction.y = 0;
	collisionRect.x = 0;
	collisionRect.y = 0;
	collisionRect.height = 0;
	collisionRect.width = 0;
}


GameObject::~GameObject()
{}

Vector2 GameObject::getCenter()
{
	Vector2 result;

	result.x = collisionRect.x + getRadius();
	result.y = collisionRect.y + getRadius();

	return result;
}

float GameObject::getRadius()
{
	return collisionRect.height < collisionRect.width ? collisionRect.height / 2 : collisionRect.width / 2;
}