#pragma once
#include "GameObject.h"
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
	bool swimming = false;
public:
	Color teamColor;
	Color enemyColor;
	Player();
	Player(Vector2 start, Weapon w, Color tcol, Color ecol, int pnum);
	~Player();
	void Update(class Controller* controller, class FloorTile*** ftile, class SpawnPad * pads, Rectangle * walls, Rectangle * pits);
	void Draw();
	void Damaged(float dmg, Controller* controller, FloorTile*** ftile);
	float GetHealth();
};

