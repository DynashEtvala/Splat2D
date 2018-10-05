#include "Shot.h"



Shot::Shot() : GameObject()
{
	deleteMe = true;
}

Shot::Shot(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, Color col)
{
	collisionRect = loc;
	direction = dir;
	speed = spd;
	damage = dmg;
	range = rng;
	color = col;
}

Shot::~Shot()
{}

void Shot::Update()
{
	collisionRect.x += direction.x * speed * GetFrameTime();
	collisionRect.y += direction.y * speed * GetFrameTime();

	if (timer >= range)
	{
		deleteMe = true;
	}

}