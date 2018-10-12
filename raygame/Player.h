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
	float timeSinceDamaged = 0;
	float ammo = 100;
	Weapon weap;
	int playerNumber;
	bool alive = false;
public:
	Color teamColor;
	Color enemyColor;
	Player();
	Player(Vector2 start, Weapon w, Color tcol, Color ecol, int pnum);
	~Player();
	void Update(class Controller* controller, class FloorTile*** ftile);
	void Damaged(float dmg);
	float GetHealth();
};

