#include "Shot.h"
#include "Math.h"
#include "Controller.h"
#include "FloorTile.h"

Shot::Shot() : GameObject()
{
	active = false;
}

Shot::~Shot()
{}

void Shot::Update(Controller* controller, FloorTile*** ftile)
{
	if (timer < range)
	{
		collisionRect.x += direction.x * speed * GetFrameTime();
		collisionRect.y += direction.y * speed * GetFrameTime();
		timer += VectorLength(direction) * speed * GetFrameTime();
	}
	else
	{
		collisionRect.x += direction.x * (range - timer);
		collisionRect.y += direction.y * (range - timer);
		Burst(controller, ftile);
	}
	if (!GetRandomValue(0, 1))
	{
		Drip(controller, ftile);
	}
}

void Shot::SetNewVals(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, int bsize, int dsize, Color col)
{
	collisionRect = loc;
	direction = dir;
	speed = spd;
	damage = dmg;
	range = rng;
	timer = 0;
	burstSize = bsize;
	dripSize = dsize;
	color = col;
	active = true;
}

void Shot::Burst(Controller* controller, FloorTile*** ftile)
{
	controller->paintFloor(getCenter(), burstSize, color, ftile);
	timer = 0;
	active = false;
}

void Shot::Drip(Controller* controller, FloorTile*** ftile)
{
	controller->paintFloor(getCenter() + Vector2{ (float)GetRandomValue(-5, 5), (float)GetRandomValue(-5, 5) }, dripSize, color, ftile);
}