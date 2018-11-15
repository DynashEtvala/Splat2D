#pragma once
#include "GameObject.h"
#include "Math.h"
class SpawnPad :
	public GameObject
{
public:
	Color teamColor = BLACK;
	Vector2 spawnSpaces[4];
	SpawnPad();
	SpawnPad(int x, int y, Color col);
	~SpawnPad();
	void Draw();
};

