#pragma once
#include "GameObject.h"
class Player :
	public GameObject
{
private:
	float fireRate;
	float fireTimer = 0;
	float maxHealth = 100;
	float currHealth = maxHealth;
	float ammo = 100;
	float mainAmmoConsume;
	float subAmmoConsume;
public:
	Color color;
	Player();
	Player(Vector2 start, float spd, float frate, float mcon, float scon, Color col);
	~Player();
	void Update(class Controller controller);
};

