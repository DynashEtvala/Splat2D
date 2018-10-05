#pragma once
#include "raylib.h"

class Controller
{
private:
	class Player** playerList;
	int playerCount;
	class Shot** shotList;
	int shotCount;
public:
	bool playerChange;
	bool shotChange;
	Controller();
	Controller(class Player** plist, int pcount, class Shot** slist, int scount);
	~Controller();
	void addShot(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, Color col);
	class Player** getPList();
	int getPCount();
	class Shot** getSList();
	int getSCount();
};

