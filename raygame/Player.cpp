#include "Player.h"
#include "Math.h"
#include "Controller.h"
#include "FloorTile.h"
#include "SpawnPad.h"


Player::Player() : GameObject()
{
	collisionRect = { 0, 0, 35, 35 };
	weap = BASE_GUN;
	speed = weap.walkSpeed;
	teamColor = BLACK;
	playerNumber = 0;
}


Player::Player(Vector2 start, Weapon w, Color tcol, Color ecol, int pnum) : GameObject()
{
	collisionRect = {start.x, start.y, 35, 35};
	weap = w;
	speed = weap.walkSpeed;
	teamColor = tcol;
	enemyColor = ecol;
	playerNumber = pnum;
	alive = true;
}

Player::~Player()
{}


void Player::Update(Controller* controller, FloorTile*** ftile, SpawnPad * pads, Rectangle * walls, Rectangle * pits)
{
	if (alive)
	{
		if (speed != weap.walkSpeed)
		{
			speed = weap.walkSpeed;
		}
		
		//healing + enemy ink damage
		if (pointOnScreen((int)(getCenter().x), (int)(getCenter().y)) && ftile[(int)(getCenter().y)][(int)(getCenter().x)]->color == enemyColor)
		{
			Damaged(0.05f, controller, ftile);
		}
		else if (timeSinceDamaged < 5)
		{
			timeSinceDamaged += GetFrameTime();
		}
		else if(currHealth < maxHealth)
		{
			currHealth += GetFrameTime() * 25;
			if (currHealth > maxHealth)
			{
				currHealth = maxHealth;
			}
		}

		direction = { GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LEFT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LEFT_Y) };
		if (VectorLength(direction) < 0.1)
		{
			direction = { 0, 0 };
		}

		// shooting
		if (GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RT) > 0.5f)
		{
			speed = weap.shootingSpeed;
			if (fireTimer >= weap.fireRate)
			{
				Rectangle shotRect = { getCenter().x - collisionRect.width / 8, getCenter().y - collisionRect.height / 8, collisionRect.height / 4, collisionRect.width / 4 };
				if (VectorLength({ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_Y) }) > 0.1)
				{
					controller->addShot(shotRect, NormalizeVector(NormalizeVector(Vector2{ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_Y) }) + Vector2{ (float)(GetRandomValue(-weap.accuracy, weap.accuracy)) * 0.01f, (float)(GetRandomValue(-weap.accuracy, weap.accuracy) * 0.01f) }), weap.bulletSpeed, weap.damage, weap.range, weap.burstSize, weap.dripSize, teamColor);
				}
				else
				{
					controller->addShot(shotRect, NormalizeVector(direction), weap.bulletSpeed, weap.damage, weap.range, weap.burstSize, weap.dripSize, teamColor);
				}
				fireTimer = 0;
			}
		}
		else if (GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LT) > 0.5f)
		{
			if (ftile[(int)(getCenter().y)][(int)(getCenter().x)]->color == teamColor)
			{
				speed = swimSpeed;
			}
			else
			{
				speed = drySwimSpeed;
			}
		}

		if (pointOnScreen((int)(getCenter().x), (int)(getCenter().y)) && ftile[(int)(getCenter().y)][(int)(getCenter().x)]->color == enemyColor)
		{
			speed = enemyInkSpeed;
		}

		if (fireTimer < weap.fireRate)
		{
			fireTimer += GetFrameTime();
		}

		// movement

		if (VectorLength(direction) >= 0.1)
		{
			collisionRect.x += direction.x * speed * GetFrameTime();
			collisionRect.y += direction.y * speed * GetFrameTime();
		}
	}
	else
	{
		if (timeSinceDamaged < 5)
		{
			timeSinceDamaged += GetFrameTime();
		}
		else if (currHealth < maxHealth)
		{
			currHealth += GetFrameTime() * 25;
			if (currHealth > maxHealth)
			{
				currHealth = maxHealth;
				Vector2 spawn = pads[playerNumber % 2].spawnSpaces[playerNumber/2];
				collisionRect.x = spawn.x;
				collisionRect.y = spawn.y;
				alive = true;
			}
		}
	}
}

void Player::Draw()
{
	DrawCircle(getCenter().x, getCenter().y, getRadius(), BLACK);
	DrawCircle(getCenter().x, getCenter().y, getRadius() - 1, enemyColor);
	DrawCircle(getCenter().x, getCenter().y, ((getRadius() - 1) / 100) * GetHealth(), teamColor);
}

void Player::Damaged(float dmg, Controller* controller, FloorTile*** ftile)
{
	if (alive)
	{
		timeSinceDamaged = 0;
		currHealth -= dmg;
		if (currHealth <= 0)
		{
			currHealth = 0;
			controller->paintFloor(getCenter(), deathPopSize, enemyColor, ftile);
			collisionRect.x = -100;
			collisionRect.y = -100;
			alive = false;
		}
	}
}

float Player::GetHealth()
{
	return currHealth;
}