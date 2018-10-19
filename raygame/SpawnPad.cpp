#include "SpawnPad.h"



SpawnPad::SpawnPad()
{
	teamColor = WHITE;
	collisionRect = Rectangle{ 40, 40, 100, 100 };
	spawnSpaces[0] = getCenter() + Vector2{ -39, -39 };
	spawnSpaces[1] = getCenter() + Vector2{ 5, -39 };
	spawnSpaces[2] = getCenter() + Vector2{ -39, 5 };
	spawnSpaces[3] = getCenter() + Vector2{ 5, 5 };
}

SpawnPad::SpawnPad(int x, int y, Color col)
{
	teamColor = col;
	collisionRect = Rectangle{ (float)(x), (float)(y), 100, 100 };
	spawnSpaces[0] = getCenter() + Vector2{ -39, -39 };
	spawnSpaces[1] = getCenter() + Vector2{ 5, -39 };
	spawnSpaces[2] = getCenter() + Vector2{ -39, 5 };
	spawnSpaces[3] = getCenter() + Vector2{ 5, 5 };
}

SpawnPad::~SpawnPad()
{}

void SpawnPad::Draw()
{
	DrawCircle(getCenter().x, getCenter().y, getRadius(), BLACK);
	DrawCircle(getCenter().x, getCenter().y, getRadius() - 1, teamColor);
}