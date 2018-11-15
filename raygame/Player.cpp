#include "Player.h"
#include "Math.h"
#include "Controller.h"
#include "FloorTile.h"
#include "SpawnPad.h"


Player::Player()
{
	weap = BASE_GUN;
	speed = weap.walkSpeed;
	teamColor = BLACK;
	playerNumber = 0;
}


Player::Player(Vector2 start, Weapon w, Color tcol, Color ecol, int pnum)
{
	weap = w;
	speed = weap.walkSpeed;
	teamColor = tcol;
	enemyColor = ecol;
	playerNumber = pnum;
	alive = true;
	bodyRect1 = Rectangle{ 0, 0, (float)(body.width), (float)(body.height) };
	bodyRect2 = Rectangle{ start.x, start.y, 35, 35};
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
			dirBody = { 0, 0 };
		}

		// shooting
		if (GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RT) > 0.2f)
		{
			speed = weap.shootingSpeed;
			if (fireTimer >= weap.fireRate && ammo > weap.ammoConsume)
			{
				Rectangle shotRect = { getCenter().x - bodyRect2.width / 8, getCenter().y - bodyRect2.height / 8, bodyRect2.height / 4, bodyRect2.width / 4 };
				if (VectorLength({ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_Y) }) > 0.1)
				{
					controller->addShot(shotRect, NormalizeVector(NormalizeVector(Vector2{ GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_X), GetGamepadAxisMovement(playerNumber, GAMEPAD_XBOX_AXIS_RIGHT_Y) }) + Vector2{ (float)(GetRandomValue(-weap.accuracy, weap.accuracy)) * 0.01f, (float)(GetRandomValue(-weap.accuracy, weap.accuracy) * 0.01f) }), weap.bulletSpeed, weap.damage, weap.range, weap.burstSize, weap.dripSize, teamColor);
				}
				else
				{
					controller->addShot(shotRect, NormalizeVector(dirBody + Vector2{ (float)(GetRandomValue(-weap.accuracy, weap.accuracy)) * 0.01f, (float)(GetRandomValue(-weap.accuracy, weap.accuracy) * 0.01f) }), weap.bulletSpeed, weap.damage, weap.range, weap.burstSize, weap.dripSize, teamColor);
				}
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
			bodyRect2.x += dirBody.x * speed * GetFrameTime();
			bodyRect2.y += dirBody.y * speed * GetFrameTime();
			for (int i = 0; i < maxObsticles; i++)
			{
				if (CheckCollisionPointRec(getCenter(), pits[i]))
				{
					currHealth = 0;
					bodyRect2.x = -100;
					bodyRect2.y = -100;
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
						bodyRect2.x = walls[i].x - bodyRect2.width + 0.83f;
						break;
					case RIGHT:
						bodyRect2.x = walls[i].x + walls[i].width;
						break;
					case UP:
						bodyRect2.y = walls[i].y - bodyRect2.height + 0.83f;
						break;
					case DOWN:
						bodyRect2.y = walls[i].y + walls[i].height;
						break;
					case TOPLEFT:
						if (dirBody.x < dirBody.y)
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.y--;
							}
							bodyRect2.y += 1 - (bodyRect2.y - (int)(bodyRect2.y));
						}
						else
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.x--;
							}
							bodyRect2.x += 1 - (bodyRect2.x - (int)(bodyRect2.x));
						}
						break;
					case TOPRIGHT:
						if (dirBody.x * -1 < dirBody.y)
						{
							while (CheckCollisionCircleRec(getCenter(), getRadius(), walls[i]))
							{
								bodyRect2.y--;
							}
							bodyRect2.y += 1 - (bodyRect2.y - (int)(bodyRect2.y));
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
							bodyRect2.x += 1 - (bodyRect2.x - (int)(bodyRect2.x));
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
				bodyRect2.x = spawn.x;
				bodyRect2.y = spawn.y;
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
		DrawTexturePro(body, bodyRect1, bodyRect2, Vector2{ bodyRect2.width / 2, bodyRect2.height / 2 }, 90, teamColor);
		//DrawCircle(getCenter().x, getCenter().y, getRadius(), BLACK);
		//DrawCircle(getCenter().x, getCenter().y, getRadius() - 1, enemyColor);
		//DrawCircle(getCenter().x, getCenter().y, ((getRadius() - 1) / 100) * GetHealth(), teamColor);
	}
	else
	{
		if (dirBody.x != 0 && dirBody.y != 0)
		{
			Vector2 dirBodyD = NormalizeVector(dirBody);
			DrawTexturePro(body, bodyRect1, bodyRect2, Vector2{ bodyRect2.width / 2, bodyRect2.height / 2 }, 0, teamColor);
			//DrawTriangle(dirBodyD * (getRadius() + getRadius() / 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ -dirBodyD.y, dirBodyD.x }) * (-getRadius() - getRadius() / 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ dirBodyD.y, -dirBodyD.x }) * (-getRadius() - getRadius() / 2) + getCenter(), BLACK);
			//DrawTriangle(dirBodyD * (getRadius() + getRadius() / 2 - 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ -dirBodyD.y, dirBodyD.x }) * (-getRadius() - getRadius() / 2 + 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ dirBodyD.y, -dirBodyD.x }) * (-getRadius() - getRadius() / 2 + 2) + getCenter(), enemyColor);
			//DrawTriangle(dirBodyD * ((getRadius() + getRadius() / 2 - 2) / 100) * GetHealth() + getCenter(), NormalizeVector((dirBodyD)+Vector2{ -dirBodyD.y, dirBodyD.x }) * ((-getRadius() - getRadius() / 2 + 2) / 100) * GetHealth() + getCenter(), NormalizeVector((dirBodyD)+Vector2{ dirBodyD.y, -dirBodyD.x }) * ((-getRadius() - getRadius() / 2 + 2) / 100) * GetHealth() + getCenter(), teamColor);
		}
		else
		{
			Vector2 dirBodyD = NormalizeVector(lastDirBody);
			DrawTexturePro(body, bodyRect1, bodyRect2, Vector2{ bodyRect2.width / 2, bodyRect2.height / 2 }, 180, teamColor);
			//DrawTriangle(dirBodyD * (getRadius() + getRadius() / 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ -dirBodyD.y, dirBodyD.x }) * (-getRadius() - getRadius() / 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ dirBodyD.y, -dirBodyD.x }) * (-getRadius() - getRadius() / 2) + getCenter(), BLACK);
			//DrawTriangle(dirBodyD * (getRadius() + getRadius() / 2 - 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ -dirBodyD.y, dirBodyD.x }) * (-getRadius() - getRadius() / 2 + 2) + getCenter(), NormalizeVector((dirBodyD)+Vector2{ dirBodyD.y, -dirBodyD.x }) * (-getRadius() - getRadius() / 2 + 2) + getCenter(), enemyColor);
			//DrawTriangle(dirBodyD * ((getRadius() + getRadius() / 2 - 2) / 100) * GetHealth() + getCenter(), NormalizeVector((dirBodyD)+Vector2{ -dirBodyD.y, dirBodyD.x }) * ((-getRadius() - getRadius() / 2 + 2) / 100) * GetHealth() + getCenter(), NormalizeVector((dirBodyD)+Vector2{ dirBodyD.y, -dirBodyD.x }) * ((-getRadius() - getRadius() / 2 + 2) / 100) * GetHealth() + getCenter(), teamColor);
		}
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
			bodyRect2.x = -100;
			bodyRect2.y = -100;
			alive = false;
		}
	}
}

float Player::GetHealth()
{
	return currHealth;
}

dirBody Player::sideOfRect(Rectangle r)
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