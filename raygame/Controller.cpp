#include "Controller.h"

#include "Shot.h"
#include "Player.h"

Controller::Controller()
{}

Controller::Controller(Player** plist, int pcount, Shot** slist, int scount)
{
	playerList = plist;
	playerCount = pcount;
	playerChange = false;
	shotList = slist;
	shotCount = scount;
	shotChange = false;
}

Controller::~Controller()
{}

void Controller::addShot(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, Color col)
{
	Shot** hold = new Shot*[shotCount];
	for (int i = 0; i < shotCount; i++)
	{
		hold[i] = new Shot(*shotList[i]);
	}
	for (int i = 0; i < shotCount; i++)
	{
		delete shotList[i];
	}
	shotList = new Shot*[shotCount + 1];
	for (int i = 0; i < shotCount; i++)
	{
		shotList[i] = new Shot(*hold[i]);
	}
	shotList[shotCount + 1] = new Shot(loc, dir, spd, dmg, rng, col);
	for (int i = 0; i < shotCount; i++)
	{
		delete hold[i];
	}
	delete[] hold;
	shotCount++;
	shotChange = true;
}

Player** Controller::getPList()
{
	return playerList;
}

int Controller::getPCount()
{
	return playerCount;
}

Shot** Controller::getSList()
{
	return shotList;
}

int Controller::getSCount()
{
	return shotCount;
}