#pragma once
#include "GameObject.h"
class Shot :
	public GameObject
{
private:
	float damage;
	float range;
	float timer = 0;
public:
	Color color;
	Shot();
	Shot(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, Color col);
	~Shot();
	void Update();
};

