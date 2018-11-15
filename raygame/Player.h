#pragma once
#include "GameObject.h"
class Player
{
private:
	Texture2D body = LoadTexture("Sprites/tankBeige_outline.png");
	Rectangle bodyRect1;
	Rectangle bodyRect2;
	Texture2D barrel = LoadTexture("Sprites/tankBeige");
	Rectangle barrelRect1;
	Rectangle barrelRect2;
	float fireTimer = 0;
	float reloadTimer = 0;
	float maxHealth = 100;
	float currHealth = maxHealth;
	float timeSinceDamaged = 0;
	float ammo = 100;
	float speed;
	Weapon weap;
	SubWeapon sub;
	Vector2 dirBody;
	Vector2 lastDirBody = Vector2{ 0, 1 };
	Vector2 dirBarrel;
	Vector2 lastDirBarrel = Vector2{ 0, 1 };
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
	Direction sideOfRect(Rectangle r);
	Vector2 getCenter();
	float getRadius();
	bool pointOnScreen(int x, int y);
};

