#include "Player.h"
#include "Math.h"
#include "Controller.h"
#include "FloorTile.h"
#include "SpawnPad.h"
#include <math.h>


Player::Player()
{
	weap = BASE_GUN;
	speed = weap.walkSpeed;
	teamColor = BLACK;
	playerNumber = 0;
}


Player::Player(Vector2 start, Weapon w, Color tcol, Color ecol, int pnum)
{
	static Texture2D _body = LoadTexture("Sprites/tankBeige_outline.png");
	static Texture2D _barrel = LoadTexture("Sprites/barrelBeige_outline.png");
	body = _body;
	barrel = _barrel;
	weap = w;
	speed = weap.walkSpeed;
	teamColor = tcol;
	enemyColor = ecol;
	playerNumber = pnum;
	alive = true;
	bodyRect1 = Rectangle{ 0, 0, (float)(body.width), (float)(body.height) };
	bodyRect2 = Rectangle{ start.x, start.y, 35, 35};
	barrelRect1 = Rectangle{ 0, 0, (float)(barrel.width), (float)(barrel.height) };
	barrelRect2 = Rectangle{ start.x, start.y, 8, 22 };
}

Player::~Player()
{}


void Player::Update(Controller* controller, FloorTile*** ftile, SpawnPad * pads, Rectangle * walls, Rectangle * pits)
{
	if (alive)
	{
		swimming = false;
		if (speed != weap.walkSpeed)
		{
			speed = weap.walkSpeed;
		}
		
		//healing + enemy ink damage
		if (pointOnScreen((int)(getCenter().x), (int)(getCenter().y)) && ftile[(int)(getCenter().y)][(int)(getCenter().x)]->color == enemyColor)
		{
			Damaged(0.05f, controller, ftile);
		}
		else if (timeSinceDamaged < 2.5f)
		{
			timeSinceDamaged += GetFrameTime();
		}
		else if(currHealth < maxHealth)
		{
			currHealth += GetFrameTime() * 40;
			if (currHealth > maxHealth)
			{
				currHealth = maxHealth;
			}
		}

		if (dirBody.x != 0 && dirBody.y != 0)
		{
			lastDirBody = NormalizeVector(dirBody);
		}
		dirBody = Vector2{ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LEFT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LEFT_Y) };
		if (VectorLength(dirBody) < 0.1)
		{
			dirBody = Vector2{ 0, 0 };
		}

		if (dirBarrel.x != 0 && dirBarrel.y != 0)
		{
			lastDirBarrel = dirBarrel;
		}
		dirBarrel = Vector2{ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_Y) };
		if (VectorLength(dirBarrel) < 0.1)
		{
			dirBarrel = Vector2{ 0, 0 };
		}
		NormalizeVector(&dirBarrel);

		// shooting
		if (GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RT) > 0.2f && (dirBarrel.y != 0 || dirBarrel.x != 0))
		{
			speed = weap.shootingSpeed;
			if (fireTimer >= weap.fireRate && ammo > weap.ammoConsume && VectorLength(dirBarrel) > 0.1)
			{
				controller->addShot(Rectangle{ bodyRect2.x + bodyRect2.width / 2, bodyRect2.y + bodyRect2.height / 2, 10, 10 }, NormalizeVector(dirBarrel + Vector2{GetRandomValue( -weap.accuracy, weap.accuracy) * 0.01f, GetRandomValue(-weap.accuracy, weap.accuracy) * 0.01f }), weap.bulletSpeed, weap.damage, weap.range, weap.burstSize, weap.dripSize, teamColor);
				ammo -= weap.ammoConsume;
				fireTimer = 0;
			}
			reloadTimer = 0;
		}
		else if (GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_LT) > 0.2f)
		{
			swimming = true;
			if (ftile[(int)(getCenter().y)][(int)(getCenter().x)]->color == teamColor)
			{
				speed = swimSpeed;
				reloadTimer = 5;
				ammo += 20 * GetFrameTime();
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

		if (reloadTimer < 1)
		{
			reloadTimer += GetFrameTime();
		}
		else
		{
			if (ammo < 100)
			{
				ammo += 20 * GetFrameTime();
			}
			if (ammo > 100)
			{
				ammo = 100;
			}
		}

		// movement

		if (VectorLength(dirBody) >= 0.1)
		{
			barrelRect2.x = bodyRect2.x += dirBody.x * speed * GetFrameTime();
			barrelRect2.y = bodyRect2.y += dirBody.y * speed * GetFrameTime();
			for (int i = 0; i < maxObsticles; i++)
			{
				if (CheckCollisionPointRec(getCenter(), pits[i]))
				{
					currHealth = 0;
					barrelRect2.x = bodyRect2.x = -100;
					barrelRect2.y = bodyRect2.y = -100;
					alive = false;
				}
			}
			for (int i = 0; i < maxObsticles; i++)
			{
				if (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
				{
					switch (sideOfRect(walls[i]))
					{
					case LEFT:
						barrelRect2.x = bodyRect2.x = walls[i].x - bodyRect2.width + 0.83f;
						break;
					case RIGHT:
						barrelRect2.x = bodyRect2.x = walls[i].x + walls[i].width;
						break;
					case UP:
						barrelRect2.y = bodyRect2.y = walls[i].y - bodyRect2.height + 0.83f;
						break;
					case DOWN:
						barrelRect2.y = bodyRect2.y = walls[i].y + walls[i].height;
						break;
					case TOPLEFT:
						if (dirBody.x < dirBody.y)
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.y--;
							}
							barrelRect2.y = bodyRect2.y += 1 - (bodyRect2.y - (int)(bodyRect2.y));
						}
						else
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.x--;
							}
							barrelRect2.x = bodyRect2.x += 1 - (bodyRect2.x - (int)(bodyRect2.x));
						}
						break;
					case TOPRIGHT:
						if (dirBody.x * -1 < dirBody.y)
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.y--;
							}
							barrelRect2.y = bodyRect2.y += 1 - (bodyRect2.y - (int)(bodyRect2.y));
						}
						else
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.x++;
							}
							bodyRect2.x -= bodyRect2.x - (int)(bodyRect2.x);
						}
						break;
					case BOTTOMLEFT:
						if (dirBody.x < dirBody.y * -1)
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.y++;
							}
							bodyRect2.y -= bodyRect2.y - (int)(bodyRect2.y);
						}
						else
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.x--;
							}
							barrelRect2.x = bodyRect2.x += 1 - (bodyRect2.x - (int)(bodyRect2.x));
						}
						break;
					case BOTTOMRIGHT:
						if (dirBody.x * -1 < dirBody.y * -1)
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.y++;
							}
							bodyRect2.y -= bodyRect2.y - (int)(bodyRect2.y);
						}
						else
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.x++;
							}
							bodyRect2.x -= bodyRect2.x - (int)(bodyRect2.x);
						}
						break;
					default:
					case CENTER:
						bodyRect2.x -= dirBody.x * speed * GetFrameTime();
						bodyRect2.y -= dirBody.y * speed * GetFrameTime();
						break;
					}
				}
			}
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
				barrelRect2.x = bodyRect2.x = spawn.x;
				barrelRect2.y = bodyRect2.y = spawn.y;
				ammo = 100;
				alive = true;
			}
		}
	}
}

