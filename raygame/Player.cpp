#include "Player.h"
#include "Math.h"
#include "Controller.h"
#include "FloorTile.h"


Player::Player() : GameObject()
{
	collisionRect = { 0, 0, 50, 50 };
	weap = BASE_GUN;
	speed = weap.walkSpeed;
	teamColor = BLACK;
	playerNumber = 0;
}


Player::Player(Vector2 start, Weapon w, Color tcol, Color ecol, int pnum) : GameObject()
{
	collisionRect = {start.x, start.y, 50, 50};
	weap = w;
	speed = weap.walkSpeed;
	teamColor = tcol;
	enemyColor = ecol;
	playerNumber = pnum;
	alive = true;
}

Player::~Player()
{}


void Player::Update(Controller* controller, FloorTile*** ftile)
{
	if (alive)
	{
		direction = { GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LEFT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LEFT_Y) };
		if (VectorLength(direction) < 0.05)
		{
			direction = { 0, 0 };
		}

		// shooting
		if (GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RT) > 0.5)
		{
			if (fireTimer >= weap.fireSpeed)
			{
				Rectangle shotRect = { getCenter().x - collisionRect.width / 8, getCenter().y - collisionRect.height / 8, collisionRect.height / 4, collisionRect.width / 4 };
				if (VectorLength({ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_Y) }) > 0.1)
				{
					controller->addShot(shotRect, NormalizeVector(NormalizeVector(Vector2{ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_Y) }) + Vector2{ (float)(GetRandomValue(-weap.accuracy, weap.accuracy)) * 0.01f, (float)(GetRandomValue(-weap.accuracy, weap.accuracy) * 0.01f) }), weap.shotSpeed, weap.damage, weap.range, weap.burstSize, weap.dripSize, teamColor);
				}
				else
				{
					controller->addShot(shotRect, NormalizeVector(direction), weap.shotSpeed, weap.damage, weap.range, weap.burstSize, weap.dripSize, teamColor);
				}
				fireTimer = 0;
			}
		}

		if (fireTimer < weap.fireSpeed)
		{
			fireTimer += GetFrameTime();
		}

		// movement

		if (VectorLength(direction) > 0.1)
		{
			collisionRect.x += direction.x * speed * GetFrameTime();
			collisionRect.y += direction.y * speed * GetFrameTime();
		}
	}
	else
	{
		if (currHealth <= 0)
		{
			controller->paintFloor(getCenter(), deathPopSize, enemyColor, ftile);
		}
	}
}

void Player::Damaged(float dmg)
{
	timeSinceDamaged = 0;
	currHealth -= dmg;
	if (currHealth <= 0)
	{
		currHealth = 0;
		alive = false;
	}
}

float Player::GetHealth()
{
	return currHealth;
}