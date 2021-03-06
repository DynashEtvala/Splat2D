#pragma once
#include "GameObject.h"
class Shot :
	public GameObject
{
private:
	Texture2D tex;
	float damage;
	float range;
	float timer = 0;
	int burstSize;
	int dripSize;
public:
	bool active;
	Color color;
	Shot();
	~Shot();
	void SetNewVals(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, int bsize, int dsize, Color col);
	void Update(class Controller* controller, class FloorTile*** ftile, Rectangle* oblist, Rectangle* pitlist, class SpawnPad* pads, class Player** players);
	void Draw();
	void Burst(class Controller* controller, class FloorTile*** ftile);
	void Drip(class Controller* controller, class FloorTile*** ftile);
};