void Player::Draw()
{
	if (!swimming)
	{
		if (dirBody.x != 0 && dirBody.y != 0)
		{
			Vector2 norm = NormalizeVector(dirBody);
			float rot = -atan2f(norm.x, norm.y) * 180 / PI;
			DrawTexturePro(body, bodyRect1, Rectangle{ bodyRect2.x + bodyRect2.width / 2, bodyRect2.y + bodyRect2.height / 2, bodyRect2.width, bodyRect2.height }, Vector2{ bodyRect2.width / 2, bodyRect2.height / 2 }, rot - 180, teamColor);
		}
		else
		{
			Vector2 norm = NormalizeVector(lastDirBody);
			float rot = -atan2f(norm.x, norm.y) * 180 / PI;
			DrawTexturePro(body, bodyRect1, Rectangle{ bodyRect2.x + bodyRect2.width / 2, bodyRect2.y + bodyRect2.height / 2, bodyRect2.width, bodyRect2.height }, Vector2{ bodyRect2.width / 2, bodyRect2.height / 2 }, rot - 180, teamColor);
		}
	}
	if (dirBarrel.x != 0 && dirBarrel.y != 0)
	{
		Vector2 norm = NormalizeVector(dirBarrel);
		float rot = -atan2f(norm.x, norm.y) * 180 / PI;
		DrawTexturePro(barrel, barrelRect1, Rectangle{ bodyRect2.x + bodyRect2.width / 2, bodyRect2.y + bodyRect2.height / 2, barrelRect2.width, barrelRect2.height }, Vector2{ barrelRect2.width / 2, barrelRect2.height }, rot - 180, teamColor);
	}
	else
	{
		Vector2 norm = NormalizeVector(lastDirBarrel);
		float rot = -atan2f(norm.x, norm.y) * 180 / PI;
		DrawTexturePro(barrel, barrelRect1, Rectangle{ bodyRect2.x + bodyRect2.width / 2, bodyRect2.y + bodyRect2.height / 2, barrelRect2.width, barrelRect2.height }, Vector2{ barrelRect2.width / 2, barrelRect2.height }, rot - 180, teamColor);
	}

	DrawRectangle(bodyRect2.x + bodyRect2.width, bodyRect2.y, 10, 35, BLACK);
	DrawRectangle(bodyRect2.x + bodyRect2.width + 1, bodyRect2.y + 1, 8, 33, WHITE);
	DrawRectangle(bodyRect2.x + bodyRect2.width + 1, bodyRect2.y + 34 - ammo * 0.33f, 8, ammo * 0.33f, teamColor);
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
			barrelRect2.x = bodyRect2.x = -100;
			barrelRect2.y = bodyRect2.y = -100;
			alive = false;
		}
	}
}

float Player::GetHealth()
{
	return currHealth;
}

Direction Player::sideOfRect(Rectangle r)
{
	if (getCenter().x < r.x)
	{
		if (getCenter().y < r.y)
		{
			return TOPLEFT;
		}
		else if (getCenter().y > r.y + r.height)
		{
			return BOTTOMLEFT;
		}
		return LEFT;
	}
	else if (getCenter().x > r.x + r.width)
	{
		if (getCenter().y < r.y)
		{
			return TOPRIGHT;
		}
		else if (getCenter().y > r.y + r.height)
		{
			return BOTTOMRIGHT;
		}
		return RIGHT;
	}
	else if (getCenter().y < r.y)
	{
		return UP;
	}
	else if (getCenter().y > r.y + r.height)
	{
		return DOWN;
	}
	else
	{
		return CENTER;
	}
}

Vector2 Player::getCenter()
{
	return Vector2{ bodyRect2.x + getRadius(), bodyRect2.y + getRadius() };
}

float Player::getRadius()
{
	return bodyRect2.height < bodyRect2.width ? bodyRect2.height / 2 : bodyRect2.width / 2;
}

bool Player::pointOnScreen(int x, int y)
{
	if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
	{
		return true;
	}
	return false;
}