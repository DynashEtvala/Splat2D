#include "Player.h"
#include "Math.h"
#include "Controller.h"


Player::Player() : GameObject()
{
	collisionRect = { 0, 0, 50, 50 };
	speed = 100;
	fireRate = 1;
	mainAmmoConsume = 4;
	subAmmoConsume = 60;
	color = BLACK;
}


Player::Player(Vector2 start, float spd, float frate, float mcon, float scon, Color col) : GameObject()
{
	collisionRect = {start.x, start.y, 50, 50};
	speed = spd;
	fireRate = frate;
	mainAmmoConsume = mcon;
	subAmmoConsume = scon;
	color = col;
}

Player::~Player()
{}


void Player::Update(Controller controller)
{
	// shooting
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		if (fireTimer >= fireRate)
		{
			controller.addShot(collisionRect, NormalizeVector(&GetMousePosition()), 100, 10, 1, color);
		}
	}

	if (fireTimer < fireRate)
	{
		fireTimer += GetFrameTime();
	}

	// movement
	if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) // left
	{
		direction.x = -1;
	}
	else if (!IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) // right
	{
		direction.x = 1;
	}
	else // neither
	{
		direction.x = 0;
	}

	if (IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) // up
	{
		direction.y = -1;
	}
	else if (!IsKeyDown(KEY_W) && IsKeyDown(KEY_S)) // down
	{
		direction.y = 1;
	}
	else // neither
	{
		direction.y = 0;
	}

	NormalizeVector(&direction);

	collisionRect.x += direction.x * speed * GetFrameTime();
	collisionRect.y += direction.y * speed * GetFrameTime();
}
