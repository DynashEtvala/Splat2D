#include "Shot.h"
#include "Math.h"
#include "Controller.h"
#include "FloorTile.h"
#include "Player.h"

Shot::Shot() : GameObject()
{
	active = false;
}

Shot::~Shot()
{}

void Shot::Update(Controller* controller, FloorTile*** ftile, Rectangle* oblist, Rectangle* pitlist, Player** players)
{
	for (int i = 0; i < maxObsticles; i++)
	{
		if (CheckCollisionCircleRec(getCenter() + Vector2{ direction.x * speed * GetFrameTime(), direction.y * speed * GetFrameTime() }, getRadius(), oblist[i]))
		{
			collisionRect.x += direction.x * speed * GetFrameTime();
			collisionRect.y += direction.y * speed * GetFrameTime();
			Burst(controller, ftile);
			active = false;
			return;
		}
	}
	bool overwater = false;
	for (int i = 0; i < maxObsticles; i++)
	{
		if (CheckCollisionCircleRec(getCenter(), getRadius(), pitlist[i]))
		{
			overwater = true;
			break;
		}
	}
	if (timer < range)
	{
		if (!GetRandomValue(0, 1) && !overwater)
		{
			Drip(controller, ftile);
		}
		collisionRect.x += direction.x * speed * GetFrameTime();
		collisionRect.y += direction.y * speed * GetFrameTime();
		timer += VectorLength(direction) * speed * GetFrameTime();
		for (int i = 0; i < maxPlayerCount; i++)
		{
			if (CheckCollisionCircles(players[i]->getCenter(), players[i]->getRadius(), getCenter(), getRadius()) && players[i]->teamColor != color)
			{
				players[i]->Damaged(damage);
				active = false;
				return;
			}
		}
	}
	else
	{
		collisionRect.x += direction.x * (range - timer);
		collisionRect.y += direction.y * (range - timer);
		for (int i = 0; i < maxPlayerCount; i++)
		{
			if (CheckCollisionCircles(players[i]->getCenter(), players[i]->getRadius(), getCenter(), getRadius()) && players[i]->teamColor != color)
			{
				players[i]->Damaged(damage);
				active = false;
				return;
			}
		}
		if (!overwater)
		{
			Burst(controller, ftile);
		}
		active = false;
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
}

void Shot::Drip(Controller* controller, FloorTile*** ftile)
{
	controller->paintFloor(getCenter() + Vector2{ (float)GetRandomValue(-5, 5), (float)GetRandomValue(-5, 5) }, dripSize, color, ftile);
}