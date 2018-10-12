#pragma once
#include "GameObject.h"
#include "GlobalVars.h"
class Player :
	public GameObject
{
private:
	float fireTimer = 0;
	float maxHealth = 100;
	float currHealth = maxHealth;
	float ammo = 100;
	Weapon weap;
	int playerNumber;
public:
	Color color;
	Player();
	Player(Vector2 start, Weapon w, Color col);
	~Player();
	void Update(class Controller* controller);
};

